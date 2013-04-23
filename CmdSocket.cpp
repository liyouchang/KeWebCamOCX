#include "StdAfx.h"
#include "CmdSocket.h"
#include "CommonUtility/Md5A.h"
#include "MediaSocket.h"
#include "KeWebCamOCX.h"
#include "CommonUtility/Markup.h"

CCmdSocket::CCmdSocket(void)
{
	m_clientID = 0;
	m_nSockType = SOCK_TCP;
	m_msgWaitTime = 10000;//10s 一个消息
	m_serverPort = TEXT("22616");
	currentPackage = 0;
	totalPackage = 0;
	//m_HeartbeatThread = new CHeartBeatThread;
}

CCmdSocket::~CCmdSocket(void)
{
	TRACE1("delete CCmdSocket time start %d\n",GetTickCount());
	//delete m_HeartbeatThread;
	CloseConnect();
	TRACE1("delete CCmdSocket time end %d\n",GetTickCount());
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
	 const int nRecvBufSize = 0x8000;
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

	if (transIp==0 &&  videoSvrIp == 0)
	{
		LOG_INFO("All the vedio server is offline!");
		
		theApp.g_pMainWnd->PostMessage(WM_RTVIDEOSTOP,cameraID,KE_RTV_BOTHOFFLINE);
		return;
	}

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
	if (videoSvrIp != 0 )//连接视频服务器
	{
		if(!media->ConnectToServer(videoSvrIp,port))
		{
			LOG_WARN("Connect video server failed!");
		}
		else
		{
			ret = media->ReqestVideoServer(m_clientID,videoID,channelNo,Media_Vedio);
			if (ret != KE_SUCCESS)
			{
				theApp.g_pMainWnd->PostMessage(WM_RTVIDEOSTOP,cameraID,ret);
			}
			return;
		}
	}
	if ( transIp != 0)//连接转发服务器
	{
		if(!media->ConnectToServer(transIp,22615))
		{
			LOG_WARN("Connect trans server failed!");
		}
		else
		{
			ret = media->ReqestMediaTrans(m_clientID,videoID,channelNo,Media_Vedio);
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
			AllNotesInfo.insert(AllNotesInfo.length(),xmlInfoPtr,xmlInfoLen);

			currentPackage = pMsg->packageNo+1;
			totalPackage = pMsg->packageTotal;
			if (currentPackage < totalPackage)
			{
				AskAllRootNodes(currentPackage);
			}
			else
			{
				LOG_DEBUG("Receive XML data finished !");
				theApp.g_pMainWnd->SendMessage(WM_TREESTRUCTNOTIFY,KEMSG_ASKTREE_DATATYPE_AllRootNodes,(LPARAM)AllNotesInfo.c_str());
			}
		}

	default:
		break;
	}
 }

 int CCmdSocket::AskAllRootNodes( short packageNo )
 {
	 if (currentPackage +1 < totalPackage)
	 {
		 return KE_XML_Receiving;
	 }
	 AllNotesInfo.clear();
	 short pNo = packageNo;

	return SendAskTreeMsg(KEMSG_ASKTREE_DATATYPE_AllRootNodes,(char *)&pNo,2);
 }

