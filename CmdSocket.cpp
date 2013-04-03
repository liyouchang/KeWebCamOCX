#include "StdAfx.h"
#include "CmdSocket.h"
#include "CommonUtility/Md5A.h"
#include "MediaSocket.h"
#include "KeWebCamOCX.h"
CCmdSocket::CCmdSocket(void)
{
	m_clientID = 0;
	m_nSockType = SOCK_TCP;
	m_HeartbeatTime = 10000;//10s 一次心跳
	m_serverPort = TEXT("22616");
}

CCmdSocket::~CCmdSocket(void)
{
}


 bool CCmdSocket::ConnectToServer( CString severAddr )
 {
	LOG_DEBUG("Connect to server, serverAddr="<<severAddr.GetString()<<";serverPort="<<m_serverPort.GetString());
	 if ( !m_SocketClient.StartClient(NULL, severAddr, m_serverPort, AF_INET,SOCK_STREAM) )
	 {
		 int errorCode = WSAGetLastError();
		LOG_ERROR("Failed to start client connection." << errorCode);
		 return false;
	 }
	 this->m_serverIP  = severAddr;
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
	ret = SendLoginMsg(caUserName,md5Data);

	//开始心跳
	StartHeartBeat();

	return ret;
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
	pReqMsg->head.msgType = KEMSG_TYPE_ASKKEY;
	pReqMsg->head.msgLength = msgLen;
	pReqMsg->head.clientID = 0;

	m_SocketClient.Write(&msgSend[0],msgLen);
		
	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_ASKKEY].m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		LOG_ERROR("Wait secret key response time out");
		return KE_MSG_TIMEOUT;
	}

	memcpy(keyt,(char *)respData[KEMSG_EVENT_ASKKEY],SECRETKEY_LEN);

	delete (char *)respData[KEMSG_EVENT_ASKKEY];

	return KE_SUCCESS;
 }

 bool CCmdSocket::Init()
 {
	 const int nRecvBufSize = 0x40000;
	return __super::Init(nRecvBufSize);
 }

 void CCmdSocket::HandleMessage( const BYTE* msgData )
 {
	PKEMsgHead pHead = (PKEMsgHead)msgData;

	switch(pHead->msgType)
	{
	case KEMSG_TYPE_ASKKEY:
		AskKeyMsgResp(msgData);
		break;
	case KEMSG_TYPE_LOGIN:
		LoginMsgResp(msgData);
		break;
	case KEMSG_TYPE_REALTIMEDATA:
		RecvRealTimeMedia(msgData);
		break;
	case KEMSG_TYPE_MalfunctionAlert:
		RecvMalfunctionAlert(msgData);
		break;
	default:
		LOG_INFO("Receive unkown message: " <<pHead->msgType);
		
	}
 }

 void CCmdSocket::AskKeyMsgResp( const BYTE* msgData )
 {
	 LOG_DEBUG("Receive ask key response");
	PMsgSecretKeyResp pMsg = (PMsgSecretKeyResp)msgData;
	BYTE *secretKey= new BYTE[SECRETKEY_LEN];	//从服务器获取的密钥
	memcpy(secretKey,pMsg->keyt,SECRETKEY_LEN);
	respData[KEMSG_EVENT_ASKKEY] = (int)secretKey;
	keEvent[KEMSG_EVENT_ASKKEY].PulseEvent();
 }

 void CCmdSocket::LoginMsgResp( const BYTE* msgData )
 {
	LOG_DEBUG("Receive login response");
	PKEMsgUserLoginResp pMsg = (PKEMsgUserLoginResp)msgData;
	m_clientID = pMsg->head.clientID;
	respData[KEMSG_EVENT_LOGIN] = pMsg->respData;
	Sleep(0);
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
	pReqMsg->head.msgType = KEMSG_TYPE_LOGIN;
	pReqMsg->head.msgLength = msgLen;
	//strcpy(pReqMsg->userName,userName);
	memcpy(pReqMsg->userName,userName,min(strlen(userName),USERNAME_LEN));
	memcpy(pReqMsg->encreptData,encryptData,ENCRYPTED_DATA_LEN);
	LOG_DEBUG("send login message");
	Write(&msgSend[0],msgLen);

	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_LOGIN].m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		LOG_ERROR("Wait login response time out");
		return KE_MSG_TIMEOUT;
	}
	int loginResult = respData[KEMSG_EVENT_LOGIN];
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

 int CCmdSocket::StartView( int vedioID,int ChannelID,long PlayHwd )
 {
	
	return 0;
 }

 //************************************
 // Method:    SendRealTimeDataMsg
 // FullName:  CCmdSocket::SendRealTimeDataMsg
 // Access:    public 
 // Returns:   int
 // Qualifier:
 // Parameter: int vedioID		视频服务器id
 // Parameter: char channelNo 通道号
 // Parameter: char reqType 0:请求,1:断开
 // Parameter: char dataType  0请求视频,1请求音频,2请求对讲
 //************************************
 int CCmdSocket::SendRealTimeDataMsg( int vedioID,char channelNo,char reqType,char dataType )
 {
	 if (!m_SocketClient.IsOpen())
	 {
		 return KE_SOCKET_NOTOPEN;
	 }
	 std::vector<BYTE> msgSend;
	 int msgLen = sizeof(KEMsgRequestDataReq);
	 msgSend.resize(msgLen,0);
	 PKEMsgRequestDataReq pReqMsg;
	 pReqMsg = (PKEMsgRequestDataReq)&msgSend[0];
	 pReqMsg->protocal = PROTOCOL_HEAD;
	 pReqMsg->msgType = KEMSG_TYPE_REALTIMEDATA;
	 pReqMsg->msgLength = msgLen;
	 pReqMsg->clientID = m_clientID;
	 pReqMsg->channelNo = channelNo;
	 pReqMsg->vedioID = vedioID;
	 pReqMsg->reqType = reqType;
	 pReqMsg->dataType = dataType;

	 int ret = this->Write(&msgSend[0],msgLen);
	 if (ret != msgLen)
	 {
		 return KE_SOCKET_WRITEERROR;
	 }
	

	 return KE_SUCCESS;
 }

 void CCmdSocket::RecvRealTimeMedia( const BYTE* msgData )
 {
	PKEMsgRealTimeDataResp pMsg = (PKEMsgRealTimeDataResp)msgData;
	int videoID = pMsg->videoID;
	int transIp = pMsg->transIP;
	int vedioIp = pMsg->videoIP;
	int port = pMsg->port;
	int online = pMsg->online;
	int channelNo = pMsg->channelNo;
	if (online >2)
	{
		LOG_INFO("连接转发服务器和视频服务器均不在线!");
		return;
	}
	if (online == 0 || online == 2)//连接转发服务器
	{
		CMediaSocket * media = new CMediaSocket;
		media->Init();

		if(!media->ConnectToServer(transIp,port))
		{
			LOG_ERROR("连接转发服务器失败!");
			return;
		}
		media->ReqestMediaTrans(m_clientID,videoID,channelNo,Media_Vedio);
		theApp.SetMediaSocket(videoID,channelNo,media);
	}
	else
	{
		LOG_WARN("暂不支持从服务器获取!");
	}



 }

 unsigned int __stdcall CCmdSocket::ThreadHeartBeat( void* arg )
 {
	 if ( !arg )
		 return -1;
	 TRACE("Heartbeat start\n");
	 CCmdSocket * ptr = static_cast<CCmdSocket*>(arg);
	 ptr->heartBeatStart = true;
	 ptr->m_heartCount = 0;
	 while(ptr->m_SocketClient.IsOpen() )
	 {
		 if (ptr->m_heartCount > 3)
		 {
			 LOG_ERROR("心跳超时");
			 //GetViewPointer()->PostMessage(WM_HEARTBEATSTOP);
			 break;
		 }
		 KEMsgHeartBeat msg;
		 msg.head.clientID = ptr->m_clientID;
		 msg.head.msgLength = sizeof(KEMsgHeartBeat);
		 msg.head.msgType = KEMSG_TYPE_HEARTBEAT;
		 msg.head.protocal = PROTOCOL_HEAD;
		 msg.status = 0;
		 ptr->Write((BYTE *)&msg,msg.head.msgLength);
		 ptr->m_heartCount++;
		 Sleep(ptr->m_HeartbeatTime);
	 }
	 TRACE("Heartbeat stop\n");
	 ptr->heartBeatStart = false;
	 return 0;
 }

 void CCmdSocket::StartHeartBeat()
 {
	 if (heartBeatStart)
	 {
		 //用户注销后心跳线程可能还没有退出
		 return;
	 }
	 unsigned int heartbeatThreadId;
	 heartbeatThread  = reinterpret_cast<HANDLE>(
		 ::_beginthreadex (0, 0, ThreadHeartBeat, this, 0, &heartbeatThreadId));
	 if (! heartbeatThread)
	 {
		 //AfxMessageBox("心跳线程创建失败");
	 }
 }

 void CCmdSocket::RecvMalfunctionAlert( const BYTE *msgData )
 {
	PKEMalfunctionAlert  pMsg = (PKEMalfunctionAlert)msgData;
	LOG_DEBUG("Malfunction alert:"<<pMsg->alertType<<"-"<<pMsg->alertNo<<"; status="<<pMsg->status);
 }


