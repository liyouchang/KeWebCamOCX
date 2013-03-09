#include "StdAfx.h"
#include "CmdSocket.h"
#include "CommonUtility/Md5A.h"

CCmdSocket::CCmdSocket(void):serverPort(TEXT("22616"))
{
	m_clientID = 0;
}

CCmdSocket::~CCmdSocket(void)
{
}


 bool CCmdSocket::ConnectToServer( CString severAddr )
 {
	 
	LOG_DEBUG("Connect to server, serverAddr="<<severAddr.GetString()<<";serverPort="<<serverPort.GetString());
	 if ( !m_SocketClient.StartClient(NULL, severAddr, serverPort, AF_INET,SOCK_STREAM) )
	 {
		 int errorCode = WSAGetLastError();
		LOG_ERROR("Failed to start client connection." << errorCode);
		 return false;
	 }
	return true;
 }

 int CCmdSocket::LoginServer( CString userName,CString pwd )
 {
	char caUserName[USERNAME_LEN];
	char caPwd[PASSWORD_LEN];
	
	WideCharToMultiByte(CP_ACP,0,userName,-1,caUserName,USERNAME_LEN,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,pwd,-1,caPwd,PASSWORD_LEN,NULL,NULL);

	//请求密钥
	char keyt[SECRETKEY_LEN];
	int ret = AskKeyt(keyt);
	if (ret != KE_SUCCESS)
	{
		return ret;
	}
	//生成加密数据
	char md5Data[ENCRYPTED_DATA_LEN];
	EncryptData(caUserName,caPwd,keyt,md5Data);
	//发送登陆请求
	
	return SendLoginMsg(caUserName,md5Data);
 }

 //************************************
 // Method:    AskKeyt 从服务器获取密钥
 // FullName:  CCmdSocket::AskKeyt
 // Access:    protected [out] 返回密钥
 // Returns:   int
 // Qualifier:
 // Parameter: BYTE * secretData
 //************************************
 int CCmdSocket::AskKeyt(char * keyt)
 {
	 if (!m_SocketClient.IsOpen())
	 {
		 return KE_SOCKET_NOTOPEN;
	 }
	 std::vector<BYTE> msgSend;
	int msgLen = sizeof(MsgSecretKeyReq);
	msgSend.resize(msgLen,0);

	PMsgSecretKeyReq pReqMsg;
	pReqMsg = (PMsgSecretKeyReq)&msgSend[0];

	pReqMsg->head.protocal = PROTOCOL_HEAD;
	pReqMsg->head.type = KEMSG_TYPE_ASKKEY;
	pReqMsg->head.length = msgLen;
	pReqMsg->clientID = 0;

	m_SocketClient.Write(&msgSend[0],msgLen);
		
	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_ASKKEY].m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		LOG_ERROR("Wait secret key response time out");
		return KE_MSG_TIMEOUT;
	}

	memcpy(keyt,tmpData[KEMSG_EVENT_ASKKEY],SECRETKEY_LEN);

	delete tmpData[KEMSG_EVENT_ASKKEY];

	return KE_SUCCESS;
 }

 bool CCmdSocket::Init()
 {
	return __super::Init(RECV_BUF_SIZE);
 }

 void CCmdSocket::HandleMessage( const BYTE* msgData )
 {
	PKEMsgHead pHead = (PKEMsgHead)msgData;

	switch(pHead->type)
	{
	case KEMSG_TYPE_ASKKEY:
		AskKeyMsgResp(msgData);
		break;
	case KEMSG_TYPE_LOGIN:
		LoginMsgResp(msgData);
		break;
	default:
		LOG_INFO("Receive unkown message");
		
	}
 }

 void CCmdSocket::AskKeyMsgResp( const BYTE* msgData )
 {
	 LOG_DEBUG("Receive ask key response");
	PMsgSecretKeyResp pMsg = (PMsgSecretKeyResp)msgData;
	BYTE *secretKey= new BYTE[SECRETKEY_LEN];	//从服务器获取的密钥
	memcpy(secretKey,pMsg->keyt,SECRETKEY_LEN);
	tmpData[KEMSG_EVENT_ASKKEY] = secretKey;
	keEvent[KEMSG_EVENT_ASKKEY].PulseEvent();
 }

 void CCmdSocket::LoginMsgResp( const BYTE* msgData )
 {

	 LOG_DEBUG("Receive login response");
	PKEMsgUserLoginResp pMsg = (PKEMsgUserLoginResp)msgData;
	m_clientID = pMsg->clientID;

	int * loginRet = new int;
	*loginRet = pMsg->respData;
	tmpData[KEMSG_EVENT_LOGIN] = loginRet;

	keEvent[KEMSG_EVENT_LOGIN].PulseEvent();
 }

 int CCmdSocket::SendLoginMsg( const char * userName,const char *encryptData )
 {
	 if (!m_SocketClient.IsOpen())
	 {
		 return KE_SOCKET_NOTOPEN;
	 }
	
	 std::vector<BYTE> msgSend;
	int msgLen = sizeof(KEMsgUserLoginReq);
	msgSend.resize(msgLen,0);

	PKEMsgUserLoginReq pReqMsg;
	pReqMsg = (PKEMsgUserLoginReq)&msgSend[0];

	pReqMsg->head.protocal = PROTOCOL_HEAD;
	pReqMsg->head.type = KEMSG_TYPE_LOGIN;
	pReqMsg->head.length = msgLen;
	//strcpy(pReqMsg->userName,userName);
	memcpy(pReqMsg->userName,userName,min(strlen(userName),USERNAME_LEN));
	memcpy(pReqMsg->encreptData,encryptData,ENCRYPTED_DATA_LEN);
	LOG_DEBUG("send login message");
	m_SocketClient.Write(&msgSend[0],msgLen);
		
	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_LOGIN].m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		LOG_ERROR("Wait login response time out");
		return KE_MSG_TIMEOUT;
	}

	int loginResult = *(int *)tmpData[KEMSG_EVENT_LOGIN];
	delete tmpData[KEMSG_EVENT_LOGIN];
	//msgSend.clear();
	if (loginResult== 0x0d || loginResult == 0x06)
	{
		LOG_INFO("login success");
		return KE_SUCCESS;
	}
	if (loginResult == 0x05)
	{
		LOG_WARN("login username or password error!");
		return KE_LOGIN_NAMEPWDERROR;
	}
LOG_WARN("login notify unkown!");
	return KE_LOGIN_OTHERERROR;
 }

 void CCmdSocket::EncryptData( const char * userName,const char * pwd,const char * keyt,char * encryptedData )
 {
	char szbuf1[24];
	char szMD5[20];
	 //计算MD5
	 memset(szbuf1, 0, sizeof(szbuf1));
	 memcpy(szbuf1,userName, min(strlen(userName),USERNAME_LEN));
	 memcpy(szbuf1 + 8, keyt, min(strlen(keyt),SECRETKEY_LEN));
	 memcpy(szbuf1 + 16, pwd, min(strlen(pwd),PASSWORD_LEN));
	 memset(szMD5, 0, sizeof(szMD5));

	 Md5.MDString(szbuf1, szMD5);

	 memcpy(encryptedData,szMD5,ENCRYPTED_DATA_LEN);
 }


