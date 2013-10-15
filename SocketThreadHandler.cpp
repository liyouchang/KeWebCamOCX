#include "StdAfx.h"

#include <atlbase.h>
#include <strsafe.h>
#include "SocketThreadHandler.h"
#include <iostream>
using namespace std;

CSocketThreadHandler::CSocketThreadHandler(void)
{
	m_SocketClient.SetInterface(this);
}

CSocketThreadHandler::CSocketThreadHandler( const UINT nCnt )
{
	m_SocketClient.SetInterface(this);
	Init(nCnt);
}

CSocketThreadHandler::~CSocketThreadHandler(void)
{
	
}


void CSocketThreadHandler::OnDataReceived( CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr )
{
	ASSERT( pSH == m_SocketClient );
	(pSH);
	//CString  strText;
	//USES_CONVERSION;
	//LPTSTR pszText = strText.GetBuffer(dwCount+1);
	//::StringCchCopyN(pszText, dwCount+1, A2CT(reinterpret_cast<LPCSTR>(pbData)), dwCount);
	//strText.ReleaseBuffer();
	//LOG_DEBUG( "Get"<< addr <<">>>>"<< strText.GetString());

	if (!CheckMessage(pbData,dwCount))
	{
		//LOG_WARN( m_loacalAddr<< " received error message from " << addr);
		return;
	}

	m_recvBuf.Write(pbData,dwCount);

}



void CSocketThreadHandler::OnThreadBegin( CSocketHandle* pSH )
{
	//LOG_INFO("Connection "<<m_loacalAddr<<" thread start");
	try
	{
		this->Start();
		if (!pSH->GetSockName(m_loacalAddr))
		{
			LOG_ERROR("pSH->GetSockName Error");
		}
	}
	catch (std::exception& e)
	{
		//LOG_ERROR("start thread error:"<<e.what());
	}
}

bool CSocketThreadHandler::Init( const UINT nCnt )
{

	BOOL ret =  m_recvBuf.Init(nCnt);
	if (ret == FALSE)
	{
		return false;
	}
	return  true;
}

void CSocketThreadHandler::Run()
{

	while (m_SocketClient->IsOpen() && !this->toStop)
	{
		if (m_recvBuf.GetCount() == 0 )
		{
			DWORD dw = m_recvBuf.WaitForNewData(10000);
			if (dw ==WAIT_TIMEOUT )
			{
				//LOG_WARN("WaitForSingleObject wait timeout\n");
				continue;
			}
		}		
		
		if (!GetMessageData())
		{
			Sleep(10);
			continue;
		}
		this->HandleMessage(&m_MsgRecv[0]);
		m_MsgRecv.clear();

	}

}

void CSocketThreadHandler::OnThreadExit( CSocketHandle* pSH )
{
	//LOG_INFO("Connection "<<m_loacalAddr<<" thread exit");
	//停止缓存数据线程
	this->Stop();
	//pSH->Close();	
}

void CSocketThreadHandler::GetAddress( const SockAddrIn& addrIn, CString& rString )
{
	TCHAR szIPAddr[MAX_PATH] = { 0 };
	CSocketHandle::FormatIP(szIPAddr, MAX_PATH, addrIn);
	rString.Format(_T("%s:%d"), szIPAddr, static_cast<int>(static_cast<UINT>(ntohs(addrIn.GetPort()))) );
}

bool CSocketThreadHandler::CheckMessage( const BYTE* data, DWORD dwCount )
{
// 	PKEMsgHead pHead = (PKEMsgHead)data;
// 	if (pHead->msgLength != dwCount)
// 	{
// 		return false;
// 	}

	return true;
}

bool CSocketThreadHandler::IsConnect()
{
		return m_SocketClient.IsOpen();
}

void CSocketThreadHandler::CloseConnect()
{

	m_SocketClient.Terminate();

}

DWORD CSocketThreadHandler::Write( const LPBYTE lpBuffer,DWORD dwCount )
{
	if ( m_SocketClient.IsOpen() )
	{
		if (m_nSockType == SOCK_TCP)
		{
			return m_SocketClient.Write(lpBuffer, dwCount, NULL);
		}
		else
		{
			SockAddrIn sockAddr;
			sockAddr.CreateFrom(m_serverIP, m_serverPort, AF_INET);
			return m_SocketClient.Write(lpBuffer, dwCount, sockAddr);
		}
	}
	else
	{
		return -1;
	}
}

