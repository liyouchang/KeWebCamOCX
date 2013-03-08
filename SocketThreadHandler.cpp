#include "StdAfx.h"

#include <atlbase.h>
#include <strsafe.h>
#include "SocketThreadHandler.h"
#include "LogModule/LogModule.h"


CSocketThreadHandler::CSocketThreadHandler(void)
{
	m_SocketClient.SetInterface(this);
}

CSocketThreadHandler::~CSocketThreadHandler(void)
{
}


void CSocketThreadHandler::OnDataReceived( CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr )
{
	ASSERT( pSH == m_SocketClient );
	(pSH);
	CString  strText;
	USES_CONVERSION;
	LPTSTR pszText = strText.GetBuffer(dwCount+1);
	::StringCchCopyN(pszText, dwCount+1, A2CT(reinterpret_cast<LPCSTR>(pbData)), dwCount);
	strText.ReleaseBuffer();
	LOG_DEBUG( addr<<">>>>"<< strText);

	if (!CheckMessage(pbData,dwCount))
	{
		LOG_WARN( m_loacalAddr<< " received error message from "<< addr);
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
		LOG_ERROR("start thread error");
	}
}

BOOL CSocketThreadHandler::Init( const UINT nCnt )
{

	m_recvBuf.Init(nCnt);
	return TRUE;
}

void CSocketThreadHandler::Run()
{
	while (m_SocketClient->IsOpen() && !this->toStop)
	{
		if (m_recvBuf.GetCount() == 0)
		{
			DWORD dw = m_recvBuf.WaitForNewData();
			switch (dw){
				case WAIT_FAILED:	
					LOG_WARN("WaitForSingleObject failed");
					break;
				case WAIT_TIMEOUT:	
					LOG_WARN("WaitForSingleObject wait timeout");
					break;
				case WAIT_OBJECT_0://success
					break;
			}
		}
		this->HandleMessage();
	}
}

void CSocketThreadHandler::OnThreadExit( CSocketHandle* pSH )
{
	this->Stop();
	this->Join();
}

void CSocketThreadHandler::GetAddress( const SockAddrIn& addrIn, CString& rString )
{
	TCHAR szIPAddr[MAX_PATH] = { 0 };
	CSocketHandle::FormatIP(szIPAddr, MAX_PATH, addrIn);
	rString.Format(_T("%s : %d"), szIPAddr, static_cast<int>(static_cast<UINT>(ntohs(addrIn.GetPort()))) );
}

bool CSocketThreadHandler::CheckMessage( const BYTE* data, DWORD dwCount )
{
	PKEMsgHead head = (PKEMsgHead)data;
	if (head->length != dwCount)
	{
		return false;
	}

	return true;
}

std::ostream& operator<<( std::ostream& output, SockAddrIn& obj )
{
	CString info;
	CSocketThreadHandler::GetAddress(obj,info);
	output<<info;
	return output;
}

std::wostream& operator<<( std::wostream& output, SockAddrIn& obj )
{
	CString info;
	CSocketThreadHandler::GetAddress(obj,info);
	output<<info;
	return output;
}
