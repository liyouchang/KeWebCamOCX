#include "StdAfx.h"
#include "CmdSocket.h"
#include "CommonUtility/Md5A.h"
#include "MediaSocket.h"
#include "KeWebCamOCX.h"


CCmdSocket::CCmdSocket(void)
{
	m_clientID = 0;
	m_nSockType = SOCK_TCP;
	m_msgWaitTime = 10000;//10s 一个消息
	m_serverPort = TEXT("22616");
	currentPackage = 0;
	totalPackageRecv = 0;
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
			totalPackageRecv = pMsg->packageTotal;
			if (currentPackage < totalPackageRecv)
			{
				LOG_DEBUG("ask another xml package :" << currentPackage);
				AskAllRootNodes(currentPackage);
			}
			else
			{
				LOG_DEBUG("Receive XML data finished !");
				::_beginthreadex (0, 0, XMLInfoThreadProc, this, 0,NULL);
				
			}
		}

	default:
		break;
	}
 }

 int CCmdSocket::AskAllRootNodes( short packageNo )
 {
// 	 if (currentPackage +1 < totalPackageRecv)
// 	 {
// 		 LOG_WARN("Device XML info receiving! currentPackege-"<<currentPackage<<"; totalPackage-"<<totalPackageRecv);
// 		 return KE_XML_Receiving;
// 	 }
// 	 AllNotesInfo.clear();
	 short pNo = packageNo;

	return SendAskTreeMsg(KEMSG_ASKTREE_DATATYPE_AllRootNodes,(char *)&pNo,2);
 }

 void CCmdSocket::DoXmlToMap( CMarkup &xml )
 {
	 while(xml.FindElem(_T("Node")))
	 {
		 CHNODE changeNode;
		 changeNode.NodeType = _ttoi(xml.GetAttrib(_T("NodeType")));
		 changeNode.NodeName = xml.GetAttrib(_T("Name"));
		 changeNode.ParentNodeID =  _ttoi(xml.GetAttrib(_T("ParentID")));
		 changeNode.NodeID = _ttoi(xml.GetAttrib(_T("ID")));
		 switch(changeNode.NodeType)
		 {
		 case 0:  //工程
			 changeNode.ParentNodeID = 0;
			 upperNodes.push_back(changeNode);
			 break;
		 case 1:  //组
			 upperNodes.push_back(changeNode);

			 break;
		 case 2:  //有线视频服务器
			 changeNode.NodeID = _ttoi(xml.GetAttrib(_T("InforID")));
			 videoSvrNodes.push_back(changeNode);
			 break;
		 case 3:  //通道
			 changeNode.NodeID =changeNode.ParentNodeID*256 +changeNode.NodeID ;
			 channelNodes.push_back(changeNode);
			 break;
		 default:
			 break;
		 }

		 if (videoSvrNodes.size() >1)
		 {
			SendVideoSvrOnline();
			Sleep(50);
		 }

		 if (xml.IntoElem())
		 {	
			 DoXmlToMap(xml);
			 xml.OutOfElem();
		 }
	 }//while
 }

 int CCmdSocket::SendVideoSvrOnline()
 {
	 if (videoSvrNodes.size() == 0)
	 {
		 return KE_SUCCESS;
	 }
	 std::vector<BYTE> msgSend;
	 int msgLen = sizeof(KEMsgHead) + videoSvrNodes.size()*2;
	 msgSend.resize(msgLen,0);
	 PKEMsgHead pReqMsg;
	 pReqMsg = (PKEMsgHead)&msgSend[0];
	 pReqMsg->protocal = PROTOCOL_HEAD;
	 pReqMsg->msgType = KEMSG_TYPE_VideoSvrOnline;
	 pReqMsg->msgLength = msgLen;
	 pReqMsg->clientID = m_clientID;

	 for (int i= 0;i<videoSvrNodes.size();i++)
	 {
		 short svrID = videoSvrNodes[i].NodeID;
		 memcpy(&msgSend[sizeof(KEMsgHead)+i*2],(char *)&svrID,2);
	 }
	 
	 keEvent[KEMSG_EVENT_VideoSvrOnline].ResetEvent();

	 int ret = this->Write(&msgSend[0],msgLen);
	 if (ret != msgLen)
	 {
		 return KE_SOCKET_WRITEERROR;
	 }
	 DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_VideoSvrOnline].m_hObject,MSG_WAIT_TIMEOUT);
	 if (dw == WAIT_TIMEOUT)
	 {
		 LOG_ERROR("Wait secret key response time out");
		 return KE_MSG_TIMEOUT;
	 }

	 theApp.g_pMainWnd->SendMessage(WM_TREESTRUCTNOTIFY,
		 KEMSG_ASKTREE_DATATYPE_AllRootNodes,(LPARAM)(&upperNodes));		
	 theApp.g_pMainWnd->SendMessage(WM_TREESTRUCTNOTIFY,
		 KEMSG_ASKTREE_DATATYPE_AllRootNodes,(LPARAM)(&videoSvrNodes));		
	 theApp.g_pMainWnd->SendMessage(WM_TREESTRUCTNOTIFY,
		 KEMSG_ASKTREE_DATATYPE_AllRootNodes,(LPARAM)(&channelNodes));	
	 upperNodes.clear();
	 videoSvrNodes.clear();
	 channelNodes.clear();

	 return KE_SUCCESS;
 }

 unsigned int __stdcall CCmdSocket::XMLInfoThreadProc( void* arg )
 {
	 LOG_DEBUG("start XMLInfoThreadProc thread");
	 CCmdSocket * ptr = (CCmdSocket *)arg;
	 CMarkup g_xml;
	 tstd::tstring xmlStr = str_to_tstr(ptr->AllNotesInfo);
	 ptr->AllNotesInfo.clear();
	 g_xml.SetDoc(xmlStr.c_str());
	 if (g_xml.IsWellFormed())
	 {
		 //log("xml is well");
		 if (g_xml.FindElem(_T("AllRootNodes")))
		 {
			 g_xml.IntoElem();
 			 ptr->upperNodes.clear();
 			 ptr->videoSvrNodes.clear();
 			 ptr->channelNodes.clear();
			 ptr->DoXmlToMap(g_xml);
			 if (ptr->SendVideoSvrOnline() != KE_SUCCESS)
			 {
				
				 return 0;
			 }
			
		 }
	 }
	 else
	 {
		 LOG_INFO("*******************error xml not formed**********************");
	 } 
	 return 1;
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
		keEvent[KEMSG_EVENT_VideoSvrOnline].SetEvent();
		return;
	}
	if (videoSvrNodes.size() != nodeNum)
	{
		//LOG_WARN("RecvVideoSvrOnline data error  2!\n");
		keEvent[KEMSG_EVENT_VideoSvrOnline].SetEvent();
		return;
	}
	for(int i=0;i<nodeNum;i++)
	{
		short nodeID = *((short *)data);
		CHNODE * node = &videoSvrNodes[i];
		if (node->NodeID != nodeID)
		{
			LOG_WARN("RecvVideoSvrOnline need to find ?\n");
			int NodeIndex = FindChNodeByID(videoSvrNodes,nodeID);
			if (NodeIndex == -1)
			{
				LOG_WARN("RecvVideoSvrOnline   find  error ?\n");
				continue;
			}
			node = &videoSvrNodes[NodeIndex];
		}
		node->onLine = data[2];
		data += EachNodeStatusLen;
	}

	LOG_DEBUG("RecvVideoSvrOnline\n");
	 keEvent[KEMSG_EVENT_VideoSvrOnline].SetEvent();
 }
//返回与nID相同的节点id的索引。成功返回在vector中的位置，失败返回-1
 int CCmdSocket::FindChNodeByID(const std::vector<CHNODE> & nodes,int nID )
 {
	 //std::vector<CHNODE>::const_iterator nodeIter = nodes.begin();
	for (int i=0;i<nodes.size();i++)
	{
		if (nodes[i].NodeID == nID)
		{
			return i;
		}
	}
	return -1;
 }
