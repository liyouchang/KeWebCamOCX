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
	m_msgWaitTime = 10000;//10s 一个消息
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
	if (ret == KE_SUCCESS)
	{
		//开始心跳
		m_HeartbeatThread.Initialize(this,m_clientID);
		m_HeartbeatThread.Start();
	}
	AskAllRootNodes(0);

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
	case KEMSG_08_REALTIMEDATA:
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
	case KEMSG_08_HistoryData:
		RecvHistoryData(msgData);
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
	BYTE *secretKey= new BYTE[SECRETKEY_LEN];	//从服务器获取的密钥
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
	 //计算MD5
	 memset(szbuf1, 0, sizeof(szbuf1));
	 memcpy(szbuf1,userName, min(strlen(userName),USERNAME_LEN));
	 memcpy(szbuf1 + 8, keyt, min(strlen(keyt),SECRETKEY_LEN));
	 memcpy(szbuf1 + 16, pwd, min(strlen(pwd),PASSWORD_LEN));
	 memset(szMD5, 0, sizeof(szMD5));
	 Md5.MDString(szbuf1, szMD5);
	 memcpy(encryptedData,szMD5,ENCRYPTED_DATA_LEN);
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
	 pReqMsg->msgType = KEMSG_08_REALTIMEDATA;
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
	if (online >2)
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

	CMediaSocket * media = CMediaSocket::GetVideoSvrMedia(videoID);
	int ret;
	bool connected = false;

	if (videoSvrIp != 0 )//连接视频服务器
	{
		if(!media->ConnectToServer(videoSvrIp,port,1,m_clientID))
		{
			LOG_WARN("Connect video server failed!");
		}
		else
		{
			ret = media->ReqestMediaData(cameraID,CMediaSocket::Media_Vedio);
			if (ret != KE_SUCCESS)
			{
				theApp.g_pMainWnd->PostMessage(WM_RTVIDEOSTOP,cameraID,ret);
			}
			return;
		}
	}
	if ( transIp != 0)//连接转发服务器
	{
		if(!media->ConnectToServer(transIp,22615,2,m_clientID))
		{
			LOG_WARN("Connect trans server failed!");
		}
		else
		{
			ret = media->ReqestMediaData(cameraID,CMediaSocket::Media_Vedio);
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
	 //取消读等待
	m_HeartbeatThread.Stop();
	m_recvBuf.CancelWaitNewData();
	m_SocketClient.Terminate();
	
	
	//清空读缓冲
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
//返回与nID相同的节点id的索引。成功返回在vector中的位置，失败返回-1
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

 int CCmdSocket::ConnectServer( CString svrIp,int svrPort )
 {
	this->m_serverPort.Format(_T("%d"),svrPort);
	if (this->ConnectToServer(svrIp))
	{
		return KE_SUCCESS;
	}
	else
	{
		return KE_CONNECT_SERVER_ERROR;
	}

 }

int CCmdSocket::StartView( int cameraID )
{
	int vid = cameraID / 256;
	int cid = cameraID % 256;

	return SendRealTimeDataMsg(vid,cid,0,0);
}

int CCmdSocket::StopView( int cameraID )
{
	CMediaSocket *media = GetMediaSocket(cameraID);	
	media->CloseConnect();
	return KE_SUCCESS;
}

int CCmdSocket::PTZControl( int cameraID, BYTE ctrlType ,BYTE speed ,BYTE data )
{
	return SendPTZControlMsg(cameraID,ctrlType,speed,data);
}



int CCmdSocket::RequestHistoryData( int cameraID,int startTime,int endTime,int fileType,int targetType/*= 2*/ )
{
	if (!m_SocketClient.IsOpen())
	{
		return KE_SOCKET_NOTOPEN;
	}
	int devID = cameraID/256;
	int channelNo = cameraID%256;
	std::vector<BYTE> msgSend;
	int msgLen = sizeof(KEHistoryDataReq);
	msgSend.resize(msgLen,0);
	KEHistoryDataReq* pReqMsg;
	pReqMsg = (KEHistoryDataReq *)&msgSend[0];
	pReqMsg->protocal = PROTOCOL_HEAD;
	pReqMsg->msgType = KEMSG_08_HistoryData;
	pReqMsg->msgLength = msgLen;
	pReqMsg->clientID = m_clientID;
	pReqMsg->channelNo = channelNo;
	pReqMsg->videoID = devID;
	CTime st(startTime);
	CTime et(endTime);
	pReqMsg->startTime[0] = st.GetYear()%2000;
	pReqMsg->startTime[1] = st.GetMonth();
	pReqMsg->startTime[2] = st.GetDay();
	pReqMsg->startTime[3] = st.GetHour();
	pReqMsg->startTime[4] = st.GetMinute();
	pReqMsg->startTime[5] = st.GetSecond();
	pReqMsg->endTime[0] = et.GetYear()%2000;
	pReqMsg->endTime[1] = et.GetMonth();
	pReqMsg->endTime[2] = et.GetDay();
	pReqMsg->endTime[3] = et.GetHour();
	pReqMsg->endTime[4] = et.GetMinute();
	pReqMsg->endTime[5] = et.GetSecond();

	pReqMsg->fileType = fileType;
	pReqMsg->alarmNo = 0;
	pReqMsg->targetType = targetType;

	int ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}

	ret = this->WaitRecvMsg(KEMSG_08_HistoryData,5000);
	return ret;
}

void CCmdSocket::RecvHistoryData( const BYTE * msgData )
{	
	KEHistoryDataResp * pMsg = (KEHistoryDataResp *)msgData;
	int cameraID = (pMsg->videoID<<8)+(pMsg->channelNo&0x7F);
	CMediaSocket *media = GetMediaSocket(cameraID);
	static bool startRecv = false;
	if (!startRecv )//连接媒体服务器
	{
		this->mediaTransIp = pMsg->transIp;
		this->mediaSvrIp = pMsg->vsIp;
		this->mediaOnline = pMsg->online;
		if (pMsg->target == 2)//
		{
		}
		else if (pMsg->target == 3)
		{
			LOG_ERROR("Donot support target 3 "<< pMsg->target);
			this->SetRecvMsg(KEMSG_08_HistoryData,KE_FUNCTION_NOTSUPPORT);
			return;
		}
		else
		{
			LOG_ERROR("Receive HistoryData target type error which is   "<< pMsg->target);
			this->SetRecvMsg(KEMSG_08_HistoryData,KE_FAILED);
			return;
		}
	}//end if
	
	if (pMsg->msgLength == sizeof(KEHistoryDataResp)) //如果只收到消息头，表示文件内容结束
	{
		this->SetRecvMsg(KEMSG_08_HistoryData,KE_SUCCESS);
		startRecv = false;
		return;
	}

	startRecv = true;

	int fileInfoNum = (pMsg->msgLength-sizeof(KEHistoryDataResp))/100;
	PKERecordFileInfo pRecordFileInfo = (PKERecordFileInfo) (msgData+sizeof(KEHistoryDataResp));

	for (int i=0;i<fileInfoNum;i++,pRecordFileInfo++)
	{
		RecordFileInfo fileInfo;
		fileInfo.fileNo = pRecordFileInfo->fileNo;
		CTime st(pRecordFileInfo->startTime[0]+2000,pRecordFileInfo->startTime[1],pRecordFileInfo->startTime[2],
			pRecordFileInfo->startTime[3],pRecordFileInfo->startTime[4],pRecordFileInfo->startTime[5]);
		CTime et(pRecordFileInfo->endTime[0]+2000,pRecordFileInfo->endTime[1],pRecordFileInfo->endTime[2],
			pRecordFileInfo->endTime[3],pRecordFileInfo->endTime[4],pRecordFileInfo->endTime[5]);

		fileInfo.startTime = st.GetTime();
		fileInfo.endTime = et.GetTime();
		fileInfo.fileSize = pRecordFileInfo->fileSize;
		memcpy(fileInfo.fileData,pRecordFileInfo->data,80);
		media->recordFileList.push_back(fileInfo);
	}
	if (pMsg->resp == 6)
	{
		this->SetRecvMsg(KEMSG_08_HistoryData,KE_SUCCESS);
		startRecv = false;
	}
}

int CCmdSocket::ConnectToMedia( int cameraID )
{

	CMediaSocket *media = GetMediaSocket(cameraID);
	if (mediaOnline >2 || (mediaSvrIp==0 &&  mediaTransIp == 0))
	{
		LOG_ERROR("the video server is offline!");	
		return KE_FAILED;
	}
	if (mediaSvrIp != 0 )//连接视频服务器
	{
		if(media->ConnectToServer(mediaSvrIp,22616,1,m_clientID))
		{
			return KE_SUCCESS;	
		}
	}
	if ( mediaTransIp != 0)//连接转发服务器
	{
		if(!media->ConnectToServer(mediaTransIp,22615,2,m_clientID))
		{
			return KE_SUCCESS;
		}
	}
	LOG_ERROR("connect media server error!");	
	return KE_CONNECT_SERVER_ERROR;
}

int CCmdSocket::GetRecordFileList( int cameraID,int startTime,int endTime,int fileType,vector<RecordFileInfo> & fileInfoList )
{
	CMediaSocket *media = GetMediaSocket(cameraID);
	media->recordFileList.clear();
	int ret = RequestHistoryData(cameraID,startTime,endTime,fileType,2);
	if (ret == KE_SUCCESS)
	{
		fileInfoList = media->recordFileList;
	}
	return ret;
}

int CCmdSocket::PlayRemoteRecord( int cameraID,int fileNo )
{	
	int devSvrID = cameraID/256;
	int ret = KE_SUCCESS;
	//断开原有连接
	CMediaSocket *media = GetMediaSocket(cameraID);
	if (fileNo == -1)
	{
		media->CloseConnect();
		return KE_SUCCESS;
	}

	ret = ConnectToMedia(cameraID) ;
	if (ret != KE_SUCCESS)
	{
		return ret;
	}
	//media = GetMediaSocket(cameraID);

	ret = media->RemoteRecordPlay(cameraID,fileNo);
	return ret;
}