// BOOL CCmdSocket::DoXmlToMap( CMarkup &xml, int parentID )
// {
// 	 int id;
// 	 int iVideoID, iPort;
// 	 int lAlarmFlag;
// 	 CHNODE changeNode;
// 	 int iPriCamPID;
// 	 char szPriChannel[2];  //通道是否启用
// 	 char szVdoType[20];
// 	 CString strMac = "";
// 	 CString strHardVer  = "";
// 	 CString strMobileNo = "";
// 	 CString strInstallPlace = "";
// 
// 	 while(xml.FindElem("Node"))
// 	 {
// 		 
// 		  xml.GetAttrib("Name");
// 		 szPriChannel[0] = '1'; //通道已启用
// 		changeNode.bNodeType = atoi(xml.GetAttrib("NodeType"));
// 		changeNode.OperType = 0;
// 		 switch(changeNode.bNodeType)
// 		 {
// 		 case 0:  //工程
// 			 changeNode.NodeID = atoi(xml.GetAttrib("ID"));  //节点ID
// 			 break;
// 		 case 1:  //组
// 			 changeNode.NodeID = atoi(xml.GetAttrib("ID"));  //节点ID
// 			 break;
// 		 case 2:  //有线视频服务器 
// 			 changeNode.HardVer = xml.GetAttrib("HardVer");
// 			 changeNode.strMac = xml.GetAttrib("DeviceID"); 
// 			 iVideoID = atoi(xml.GetAttrib("InforID"));  //视频服务器ID
// 			 iPort = atoi(xml.GetAttrib("Port"));		//视频服务器端口号
// 			changeNode.NodeID = iVideoID;
// 			changeNode.ParentNodeID = atoi(xml.GetAttrib())
// 			 break;
// 		 case 3:  //通道				
// 			 changeNode.bNodeType = 3;
// 			 memcpy(szPriChannel, xml.GetAttrib("IsStart"), 1);
// 			 Data.Kind = nkChannel;
// 			 break;
// 		 case 4:  //输入报警
// 			 changeNode.bNodeType = 4;
// 			 Data.Kind = nkAlert;
// 			 break;
// 		 case 5:  //解码起
// 			 iPort = atoi(xml.GetAttrib("Port"));  //视频服务器端口号
// 			 changeNode.bNodeType = 5;
// 
// 			 Data.Kind = nkDecode;
// 			 iVideoID = atoi(xml.GetAttrib("InforID"));  //解码器ID-解码器也是一个视频服务器
// 			 id = iVideoID*256;
// 			 Data.uVideoID = iVideoID;
// 			 Data.iVdoPort = iPort;
// 			 SetVideoPort(iVideoID,1,iPort);
// 			 break;
// 		 }
// 
// 		 //		if ((Data.Kind == nkGroup) || (Data.Kind == nkRootNode))
// 		 //		{
// 
// 		 char szPriR[2];
// 		 char szPriV[2];
// 		 char szPriB[2];
// 		 char szPriY[2];
// 		 char szPriP[2];
// 		 char szPriPurview[2];			//查看报警日志的权限
// 
// 		 memcpy(szPriP, xml.GetAttrib("Played"), 1);
// 		 memcpy(szPriB, xml.GetAttrib("Replayed"), 1);
// 		 memcpy(szPriV, xml.GetAttrib("VideoPara"), 1);
// 		 memcpy(szPriY, xml.GetAttrib("CloudCtrl"), 1);
// 		 memcpy(szPriR, xml.GetAttrib("LocalKined"), 1);
// 		 memcpy(szPriPurview, xml.GetAttrib("Purview"), 1);
// 
// 		 if (szPriR[0] == '1')
// 			{
// 				Data.PriR = 1;
// 				changeNode.iPriR = 1;
// 			}
// 
// 		 if (szPriV[0] == '1')
// 			{
// 				Data.PriV = 1;
// 				changeNode.iPriV = 1;
// 			}
// 
// 		 if (szPriB[0] == '1')
// 			{
// 				Data.PriB = 1;
// 				changeNode.iPriB = 1;
// 			}
// 
// 		 if (szPriY[0] == '1')
// 			{
// 				Data.PriY = 1;
// 				changeNode.iPriY = 1;
// 			}
// 
// 		 if (szPriP[0] == '1')
// 			{
// 				changeNode.OperType = 3;
// 				Data.PriP = 1;
// 				changeNode.iPriP = 1;
// 			}
// 
// 		 if (szPriPurview[0] == '1')
// 			{
// 				Data.PriCheckEvent = 1;
// 				changeNode.iPriAlertLog = 1;
// 			}
// 
// 		 //}
// 		 if ((Data.Kind == nkChannel))
// 		 {
// 			 if (parentID == 0)
// 			 {
// 				 iPriCamPID = atoi(xml.GetAttrib("ParentID"));  //视频服务器ID
// 				 int lVideoID = iPriCamPID;
// 				 iPriCamPID = iPriCamPID*256;
// 				 Data.ID = iPriCamPID + id;
// 				 m_structvideo[m_iVdoListLen].VideoID = (unsigned short)lVideoID;
// 				 memset(m_structvideo[m_iVdoListLen].szMac, 0, sizeof(m_structvideo[m_iVdoListLen].szMac));
// 				 memset(m_structvideo[m_iVdoListLen].szHardVer, 0, sizeof(m_structvideo[m_iVdoListLen].szHardVer));
// 				 memset(m_structvideo[m_iVdoListLen].szDeviceMobileNo, 0, sizeof(m_structvideo[m_iVdoListLen].szDeviceMobileNo));
// 				 if (strMac != "")
// 				 {					
// 					 memcpy(m_structvideo[m_iVdoListLen].szMac,strMac.GetBuffer(0),strMac.GetLength());
// 				 }
// 				 if (strMobileNo != "")
// 				 {
// 					 memcpy(m_structvideo[m_iVdoListLen].szDeviceMobileNo,strMobileNo.GetBuffer(0),strMobileNo.GetLength());
// 				 }
// 				 else
// 				 {
// 					 changeNode.pDeviceMobileNo = "";
// 				 }
// 				 if (strHardVer != "")
// 				 {
// 					 memcpy(m_structvideo[m_iVdoListLen].szHardVer,strHardVer.GetBuffer(0),strHardVer.GetLength());
// 					 for (int i=1;i<MAX_CHLCOUNT;i++)
// 					 {
// 						 SetHardVer(lVideoID,i,strHardVer.GetBuffer(0));
// 					 }
// 				 }
// 
// 				 m_structvideo[m_iVdoListLen].iOnline = 0;
// 				 m_iVdoListLen++;
// 			 }
// 			 else
// 				 Data.ID = parentID + id;
// 
// 			 Data.ChannelID = id;
// 		 }
// 		 else if (Data.Kind == nkAlert)
// 		 {
// 			 Data.ID = parentID + id + 100;
// 			 Data.ChannelID = id;
// 
// 			 lAlarmFlag = atoi(xml.GetAttrib("AlarmFlag"));  //节点ID
// 
// 			 changeNode.AlarmFlag = lAlarmFlag;
// 			 changeNode.iChlID[0] = atoi(xml.GetAttrib("ChnlID1"));  //节点ID;
// 			 changeNode.iChlID[1] = atoi(xml.GetAttrib("ChnlID2"));  //节点ID
// 			 changeNode.iChlID[2] = atoi(xml.GetAttrib("ChnlID3"));  //节点ID
// 			 changeNode.iChlID[3] = atoi(xml.GetAttrib("ChnlID4"));  //节点ID
// 			 changeNode.iChlID[4] = atoi(xml.GetAttrib("ChnlID5"));  //节点ID
// 			 changeNode.iChlID[5] = atoi(xml.GetAttrib("ChnlID6"));  //节点ID
// 			 changeNode.iChlID[6] = atoi(xml.GetAttrib("ChnlID7"));  //节点ID
// 			 changeNode.iChlID[7] = atoi(xml.GetAttrib("ChnlID8"));  //节点ID
// 			 changeNode.iChlID[8] = atoi(xml.GetAttrib("ChnlID9"));  //节点ID
// 			 changeNode.iChlID[9] = atoi(xml.GetAttrib("ChnlID10"));  //节点ID
// 			 changeNode.iChlID[10] = atoi(xml.GetAttrib("ChnlID11"));  //节点ID
// 			 changeNode.iChlID[11] = atoi(xml.GetAttrib("ChnlID12"));  //节点ID
// 			 changeNode.iChlID[12] = atoi(xml.GetAttrib("ChnlID13"));  //节点ID
// 			 changeNode.iChlID[13] = atoi(xml.GetAttrib("ChnlID14"));  //节点ID
// 			 changeNode.iChlID[14] = atoi(xml.GetAttrib("ChnlID15"));  //节点ID
// 			 changeNode.iChlID[15] = atoi(xml.GetAttrib("ChnlID16"));  //节点ID
// 		 }
// 		 else
// 		 {
// 			 Data.ID = id;
// 
// 		 }
// 
// 		 Data.ParentID = parentID;
// 		 if (Data.Kind == nkChannel)
// 		 {
// 			 if (szPriChannel[0] == '1')
// 			 {
// 				 storetree->Insert(Data);
// 
// 				 //向主窗口发送消息
// 				 changeNode.ParentNodeID = parentID;
// 				 changeNode.NodeID = Data.ID;
// 				 changeNode.iVdoPort = Data.iVdoPort;
// 				 changeNode.NodeName=Data.Caption;
// 				 changeNode.pDeviceMobileNo = Data.szpDeviceMobileNo;
// 				 changeNode.pInstallPlace = Data.szInstallPlace;
// 				 changeNode.NameLen = strlen(changeNode.NodeName);
// 
// 				 //增加通道信息
// 				 memset(&ChannelInfo, 0, sizeof(ChannelInfo));
// 				 ChannelInfo.iChannelID = Data.ID;
// 				 //ChannelInfo.byPopedom = *szPriB;
// 				 memcpy(ChannelInfo.szName, Data.Caption, strlen(Data.Caption));
// 				 SetChlName(Data.ID /256, Data.ID % 256,Data.Caption);
// 				 SetVideoPort(Data.ID /256,1,changeNode.iVdoPort);
// 				 if (MsgHwnd >0)
// 					 SendMessage(MsgHwnd, WM_ADDNODE, (WPARAM)&changeNode, NULL);
// 
// 				 if (m_lCallback != 0)
// 				 {
// 					 PMsgCallback pCallback = (PMsgCallback)m_lCallback;
// 					 //pCallback(TYPE_ADD_NODE, (char *)&changeNode, sizeof(&changeNode));
// 				 }				
// 			 }
// 		 }
// 		 else
// 		 {
// 			 storetree->Insert(Data);
// 
// 			 //向主窗口发送消息
// 			 changeNode.ParentNodeID = parentID;
// 			 changeNode.NodeID = Data.ID;
// 			 changeNode.iVdoPort = Data.iVdoPort;
// 			 changeNode.NodeName=Data.Caption;
// 			 changeNode.NameLen = strlen(changeNode.NodeName);
// 			 changeNode.pDeviceMobileNo = Data.szpDeviceMobileNo;
// 			 changeNode.pInstallPlace = Data.szInstallPlace;
// 			 if (MsgHwnd >0)
// 				 SendMessage(MsgHwnd, WM_ADDNODE, (WPARAM)&changeNode, NULL);
// 
// 			 if (m_lCallback != 0)
// 			 {
// 				 PMsgCallback pCallback = (PMsgCallback)m_lCallback;
// 				 //pCallback(TYPE_ADD_NODE, (char *)&changeNode, sizeof(&changeNode));
// 			 }
// 		 }
// 
// 
// 		 //TRACE("NodeID =%d ,ParentID = %d ,Name = %s,InfoID = %d\n",Data.ID,Data.ParentID,Data.Caption,Data.uVideoID);
// 
// 		 if (((Data.Kind == nkVideoServer) || (Data.Kind == nkDecode))&& (m_iVdoListLen < 3000))
// 		 {
// 			 m_structvideo[m_iVdoListLen].VideoID = (unsigned short)Data.uVideoID;
// 			 memset(m_structvideo[m_iVdoListLen].szMac, 0, sizeof(m_structvideo[m_iVdoListLen].szMac));
// 			 memset(m_structvideo[m_iVdoListLen].szHardVer, 0, sizeof(m_structvideo[m_iVdoListLen].szHardVer));
// 			 memset(m_structvideo[m_iVdoListLen].szDeviceMobileNo, 0, sizeof(m_structvideo[m_iVdoListLen].szDeviceMobileNo));
// 
// 			 if (strMac != "")
// 			 {					
// 				 memcpy(m_structvideo[m_iVdoListLen].szMac,strMac.GetBuffer(0),strMac.GetLength());
// 			 }
// 			 if (strMobileNo != "")
// 			 {
// 				 memcpy(m_structvideo[m_iVdoListLen].szDeviceMobileNo,strMobileNo.GetBuffer(0),strMobileNo.GetLength());
// 			 }
// 			 else
// 			 {
// 				 changeNode.pDeviceMobileNo = "";
// 			 }
// 			 if (strHardVer != "")
// 			 {
// 				 memcpy(m_structvideo[m_iVdoListLen].szHardVer,strHardVer.GetBuffer(0),strHardVer.GetLength());
// 				 for (int i=1;i<MAX_CHLCOUNT;i++)
// 				 {
// 					 SetHardVer(m_structvideo[m_iVdoListLen].VideoID,i,strHardVer.GetBuffer(0));
// 				 }
// 			 }
// 			 m_structvideo[m_iVdoListLen].iOnline = 0;
// 			 m_iVdoListLen++;
// 		 }
// 
// 		 if (xml.IntoElem())
// 		 {	
// 			 DoXmlToMap(xml, Data.ID);
// 			 xml.OutOfElem();
// 		 }
// 
// 	 }//while
// 
// 	 return true;
// }


