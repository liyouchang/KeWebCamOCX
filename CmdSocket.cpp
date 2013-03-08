#include "StdAfx.h"
#include "CmdSocket.h"
#include "command.h"

CCmdSocket::CCmdSocket(void):serverPort(TEXT("22616"))
{
	
}

CCmdSocket::~CCmdSocket(void)
{
}

 void CCmdSocket::OnDataReceived( CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr )
 {
	 ASSERT( pSH == m_SocketClient );
	 (pSH);
	 CString strAddr;
	 GetAddress( addr, strAddr );
	
	

 }

 bool CCmdSocket::ConnectToServer( CString severAddr )
 {
	 CString strAddr, strPort;
	 if ( !m_SocketClient.StartClient(NULL, severAddr, strPort, AF_INET,SOCK_STREAM) )
	 {
		 //MessageBox(_T("Failed to start client connection."), NULL, MB_ICONSTOP);
		 return false;
	 }
	return true;
 }

 int CCmdSocket::LoginServer( CString userName,CString pwd )
 {
	m_UserName = userName;
	m_Password = pwd;

	return 0;
 }

 int CCmdSocket::AskKeyt()
 {
	 if (!m_SocketClient.IsOpen())
	 {
		 return KE_SOCKET_NOTOPEN;
	 }
	int msgLen = sizeof(MsgSecretKeyReq);
	msgSend.resize(msgLen,0);

	PMsgSecretKeyReq pReqMsg;
	pReqMsg = (PMsgSecretKeyReq)&msgSend[0];

	pReqMsg->head.protocal = PROTOCOL_HEAD;
	pReqMsg->head.type = ASK_KEY;
	pReqMsg->head.length = msgLen;
	pReqMsg->clientID = 0;

	m_SocketClient.Write(&msgSend[0],msgLen);
		

	WaitForSingleObject(eventKeyt.m_hObject,MSG_WAIT_TIMEOUT);



	return KE_SUCCESS;

 }



 void CCmdSocket::OnThreadExit( CSocketHandle* pSH )
 {
	 ASSERT( pSH == m_SocketClient );
	 (pSH);
 }
