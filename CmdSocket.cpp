#include "StdAfx.h"
#include "CmdSocket.h"
#include "CommonUtility/Md5A.h"
#include "MediaSocket.h"
#include "KeWebCamOCX.h"


CCmdSocket::CCmdSocket(void)
{
	LOG_DEBUG("CCmdSocket::CCmdSocket()" );
	m_clientID = 0;
	m_nSockType = SOCK_TCP;
	m_msgWaitTime = 10000;//10s һ����Ϣ
	m_serverPort = TEXT("22616");
// 	currentPackage = 0;
// 	totalPackageRecv = 0;
	//m_HeartbeatThread = new CHeartBeatThread;
	
}

CCmdSocket::~CCmdSocket(void)
{
	
	//delete m_HeartbeatThrea
	m_xmlInfoThread.Stop();
	m_HeartbeatThread.Stop();
	CloseConnect();
	
	LOG_DEBUG("delete CCmdSocket time end " );
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
	 char caUserName[USERNAME_LEN] = {0};
	 char caPwd[PASSWORD_LEN] = {0};
#ifdef UNICODE
	WideCharToMultiByte(CP_ACP,0,userName,-1,caUserName,USERNAME_LEN,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,pwd,-1,caPwd,PASSWORD_LEN,NULL,NULL);
#else
	strncpy(caUserName,userName.GetString(),min(USERNAME_LEN,userName.GetLength()));
	strncpy(caPwd,pwd.GetString(),min(PASSWORD_LEN,pwd.GetLength()));
#endif

	//������Կ
	char keyt[SECRETKEY_LEN];
	int ret = AskKeyt(keyt);
	if (ret != KE_SUCCESS)
	{
		return ret;
	}
	//���ɼ�������
	char md5Data[ENCRYPTED_DATA_LEN];
	EncryptData(caUserName,caPwd,keyt,md5Data);
	//���͵�½����
	ret = SendLoginMsg(caUserName,md5Data);
	if (ret == KE_SUCCESS)
	{
		//��ʼ����
		m_HeartbeatThread.Initialize(this,m_clientID);
		m_HeartbeatThread.Start();
	}
	AskAllRootNodes(0);

	return ret;
 }

 //************************************
 // Method:    AskKeyt �ӷ�������ȡ��Կ
 // FullName:  CCmdSocket::AskKeyt
 // Access:    protected [out] ������Կ
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

	int ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	ret = WaitRecvMsg(KEMSG_TYPE_ASKKEY);
	if (ret != KE_SUCCESS)
	{
		return KE_MSG_TIMEOUT;
	}
	void * keyPtr = NULL;
	GetRecvMsgData(KEMSG_TYPE_ASKKEY,&keyPtr);
	memcpy(keyt,(char *)keyPtr,SECRETKEY_LEN);
	return KE_SUCCESS;
 }

 bool CCmdSocket::Init()
 {
	 LOG_DEBUG("Init CCmdSocket");
	 const int nRecvBufSize = 0x2000;
	return __super::Init(nRecvBufSize);
 }

 void CCmdSocket::HandleMessage( const BYTE* msgData )
 {
	PKEMsgHead pHead = (PKEMsgHead)msgData;
	switch(pHead->msgType)
	{
	case  KEMSG_TYPE_HEARTBEAT:
		RecvHeartBeat(msgData);
		break;
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
	case  KEMSG_TYPE_PTZControl:
		break;
	case KEMSG_TYPE_AskTreeStruct:
		RecvAskTreeMsg(msgData);
		break;
	case  KEMSG_TYPE_VideoSvrOnline:
		RecvVideoSvrOnline(msgData);
		break;
	default:
		break;
		//LOG_INFO("Receive unkown message: " <<pHead->msgType);
		
	}
 }

 void CCmdSocket::AskKeyMsgResp( const BYTE* msgData )
 {
	 LOG_DEBUG("Receive ask key response");
	PMsgSecretKeyResp pMsg = (PMsgSecretKeyResp)msgData;
	BYTE *secretKey= new BYTE[SECRETKEY_LEN];	//�ӷ�������ȡ����Կ
	memcpy(secretKey,pMsg->keyt,SECRETKEY_LEN);
	SetRecvMsg(pMsg->head.msgType,0,secretKey);
 }

 void CCmdSocket::LoginMsgResp( const BYTE* msgData )
 {
	LOG_DEBUG("Receive login response");
	PKEMsgUserLoginResp pMsg = (PKEMsgUserLoginResp)msgData;
	m_clientID = pMsg->head.clientID;
	SetRecvMsg(pMsg->head.msgType,pMsg->respData);

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
	int ret = Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	ret = WaitRecvMsg(pReqMsg->head.msgType);
	if (ret != KE_SUCCESS)
	{
		return KE_MSG_TIMEOUT;
	}
	int loginResult = GetRecvMsgData(pReqMsg->head.msgType);
	if (loginResult== 0x0d || loginResult == 0x06)
	{
		LOG_INFO("login success");
		TRACE("login success\n");
		return KE_SUCCESS;
	}
	LOG_WARN("login not success ,error number "<< loginResult);
	switch(loginResult)
	{
	case 5:return KE_LOGIN_NAMEPWDERROR;
	case 6:return KE_LOGIN_AlreadyLogin;
	case 1: return KE_LOGIN_DBServerOff;
	}
	return KE_OTHERS;
 }

 void CCmdSocket::EncryptData( const char * userName,const char * pwd,const char * keyt,char * encryptedData )
 {
	char szbuf1[24];
	char szMD5[20];
	 //����MD5
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
 // Parameter: int vedioID		��Ƶ������id
 // Parameter: char channelNo ͨ����
 // Parameter: char reqType 0:����,1:�Ͽ�
 // Parameter: char dataType  0������Ƶ,1������Ƶ,2����Խ�
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
	 pReqMsg->videoID = vedioID;
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
	int videoSvrIp = pMsg->videoIP;
	int port = pMsg->port;
	int online = pMsg->online;
	int channelNo = pMsg->channelNo;
	int cameraID = MakeCameraID(videoID,channelNo);
	if (online >1)
	{
		LOG_INFO("the video server is offline!");	
		theApp.g_pMainWnd->PostMessage(WM_RTVIDEOSTOP,cameraID,KE_RTV_BOTHOFFLINE);
		return;
	}
	if (transIp==0 &&  videoSvrIp == 0)
	{
		LOG_INFO("All the vedio server is offline!");	
		theApp.g_pMainWnd->PostMessage(WM_RTVIDEOSTOP,cameraID,KE_RTV_BOTHOFFLINE);
		return;
	}
	int mediaSvrType;
	
	COneCamera * tmpCamera = theApp.g_PlayWnd->GetOnePlayer(cameraID);
	CMediaSocket * media = tmpCamera->m_MediaSocket;//CMediaSocket::GetMediaSocket(videoID,channelNo,true);
	if (media == NULL)
	{
		media = new CMediaSocket;
		media->Init();
		tmpCamera->m_MediaSocket = media;
	}
	int ret;
	bool connected = false;

	if (videoSvrIp != 0 )//������Ƶ������
	{
		if(!media->ConnectToServer(videoSvrIp,port,1,m_clientID))
		{
			LOG_WARN("Connect video server failed!");
		}
		else
		{
			ret = media->ReqestMediaData(cameraID,Media_Vedio);
			if (ret != KE_SUCCESS)
			{
				theApp.g_pMainWnd->PostMessage(WM_RTVIDEOSTOP,cameraID,ret);
			}
			return;
		}
	}
	if ( transIp != 0)//����ת��������
	{
		if(!media->ConnectToServer(transIp,22615,2,m_clientID))
		{
			LOG_WARN("Connect trans server failed!");
		}
		else
		{
			ret = media->ReqestMediaData(cameraID,Media_Vedio);
			if (ret != KE_SUCCESS)
			{
				theApp.g_pMainWnd->PostMessage(WM_RTVIDEOSTOP,cameraID,ret);
			}
			return;
		}
	}

	theApp.g_pMainWnd->PostMessage(WM_RTVIDEOSTOP,cameraID,KE_CONNECT_SERVER_ERROR);
	
 }

 void CCmdSocket::RecvMalfunctionAlert( const BYTE *msgData )
 {
	PKEMalfunctionAlert  pMsg = (PKEMalfunctionAlert)msgData;
	//LOG_DEBUG("Malfunction alert:"<<pMsg->alertType<<"-"<<pMsg->alertNo<<"; status="<<pMsg->status);
 }

 void CCmdSocket::RecvHeartBeat( const BYTE * msgData )
 {
	PKEMsgHeartBeat pMsg = (PKEMsgHeartBeat)msgData;
	if (pMsg->status == 0x0d )
	{
		m_HeartbeatThread.ResetCount();
	}
 }

 void CCmdSocket::Run()
 {
	 while (m_SocketClient->IsOpen() && !this->toStop)
	 {
		 //TRACE1("LOOP  time %d\n",GetTickCount());
		 if (m_recvBuf.GetCount() == 0 )
		 {
			 DWORD dw = m_recvBuf.WaitForNewData(m_msgWaitTime);
			 if (dw ==WAIT_TIMEOUT )
			 {
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

 void CCmdSocket::CloseConnect()
 {
	 //ȡ�����ȴ�
	m_HeartbeatThread.Stop();
	m_recvBuf.CancelWaitNewData();
	m_SocketClient.Terminate();
	
	
	//��ն�����
	m_recvBuf.Drain( m_recvBuf.GetCount() );
 }

 int CCmdSocket::SendPTZControlMsg( int cameraID,BYTE ctrlType,BYTE speed,BYTE data )
 {
	 if (cameraID <0 || ctrlType > 31 )
	 {
		 return KE_ERROR_PARAM;
	 }
	 if (!m_SocketClient.IsOpen())
	 {
		 return KE_SOCKET_NOTOPEN;
	 }
	 std::vector<BYTE> msgSend;
	 int msgLen = sizeof(KEPTZControlReq);
	 msgSend.resize(msgLen,0);
	 PKEPTZControlReq pReqMsg;
	 pReqMsg = (PKEPTZControlReq)&msgSend[0];
	 pReqMsg->protocal = PROTOCOL_HEAD;
	 pReqMsg->msgType = KEMSG_TYPE_PTZControl;
	 pReqMsg->msgLength = msgLen;
	 pReqMsg->clientID = m_clientID;
	 pReqMsg->channelNo = cameraID & 0xFF;
	 pReqMsg->videoID = cameraID >> 8;
	 pReqMsg->ctrlType = ctrlType;
	 pReqMsg->speed = speed;
	 pReqMsg->data = data;

	 int ret = this->Write(&msgSend[0],msgLen);
	 if (ret != msgLen)
	 {
		 return KE_SOCKET_WRITEERROR;
	 }
	return KE_SUCCESS;
 }

 int CCmdSocket::SendAskTreeMsg( int dataType ,char * data,int dataLen)
 {
	 std::vector<BYTE> msgSend;
	 int msgLen = sizeof(KEAskTreeMsg) + dataLen;
	 msgSend.resize(msgLen,0);
	 PKEAskTreeMsg pReqMsg;
	 pReqMsg = (PKEAskTreeMsg)&msgSend[0];
	 pReqMsg->protocal = PROTOCOL_HEAD;
	 pReqMsg->msgType = KEMSG_TYPE_AskTreeStruct;
	 pReqMsg->msgLength = msgLen;
	 pReqMsg->clientID = m_clientID;
	 pReqMsg->dataType = dataType;
	 pReqMsg->packageNo = 0;
	 pReqMsg->packageTotal = 1;
	 if (data != NULL)
	 {
		memcpy(&msgSend[sizeof(KEAskTreeMsg)],data,dataLen);
	 }
	
	 int ret = this->Write(&msgSend[0],msgLen);
	 if (ret != msgLen)
	 {
		 return KE_SOCKET_WRITEERROR;
	 }
	 return KE_SUCCESS;
 }


 void CCmdSocket::RecvAskTreeMsg( const BYTE * msgData )
 {
	PKEAskTreeMsg pMsg = (PKEAskTreeMsg)msgData;
	switch(pMsg->dataType)
	{
	case KEMSG_ASKTREE_DATATYPE_AllRootNodes:
		{
			char * xmlInfoPtr = (char *)msgData + sizeof(KEAskTreeMsg);
			int xmlInfoLen = pMsg->msgLength - sizeof(KEAskTreeMsg);
			m_xmlInfoThread.AllNotesInfo.insert(m_xmlInfoThread.AllNotesInfo.length(),xmlInfoPtr,xmlInfoLen);

			m_xmlInfoThread.currentPackage = pMsg->packageNo+1;
			int packageTotal = pMsg->packageTotal;
			if (m_xmlInfoThread.currentPackage < packageTotal)
			{
				//LOG_DEBUG("ask another xml package :" << m_xmlInfoThread.currentPackage);
				AskAllRootNodes(m_xmlInfoThread.currentPackage);
			}
			else
			{
				LOG_DEBUG("Receive XML data finished !");
				//::_beginthreadex (0, 0, XMLInfoThreadProc, this, 0,NULL);
				m_xmlInfoThread.Initialize(this,m_clientID);
				m_xmlInfoThread.Start();
			}
		}

	default:
		break;
	}
 }

 int CCmdSocket::AskAllRootNodes( short packageNo )
 {
	 short pNo = packageNo;	
	return SendAskTreeMsg(KEMSG_ASKTREE_DATATYPE_AllRootNodes,(char *)&pNo,2);
 }

 void CCmdSocket::RecvVideoSvrOnline( const BYTE * msgData )
 {
	 //LOG_DEBUG("RecvVideoSvrOnline data receive!\n");
	 PKEMsgHead pMsg = (PKEMsgHead)msgData;
	 int dataLen = pMsg->msgLength - sizeof(KEMsgHead);
	const BYTE * data = msgData + sizeof(KEMsgHead);
	const int EachNodeStatusLen = 7;
	int nodeNum = dataLen/EachNodeStatusLen;
	if (dataLen%EachNodeStatusLen != 0)
	{
		LOG_WARN("RecvVideoSvrOnline data error  1!\n");
		SetRecvMsg(KEMSG_TYPE_VideoSvrOnline,0);
		return;
	}
	if (nodeNum == 1)
	{
		return;
	}
	if (m_xmlInfoThread.videoSvrNodes.size() != nodeNum)
	{
		LOG_WARN("RecvVideoSvrOnline data error  ,nodeNum="<< nodeNum);
		//SetRecvMsg(KEMSG_TYPE_VideoSvrOnline,nodeNum);
		//return;
	}
	
	for(int i=0;i<nodeNum;i++)
	{
		short nodeID = *((short *)data);
		CHNODE * node = &m_xmlInfoThread.videoSvrNodes[i];
		if (node->NodeID != nodeID)
		{
			LOG_WARN("RecvVideoSvrOnline need to find --"<<nodeID);
			int nodeIndex = FindChNodeByID(m_xmlInfoThread.videoSvrNodes,nodeID);
			if (nodeIndex == -1)
			{
				LOG_ERROR("Cannot find the node of id "<< nodeID);
			}
			else
			{
				node = &m_xmlInfoThread.videoSvrNodes[nodeIndex];
			}
		}
		node->onLine = data[2];
		data += EachNodeStatusLen;
	}
	 SetRecvMsg(KEMSG_TYPE_VideoSvrOnline,0);
 }
//������nID��ͬ�Ľڵ�id���������ɹ�������vector�е�λ�ã�ʧ�ܷ���-1
 int CCmdSocket::FindChNodeByID(const std::vector<CHNODE> & nodes,int nID )
 {
	 //std::vector<CHNODE>::const_iterator nodeIter = nodes.begin();
	for (UINT i=0;i<nodes.size();i++)
	{
		if (nodes[i].NodeID == nID)
		{
			return i;
		}
	}
	return -1;
 }