bool CSocketThreadHandler::GetMessageData()
{
	int nRead= 0;
	int headLen = sizeof(KEMsgHead);
	if (m_MsgRecv.size() == 0)//上一个消息已经读取完成
	{
		KEMsgHead head;
		nRead = m_recvBuf.Read( (BYTE*)&head , headLen );
		if (nRead != headLen)
		{
			return false;
		}
		if (head.protocal != PROTOCOL_HEAD)
		{
			LOG_ERROR("The message Protocal Head error, Clear the recv buffer!");
			m_recvBuf.Drain(m_recvBuf.GetCount());
			m_MsgRecv.clear();
			return false;
		}
		m_MsgRecv.resize(head.msgLength,0);
		memcpy(&m_MsgRecv[0],&head,headLen);
		if (head.msgLength-headLen != 0)//防止 headLen 越界
		{
			nRead = m_recvBuf.Read(&m_MsgRecv[headLen], head.msgLength-headLen);
			if (nRead != head.msgLength-headLen)
			{
				return false;
			}
		}
	}
	else//上一个消息未完成读取
	{
		int waitRecv = m_MsgRecv.size() - headLen;
		nRead = m_recvBuf.Read(&m_MsgRecv[headLen],waitRecv);
		if (nRead != waitRecv)
		{
			//TRACE("Read fifo buffer error:1 ");
			return false;
		}
	}
	
	return true;
}

int CSocketThreadHandler::WaitRecvMsg( int msgType , int timeout /*= MSG_WAIT_TIMEOUT*/,void ** respPtr /*= NULL*/ )
{
	TRACE2("WaitMsg %d start at %d\n",msgType,GetTickCount());
	msgEventMap[msgType].ResetMsgEvent(msgType);
	DWORD dw = msgEventMap[msgType].WaitMsgEvent(timeout);
	if (dw == WAIT_TIMEOUT)
	{
		//LOG_ERROR("Wait msg event time out, msgType = "<< msgType);
		return KE_MSG_TIMEOUT;
	}
	TRACE2("WaitMsg %d success at %d\n",msgType,GetTickCount());
	if (respPtr != NULL)
	{
		*respPtr = msgEventMap[msgType].respPtr;
	}
	return msgEventMap[msgType].respData;
}

void CSocketThreadHandler::SetRecvMsg( int msgType,int respData,void * respPtr )
{
 	Sleep(10);
	msgEventMap[msgType].respData = respData;
	msgEventMap[msgType].respPtr = respPtr;
 	msgEventMap[msgType].SetMsgEvent();
	TRACE2("SetRecvMsg %d success at %d\n",msgType,GetTickCount());
}
//返回消息响应数据
int CSocketThreadHandler::GetRecvMsgData( int msgType ,void ** respPtr /*= NULL*/ )
{
	if (respPtr != NULL)
	{
		*respPtr = msgEventMap[msgType].respPtr;
	}
 	return msgEventMap[msgType].respData;
}

void CSocketThreadHandler::OnConnectionDropped( CSocketHandle* pSH )
{
	trace("OnConnectionDropped-------\n");
	pSH->Close();
}

void CSocketThreadHandler::OnConnectionError( CSocketHandle* pSH, DWORD dwError )
{
	trace("OnConnectionError-------\n");
	pSH->Close();
}


// std::ostream& operator<<( std::ostream& output,const SockAddrIn& obj )
// {
// 	CString info;
// 	CSocketThreadHandler::GetAddress(obj,info);
// 	//output<<info.GetString();
// 	return output;
// }

// std::wostream& operator<<( std::wostream& output,const  SockAddrIn& obj )
// {
// 	CString info;
// 	CSocketThreadHandler::GetAddress(obj,info);
// 	//output<<info.GetString();
// 	return output;
// }

RecvMsgEvent::RecvMsgEvent()
{
	TRACE("RecvMsgEvent construct\n");
	msgType = 0;
	pEvent = NULL;
	respPtr = NULL;
}


RecvMsgEvent::~RecvMsgEvent()
{
	TRACE1("~RecvMsgEvent %d\n",msgType);
	if (respPtr != NULL)
	{
		delete respPtr;
		respPtr = NULL;
	}
	if (pEvent != NULL)
	{
		delete pEvent;
		pEvent = NULL;
	}
}

void RecvMsgEvent::ResetMsgEvent( int msgType /*= 0*/ )
{
	this->msgType = msgType;
	if (pEvent ==NULL)
	{
		TRACE1("Init event %d",msgType);
		pEvent = new CEvent();
	}
	pEvent->ResetEvent();
	if (respPtr != NULL)//这里删除respPtr，让使用者无需delete
	{
		delete respPtr;
		respPtr = NULL;
	}
}

DWORD RecvMsgEvent::WaitMsgEvent( int timeout )
{
	return	WaitForSingleObject(pEvent->m_hObject,timeout);
}