#include "StdAfx.h"

#include <atlbase.h>
#include <strsafe.h>
#include "SocketThreadHandler.h"
#include <iostream>


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
		LOG_WARN( m_loacalAddr<< " received error message from " << addr);
		return;
	}

	m_recvBuf.Write(pbData,dwCount);

}



void CSocketThreadHandler::OnThreadBegin( CSocketHandle* pSH )
{
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
		LOG_ERROR("start thread error:"<<e.what());
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
	this->Stop();
	
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
		 nRead = m_recvBuf.Read((BYTE*)&head,headLen);
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
		nRead = m_recvBuf.Read(&m_MsgRecv[headLen], head.msgLength-headLen);
		if (nRead != head.msgLength-headLen)
		{
			return false;
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


std::ostream& operator<<( std::ostream& output,const SockAddrIn& obj )
{
	CString info;
	CSocketThreadHandler::GetAddress(obj,info);
	output<<info.GetString();
	return output;
}

std::wostream& operator<<( std::wostream& output,const  SockAddrIn& obj )
{
	CString info;
	CSocketThreadHandler::GetAddress(obj,info);
	output<<info.GetString();
	return output;
}
