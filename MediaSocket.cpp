#include "StdAfx.h"
#include "MediaSocket.h"
#include "KeWebCamOCX.h"
#include "CommonUtility/inifile.h"
CMediaSocket::CMediaSocket(void)
{
	m_AVPlayer = NULL;
	m_videoID = 0;
	m_channelNo = 0;
	this->m_SvrType = 0;
	m_Recorder = NULL;
	m_nSockType = SOCK_TCP;
	Init();
	this->totalFile = 0;
	this->downloaded = 0;
	this->playedFile = 0;
	
}

CMediaSocket::~CMediaSocket(void)
{

	CloseConnect();

	if (m_Recorder != NULL)
	{
		m_Recorder->Stop();
		delete m_Recorder;
		m_Recorder = NULL;
	}
	
}

void CMediaSocket::HandleMessage( const BYTE* msgData )
{
	PKEMsgHead pHead = (PKEMsgHead)msgData;

	switch(pHead->msgType)
	{
	case KEMSG_TYPE_VIDEOSERVER:
		RecvVideoServer(msgData);
		break;
	case KEMSG_TYPE_MEDIATRANS:
		RecvMediaTransResp(msgData);
		break;
	case KEMSG_TYPE_VIDEOSTREAM:
		RecvVideoStream(msgData);
		break;
	case  KEMSG_TYPE_AUDIOSTREAM:
		RecvAudioStream(msgData);
		break;
	case KEMSG_RecordFileList:
		RecvRecordFileList(msgData);
		break;
	case  KEMSG_RecordPlayData:
		RecvRecordPlayData(msgData);
		break;
	case  DevMsg_GetPTZParam:
		RecvGetPTZParam(msgData);
		break;
	case  DevMsg_WifiCheck:
		RecvDevWifiCheck(msgData);
		break;
	case  DevMsg_WifiStart:
		RecvSetDevWifiResp(msgData);
		break;
	case DevMsg_HeartBeat:
		RecvHeartBeat(msgData);
		break;
	default:
		break;
		//LOG_INFO("Receive unkown message: " <<pHead->msgType);

	}
}

bool CMediaSocket::Init()
{
	if (m_Recorder == NULL)
	{
		m_Recorder = new CRecorder();
		m_Recorder->Initialize();
	}
	IniCloudData();
	const int recvBufSize = 128*1024;
	return __super::Init(recvBufSize);
}

bool CMediaSocket::ConnectToServer(CString severAddr,CString serverPort )
{
	LOG_DEBUG("Connect to server, serverAddr="<<severAddr.GetString()<<";serverPort="<<serverPort.GetString());
	if ( !m_SocketClient.StartClient(NULL, severAddr, serverPort, AF_INET,SOCK_STREAM) )
	{
		int errorCode = WSAGetLastError();
		LOG_ERROR("Failed to start client connection." << errorCode);
		return false;
	}
	this->m_serverIP  = severAddr;
	this->m_serverPort = serverPort;
	ptzInfo.isgot = false;
	return true;
}

bool CMediaSocket::ConnectToServer( int serverAddr,int serverPort ,int svrType ,int clientID)
{
	TCHAR pointIP[MAX_PATH];
	if (!CSocketHandle::FormatIP(pointIP,MAX_PATH,serverAddr,false))
	{
		TRACE("ConnectToServer--FormatIP Error!");
		return false;
	}
	this->m_SvrType = svrType;
	this->m_clientID = clientID;
	TCHAR strPort[8];
	_itot(serverPort,strPort,10);
	ptzInfo.isgot = false;
	return ConnectToServer(pointIP,strPort);

}

bool CMediaSocket::ConnectToServer()
{
	return ConnectToServer(this->m_serverIP,this->m_serverPort);
}

int CMediaSocket::ReqestMediaTrans( int videoID,int channelNo,int mediaType )
{
	if (!m_SocketClient.IsOpen())
	{
		return KE_SOCKET_NOTOPEN;
	}
	std::vector<BYTE> msgSend;
	int msgLen = sizeof(KEMsgMediaTransReq);
	msgSend.resize(msgLen,0);

	PKEMsgMediaTransReq pReqMsg;
	pReqMsg = (PKEMsgMediaTransReq)&msgSend[0];
	pReqMsg->head.protocal = PROTOCOL_HEAD;
	pReqMsg->head.msgType = KEMSG_TYPE_MEDIATRANS;
	pReqMsg->head.msgLength = msgLen;
	pReqMsg->head.clientID = m_clientID;
	pReqMsg->channelNo = channelNo;
	pReqMsg->videoID = videoID;
	pReqMsg->video =(mediaType & Media_Vedio)?0: 1;
	pReqMsg->listen = (mediaType & Media_Listen)?0: 1;
	pReqMsg->talk = (mediaType & Media_Talk)?0: 1;
	pReqMsg->devType = theApp.g_cmd->platformType==1?6:3;
	mediaEvent.ResetEvent();
	int ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	DWORD dw = WaitForSingleObject(mediaEvent.m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		LOG_ERROR("Request trans server message timeout!");
		return KE_MSG_TIMEOUT;
	}
	ret = respData;
	switch(ret)
	{
	case  0://请求成功
			return KE_SUCCESS;
	case 1:		return KE_FAILED;
	case 2:		return KE_RTV_DVSOFFLINE;
	case 3:		return KE_RTV_CHANNELDISABLE;
	case 4:		return KE_RTV_MAXTRANSNUM;
	case 8:		return KE_RTV_MAXVIEWNUM;
	case 6:		return KE_RTV_MAXTCPNUM;	
	}
	return KE_OTHERS;
}

void CMediaSocket::RecvMediaTransResp( const BYTE* msgData )
{
	PKEMsgMediaTransResp pMsg = (PKEMsgMediaTransResp)msgData;
	respData = pMsg->respType;
	Sleep(0);
	mediaEvent.SetEvent();

}

void CMediaSocket::RecvVideoStream( const BYTE * msgData )
{
	PKERTStreamHead pMsg = (PKERTStreamHead)msgData;
	//TRACE2("Receive %d Msg Len %d\n",pMsg->msgType,pMsg->msgLength);
	int videoID = pMsg->videoID;
	int channelNo = pMsg->channelNo;
	int cameraID = videoID*256+channelNo;
	//CMyAVPlayer *AVIPlayer = theApp.g_PlayWnd->GetOnePlayer(cameraID)->m_AVIPlayer;
	int ret = this->m_AVPlayer->InputStream(msgData+sizeof(KERTStreamHead),pMsg->msgLength-sizeof(KERTStreamHead));
	if (ret != 0)
	{
		LOG_ERROR(" input stream error: "<<ret);
	}
	if (m_Recorder->isRunning())
	{
		m_Recorder->InputBuf(msgData+sizeof(KERTStreamHead),pMsg->msgLength-sizeof(KERTStreamHead));
	}
}

bool CMediaSocket::CheckMessage( const BYTE* data, DWORD dwCount )
{
	
	return true;
}

void CMediaSocket::Run()
{
	while (m_SocketClient->IsOpen() && !this->toStop)
	{
		//处理消息数据
		if (m_recvBuf.GetCount()==0 )
		{
			Sleep(10);
			continue;
		}
		if (!GetMessageData())
		{
			Sleep(5);
			continue;
		}
		this->HandleMessage(&m_MsgRecv[0]);
		m_MsgRecv.clear();
	}
}

void CMediaSocket::CloseConnect()
{
	m_Recorder->Stop();
	m_SocketClient.Terminate();
	//清空读缓冲
	m_recvBuf.Drain(m_recvBuf.GetCount() );
	this->m_clientID = 0;
	
	this->downloadFileNoList.clear();
	this->PlayFileInfoList.clear();

	this->totalFile = 0;
	this->downloaded = 0;
	this->playedFile =0;
}

int CMediaSocket::ReqestVideoServer( int videoID,int channelNo ,int mediaType)
{
	if (!m_SocketClient.IsOpen())
	{
		return KE_SOCKET_NOTOPEN;
	}
	std::vector<BYTE> msgSend;
	int msgLen = sizeof(KEVideoServerReq);
	msgSend.resize(msgLen,0);
	PKEVideoServerReq pReqMsg;
	pReqMsg = (PKEVideoServerReq)&msgSend[0];
	pReqMsg->protocal = PROTOCOL_HEAD;
	pReqMsg->msgType = KEMSG_TYPE_VIDEOSERVER;
	pReqMsg->msgLength = msgLen;
	pReqMsg->clientID = m_clientID;
	pReqMsg->channelNo = channelNo;
	pReqMsg->videoID = videoID;
	pReqMsg->video =(mediaType & Media_Vedio) ? 0 : 1;
	pReqMsg->listen = (mediaType & Media_Listen) ? 0 : 1;
	pReqMsg->talk = (mediaType & Media_Talk) ? 0 : 1;
	pReqMsg->protocalType = 0;
	pReqMsg->transSvrIp = 0;
	mediaEvent.ResetEvent();
	int ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	DWORD dw = WaitForSingleObject(mediaEvent.m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		LOG_ERROR("Receive KEMSG_TYPE_VIDEOSERVER timeout !");
		return KE_MSG_TIMEOUT;
	}
	ret = respData;
	switch(ret)
	{
	case  0x0d://请求成功
			return KE_SUCCESS;
	default:
		return KE_FAILED;
	}
	return KE_OTHERS;
}

void CMediaSocket::RecvVideoServer( const BYTE *msgData )
{
	PKEVideoServerResp pMsg = (PKEVideoServerResp)msgData;
	respData = pMsg->respType;
	Sleep(0);
	mediaEvent.SetEvent();
}

int CMediaSocket::OpenMedia( int cameraID, int mediaType )
{
	m_AVPlayer = theApp.g_PlayWnd->GetOnePlayer(cameraID)->m_AVIPlayer;
	if ( (mediaType & Media_Vedio) && !m_AVPlayer->IsPlaying())
	{
		int ret = m_AVPlayer->OpenStream();
		if (ret != 0)
		{
			LOG_ERROR(" Open stream error: "<<ret);
			return KE_RTV_OpenStreamFailed;
		}
	}
	if (mediaType & Media_Listen)
	{
		int ret = m_AVPlayer->OpenSound();
		if (ret != 0)
		{
			LOG_ERROR(" Open sound error: "<<ret);
			return KE_RTV_OpenSoundFailed;
		}
	}
	else
	{
		m_AVPlayer->CloseSound();
	}
	return KE_SUCCESS;
}



//************************************
// Method:    ReqestMediaData  ——请求某个镜头播放数据
// FullName:  CMediaSocket::ReqestMediaData
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: int cameraID
// Parameter: int mediaType——播放数据类型，由MediaType结构体定义
//************************************
int CMediaSocket::ReqestMediaData( int cameraID,int mediaType )
{
	if (!IsConnect())
	{
		return KE_SOCKET_NOTOPEN;
	}
	int videoID = cameraID /256;
	int channelNo = cameraID %256;
	m_videoID = videoID;
	m_channelNo = channelNo;
	int ret ;
	if (m_SvrType == 1)
	{
		ret =  ReqestVideoServer(videoID,channelNo,mediaType);
	} 
	if (m_SvrType == 2)
	{
		ret =  ReqestMediaTrans(videoID,channelNo,mediaType);
	}
	if (ret == KE_SUCCESS)
	{
		return OpenMedia(cameraID,mediaType);
	}
	return ret;
}

void CMediaSocket::RecvAudioStream( const BYTE *msgData )
{
	PKERTStreamHead pMsg = (PKERTStreamHead)msgData;
	//TRACE2("Receive %d Msg Len %d\n",pMsg->msgType,pMsg->msgLength);
	int videoID = pMsg->videoID;
	int channelNo = pMsg->channelNo;
	int cameraID = videoID*256+channelNo;
	CMyAVPlayer *AVIPlayer = theApp.g_PlayWnd->GetOnePlayer(cameraID)->m_AVIPlayer;
	int ret = AVIPlayer->InputStream(msgData+sizeof(KERTStreamHead),pMsg->msgLength-sizeof(KERTStreamHead));
	if (ret != 0)
	{
		LOG_ERROR(" input stream error: "<<ret);
	}
	if (m_Recorder->isRunning())
	{
		m_Recorder->InputBuf(msgData+sizeof(KERTStreamHead),pMsg->msgLength-sizeof(KERTStreamHead));
	}
}

int CMediaSocket::StartRecord( const char * fileName )
{
	if (m_Recorder != NULL)
	{
		m_Recorder->SetFileName(fileName);
		m_Recorder->Start();
	}
	return KE_SUCCESS;
}


int CMediaSocket::StartRecord( int cameraID,int fileNo/*=-1*/ )
{
	if (m_Recorder != NULL)
	{
		if (fileNo == -1)
		{
			if (!m_Recorder->MakeRecordFileName(cameraID))
			{
				return KE_FAILED;
			}
		}
		else
		{
			CString fullPath;
			CString fileName ;
			CString filePath;
			CString extName = _T("h264");
			RecordFileInfo recordInfo = recordFileList[fileNo];
			fileName.Format(_T("%d_%d-%d.%s"), cameraID,recordInfo.startTime,recordInfo.endTime,extName);
			CString recordPath = CRecorder::GetRecordFilePath();
			filePath.Format(_T("%s%d\\"),recordPath,cameraID);
			if (!FolderExist(filePath))
			{
				CreateFolderEx(filePath);
			}
			fullPath = filePath + fileName;
			std::string recordFileName = tstd::tstr_to_str(fullPath.GetString());
			m_Recorder->SetFileName(recordFileName.c_str());
			m_Recorder->SetTotalFileSize(recordInfo.fileSize);
		}
		m_Recorder->Start();
	}
	return KE_SUCCESS;
}

int CMediaSocket::StopRecord()
{
	if (m_Recorder != NULL)
	{
		m_Recorder->Stop();
	}
	return KE_SUCCESS;
}

int CMediaSocket::GetRecordFileList( int cameraID,int startTime,int endTime,int fileType,std::vector<RecordFileInfo> & fileInfoList )
{	
	if (!m_SocketClient.IsOpen())
	{
		return KE_SOCKET_NOTOPEN;
	}
	int devID = cameraID/256;
	int channelNo = cameraID%256;
	std::vector<BYTE> msgSend;
	int msgLen = sizeof(KERecordFileListReq);
	msgSend.resize(msgLen,0);
	PKERecordFileListReq pReqMsg;
	pReqMsg = (PKERecordFileListReq)&msgSend[0];
	pReqMsg->protocal = PROTOCOL_HEAD;
	pReqMsg->msgType = KEMSG_RecordFileList;
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

	this->recordFileList.clear();

	int ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}

	ret = this->WaitRecvMsg(KEMSG_RecordFileList);
	
	fileInfoList = this->recordFileList;

	//return ret;
	return KE_SUCCESS;
}

void CMediaSocket::RecvRecordFileList( const BYTE * msgData )
{
	PKERecordFileListResp pMsg = (PKERecordFileListResp)msgData;
	if (pMsg->msgLength == 16)//如果只收到消息头，表示文件内容结束
	{
		this->SetRecvMsg(KEMSG_RecordFileList,KE_SUCCESS);
		return;
	}

	int fileInfoNum = (pMsg->msgLength-sizeof(KERecordFileListResp))/100;
	PKERecordFileInfo pRecordFileInfo = (PKERecordFileInfo) (msgData+sizeof(KERecordFileListResp));
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
		this->recordFileList.push_back(fileInfo);
	}

	if (pMsg->resp == 6)//如果收到应答为6表示文件信息输送结束
	{
			this->SetRecvMsg(KEMSG_RecordFileList,KE_SUCCESS);
	}
	
}

//************************************
// Method:    RemoteRecordPlay 远程录像回放
// FullName:  CMediaSocket::RemoteRecordPlay
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: int cameraID
// Parameter: int fileNo 录像文件编号，用于获得存储在recordFileList中的远程录像文件信息
//************************************
int CMediaSocket::RemoteRecordPlay( int cameraID,int fileNo )
{
	if (fileNo > recordFileList.size())
	{
		return KE_ERROR_PARAM;
	}
 	if (PlayFileInfoList.empty()&& this->downloadFileNoList.empty())
 	{
		this->totalFile =1;
		this->downloaded = 0;
		this->playedFile = 0;
		this->downloadFileNoList.push_back(fileNo);
		//启动录像
		StopRecord();
		StartRecord(cameraID,fileNo);
		//发送消息
		int ret =  RequestRecordPlayData(cameraID,fileNo);
		if (ret != KE_SUCCESS)
		{
			this->downloadFileNoList.clear();
			return ret;
		}
		return ret;
 	}
	this->totalFile++;
	this->downloadFileNoList.push_back(fileNo);

	return KE_SUCCESS;
}

void CMediaSocket::RecvRecordPlayData( const BYTE * msgData )
{
	PKEPlayRecordDataHead pMsg = (PKEPlayRecordDataHead)msgData;
	int iKbytes;
	int cameraID = pMsg->videoID*256 + pMsg->channelNo;
	if (pMsg->resp == 6)//下载完成,如果下载列表中还有内容，则继续下载
	{
		if (m_Recorder->isRunning())
		{
			m_Recorder->InputBuf(msgData+sizeof(KEPlayRecordDataHead),pMsg->msgLength-sizeof(KEPlayRecordDataHead));
		}
		StopRecord();

		//设置播放器文件大小
		if (downloaded == playedFile)
		{
			m_AVPlayer =  theApp.g_cmd->GetAVIPlayer(cameraID);	
			m_AVPlayer->SetFileSize(m_Recorder->GetFileSize());
		}else{
			PlayFileInfo &playInfo =	PlayFileInfoList.back();
			playInfo.fileSize = m_Recorder->GetFileSize();
		}
		//继续下载文件
		this->downloaded++;
		downloadFileNoList.pop_front();
		if (!this->downloadFileNoList.empty())
		{
			int fileNo =downloadFileNoList.front(); 
			StartRecord(cameraID,fileNo);
			RequestRecordPlayData(cameraID,fileNo);
		}
	}
	if (pMsg->resp == 13)
	{
		if (m_Recorder->isRunning())
		{
			m_Recorder->InputBuf(msgData+sizeof(KEPlayRecordDataHead),pMsg->msgLength-sizeof(KEPlayRecordDataHead));
		}
	}
	//达到播放条件后加入播放列表
	if (m_Recorder->FreshToPlay())
	{
		PlayFileInfo playInfo ;
		playInfo.fileName = m_Recorder->GetFileName().c_str();
		playInfo.fileSize = m_Recorder->GetTotalFileSize();
		this->PlayFileInfoList.push_back(playInfo);
		if (PlayFileInfoList.size() == 1)
		{
			m_AVPlayer =  theApp.g_cmd->GetAVIPlayer(cameraID);
			m_AVPlayer->PlayFile(playInfo.fileName.c_str(),playInfo.GetFileSizeKB());	
		}
	}

		
	//int ret = m_AVPlayer->InputStream(msgData+sizeof(PKEPlayRecordDataHead),pMsg->msgLength-sizeof(PKEPlayRecordDataHead));
	//if (ret != 0)
	//{
	//	LOG_ERROR(" input stream error: "<<ret);
	//}
	

}

void CMediaSocket::RecvGetPTZParam( const BYTE * msgData )
{
	int resp = msgData[15];
	this->ptzInfo.protocal = msgData[16];
	this->ptzInfo.addr = msgData[17];
	if (resp == KE_SUCCESS)
	{
		this->ptzInfo.isgot = true;
	}
	SetRecvMsg(DevMsg_GetPTZParam,resp);

}

int CMediaSocket::GetPTZParam( int cameraID )
{
	int devID = cameraID/256;
	int channelNo = cameraID%256;
	std::vector<BYTE> msgSend;
	int msgLen = sizeof(KEDevGetPTZParamReq);
	msgSend.resize(msgLen,0);
	KEDevGetPTZParamReq *  pReqMsg = (KEDevGetPTZParamReq *)&msgSend[0];
	pReqMsg->protocal = PROTOCOL_HEAD;
	pReqMsg->msgType = DevMsg_GetPTZParam;
	pReqMsg->msgLength = msgLen;
	pReqMsg->clientID = m_clientID;
	pReqMsg->videoID = devID;
	pReqMsg->channelNo = channelNo;
	int	ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	ret = WaitRecvMsg(DevMsg_GetPTZParam);
	return ret;
}

int CMediaSocket::PTZControl( int cameraID, BYTE ctrlType ,BYTE speed )
{
	int ret = KE_SUCCESS;
	
	ptzInfo.protocal = 0;
	ptzInfo.addr = 1;
	if (!ptzInfo.isgot)
	{
		ret = GetPTZParam(cameraID);
		if (ret != KE_SUCCESS)		return ret;
		ptzInfo.isgot = true;
	}
	WORD wSpeed = speed;
	if (ctrlType == 2 || ctrlType == 3)
	{
		wSpeed = wSpeed << 8;
	}
	char szBuf[128];
	int iLen = GetCloudProtocol(ptzInfo.protocal, ptzInfo.addr, ctrlType, wSpeed, szBuf );    
	if (iLen < 0)	return KE_FAILED;
	
	int devID = cameraID/256;
	int channelNo = cameraID%256;
	std::vector<BYTE> msgSend;
	int msgLen = sizeof(KEDevGetSerialDataHead) + iLen;
	msgSend.resize(msgLen,0);
	KEDevGetSerialDataHead *  pReqMsg = (KEDevGetSerialDataHead *)&msgSend[0];
	pReqMsg->protocal = PROTOCOL_HEAD;
	pReqMsg->msgType = DevMsg_SerialData;
	pReqMsg->msgLength = msgLen;
	pReqMsg->clientID = m_clientID;
	pReqMsg->videoID = devID;
	pReqMsg->dataLen = iLen;
	memcpy(&msgSend[0]+sizeof(KEDevGetSerialDataHead) ,szBuf,iLen);

	ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	return KE_SUCCESS;

}

int CMediaSocket::GetDevWifiAPList( int cameraID )
{
	if (!m_SocketClient.IsOpen())
	{
		return KE_SOCKET_NOTOPEN;
	}
	if (this->m_SvrType != MediaSvr_DVS)
	{
		return KE_RTV_DVSOFFLINE;
	}
	int devID = cameraID/256;
	int channelNo = cameraID%256;
	std::vector<BYTE> msgSend;
	int msgLen = sizeof(KEDevWifiCheckReq);
	msgSend.resize(msgLen,0);
	KEDevWifiCheckReq* pReqMsg;
	pReqMsg = (KEDevWifiCheckReq*)&msgSend[0];
	pReqMsg->protocal = PROTOCOL_HEAD;
	pReqMsg->msgType = DevMsg_WifiCheck;
	pReqMsg->msgLength = msgLen;
	pReqMsg->clientID = m_clientID;
	pReqMsg->videoID = devID;
	this->APList.clear();
	int ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	ret = this->WaitRecvMsg(DevMsg_WifiCheck,10000);
	return ret;
}

void CMediaSocket::RecvDevWifiCheck( const BYTE * msgData )
{
	KEDevWifiCheckResp * pMsg = (KEDevWifiCheckResp*)msgData;
	if (pMsg->resp != KE_SUCCESS)
	{
		SetRecvMsg(DevMsg_WifiCheck,pMsg->resp);
		return;
	}
	int listNum = (pMsg->msgLength - sizeof(KEDevWifiCheckResp))/sizeof(KEDevAPListItem);
	KEDevAPListItem * pItem	= (KEDevAPListItem *)(msgData+sizeof(KEDevWifiCheckResp));
	for (int i=0;i<listNum;i++)
	{
			KEDevAPListItem aItem = *pItem;
			APList.push_back(aItem);
			pItem++;
	}
	SetRecvMsg(DevMsg_WifiCheck,pMsg->resp);
}

int CMediaSocket::SetDevWifi( int cameraID,int apListNum,KEDevWifiStartReq wifiStart )
{
	if (!IsConnect())
	{
		return KE_SOCKET_NOTOPEN;
	}
	if (apListNum >= APList.size())
	{
		return KE_ERROR_PARAM;
	}
	if (this->m_SvrType != MediaSvr_DVS)
	{
		return KE_RTV_DVSOFFLINE;
	}
	int devID = cameraID/256;
	int channelNo = cameraID%256;
	std::vector<BYTE> msgSend;
	int msgLen = sizeof(KEDevWifiStartReq);
	msgSend.resize(msgLen,0);
	KEDevWifiStartReq* pReqMsg;
	pReqMsg = (KEDevWifiStartReq*)&msgSend[0];
	pReqMsg->protocal = PROTOCOL_HEAD;
	pReqMsg->msgType = DevMsg_WifiStart;
	pReqMsg->msgLength = msgLen;
	pReqMsg->clientID = m_clientID;
	pReqMsg->videoID = devID;

	pReqMsg->APItem = APList[apListNum];
	pReqMsg->APItem.wifiStart = 1;
	//wifiStart parameter
	memcpy(pReqMsg->APItem.password, wifiStart.APItem.password,32);
	pReqMsg->pppoeUse = wifiStart.pppoeUse;
	memcpy(pReqMsg->pppoeAccount,wifiStart.pppoeAccount,30);
	memcpy(pReqMsg->pppoePWD,wifiStart.pppoePWD,30);
	
	int ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	ret = this->WaitRecvMsg(DevMsg_WifiStart);
	return ret;
}

void CMediaSocket::RecvSetDevWifiResp( const BYTE * msgData )
{
	int resp = msgData[14];
	SetRecvMsg(DevMsg_WifiStart,resp);

}

bool CMediaSocket::GetMessageData()
{
	int nRead= 0;
	int headLen = sizeof(KEMsgHead);
	if (m_MsgRecv.size() == 0)//上一个消息已经读取完成
	{
		KEDevMsgHead head;
		nRead = m_recvBuf.Read( (BYTE*)&head , headLen );
		if (nRead != headLen)
		{
			return false;
		}
		if (head.protocal != PROTOCOL_HEAD|| head.msgLength>8192)
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

void CMediaSocket::OnThreadExit( CSocketHandle* pSH )
{
	__super::OnThreadExit(pSH);
}

void CMediaSocket::OnConnectionDropped( CSocketHandle* pSH )
{
	int cameraID = m_videoID*256+m_clientID;
	if (theApp.g_pMainWnd->GetSafeHwnd() != NULL)
	{
		theApp.g_pMainWnd->PostMessage(WM_RTVIDEOSTOP,cameraID,KE_SOCKET_ConnectionDropped);
	}
	__super::OnConnectionDropped(pSH);
}

void CMediaSocket::OnConnectionError( CSocketHandle* pSH, DWORD dwError )
{
	int cameraID = m_videoID*256+m_clientID;
	if (theApp.g_pMainWnd->GetSafeHwnd() != NULL)
	{
		//theApp.g_pMainWnd->PostMessage(WM_RTVIDEOSTOP,cameraID,KE_SOCKET_ConnectionError);
	}
	__super::OnConnectionDropped(pSH);
}

int CMediaSocket::CheckHeartBeat( int devID )
{
	if (!IsConnect())
	{
		return KE_SOCKET_NOTOPEN;
	}
	if (this->m_SvrType != MediaSvr_DVS)
	{
		return KE_RTV_DVSOFFLINE;
	}
	this->m_videoID = devID;
	std::vector<BYTE> msgSend;
	int msgLen = sizeof(KEDevMsgHead);
	msgSend.resize(msgLen,0);
	KEDevMsgHead* pReqMsg;
	pReqMsg = (KEDevMsgHead*)&msgSend[0];
	pReqMsg->protocal = PROTOCOL_HEAD;
	pReqMsg->msgType = DevMsg_HeartBeat;
	pReqMsg->msgLength = msgLen;
	pReqMsg->videoID = devID;

	int ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		this->CloseConnect();
		return KE_SOCKET_WRITEERROR;
	}
	ret = this->WaitRecvMsg(DevMsg_HeartBeat);
	if (ret != KE_SUCCESS)
	{
		this->CloseConnect();
	}
	return ret;
}

void CMediaSocket::RecvHeartBeat( const BYTE * msgData )
{
	SetRecvMsg(DevMsg_HeartBeat,KE_SUCCESS);
}

CMediaSocket * CMediaSocket::GetVideoSvrMedia( int videoSvrID )
{
	std::map<int ,CMediaSocket*>::const_iterator rcIter;
	rcIter = videoSvrMediaMap.find(videoSvrID);
	if ( rcIter == videoSvrMediaMap.end() || rcIter->second == NULL)
	{
		CMediaSocket * pMS = new CMediaSocket;
		//pMS->Init();
		videoSvrMediaMap[videoSvrID] = pMS;
	}

	return videoSvrMediaMap[videoSvrID];

}

void CMediaSocket::DelVideoSvrMedia( int videoSvrID )
{
	std::map<int ,CMediaSocket*>::iterator rcIter;
	rcIter = videoSvrMediaMap.find(videoSvrID);
	if ( rcIter == videoSvrMediaMap.end() || rcIter->second == NULL)
	{
		return;
	}
	else
	{
		delete rcIter->second;
		rcIter->second = NULL;
	}

}

int CMediaSocket::RequestRecordPlayData( int cameraID,int fileNo )
{
	if (!m_SocketClient.IsOpen())
	{
		return KE_SOCKET_NOTOPEN;
	}
	int devID = cameraID/256;
	int channelNo = cameraID%256;
	std::vector<BYTE> msgSend;
	int msgLen = sizeof(KEPlayRecordFileReq);
	msgSend.resize(msgLen,0);
	PKEPlayRecordFileReq pReqMsg;
	pReqMsg = (PKEPlayRecordFileReq)&msgSend[0];
	pReqMsg->protocal = PROTOCOL_HEAD;
	pReqMsg->msgType = KEMSG_REQUEST_DOWNLOAD_FILE;
	pReqMsg->msgLength = msgLen;
	pReqMsg->clientID = m_clientID;
	pReqMsg->channelNo = channelNo;
	pReqMsg->videoID = devID;
	pReqMsg->clientIp = 0;
	
	if (m_SvrType == MediaSvr_Trans)
		pReqMsg->protocalType = 2;
	else
		pReqMsg->protocalType = 1;
	
	pReqMsg->fileNo = recordFileList[fileNo].fileNo;
	pReqMsg->fileType =0;
	CTime	 st = CTime(recordFileList[fileNo].startTime);
	pReqMsg->startTime[0] = st.GetYear()%2000;
	pReqMsg->startTime[1] = st.GetMonth();
	pReqMsg->startTime[2] = st.GetDay();
	pReqMsg->startTime[3] = st.GetHour();
	pReqMsg->startTime[4] = st.GetMinute();
	pReqMsg->startTime[5] = st.GetSecond();

	memcpy(pReqMsg->fileData,recordFileList[fileNo].fileData,80);
	int ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	return KE_SUCCESS;
}

std::map<int ,CMediaSocket*> CMediaSocket::videoSvrMediaMap;

// 
// CMediaSocket * CMediaSocket::GetMediaSocket( int videoID,int channelNo,bool noCreate )
// {
// 	int nIndex = CameraIndex(videoID,channelNo);
// 	if (g_MediaMap[nIndex] == NULL && !noCreate)
// 	{
// 		g_MediaMap[nIndex] = new CMediaSocket;
// 	}
// 	return g_MediaMap[nIndex];
// }
// 
// void CMediaSocket::SetMediaSocket( int videoID,int channelNo,CMediaSocket * mediaSocket )
// {
// 	int nIndex = CameraIndex(videoID,channelNo);
// 	if (g_MediaMap[nIndex] != NULL)
// 	{
// 		delete g_MediaMap[nIndex];
// 	}
// 	g_MediaMap[nIndex] = mediaSocket;
// }


//std::map<int,CMediaSocket *> CMediaSocket::g_MediaMap;

AudioTalkThread * AudioTalkThread::GetInstanse()
{
	static AudioTalkThread audioTalk;
	return &audioTalk;
}

void AudioTalkThread::Initialize( CSocketThreadHandler * socketHandle,int cameraID )
{
	this->m_socketHandle = socketHandle;
	this->m_cameraID = cameraID;
}

AudioTalkThread::AudioTalkThread()
{
	this->m_socketHandle = NULL;
}

AudioTalkThread::~AudioTalkThread()
{
	
}

void AudioTalkThread::Run()
{
	CMyAVPlayer::StartTalk();
	int datalen;
	char SendBuf[500];
	char buf[500];	
	while (!this->toStop)
	{
		datalen=CMyAVPlayer::GetAudioData(&buf[0]);
		if (datalen >0)
		{
			SendBuf[0]=(char) PROTOCOL_HEAD;					//协议头
			SendBuf[1]=(char) SEND_TALKDATA_TOMEDIASERVER ;		//协议类型	
			int msgLen = 17 + datalen;
			*(int *)&SendBuf[2] = msgLen;							//协议长度
			*(int *)&SendBuf[6] = m_cameraID/256;
			SendBuf[10] = m_cameraID%256;
			memcpy(&SendBuf[15], &buf[0], 6);
			*(short*)&SendBuf[21] = datalen - 6;
			memcpy(&SendBuf[23], &buf[6], datalen - 6);
			if (m_socketHandle!= NULL && m_socketHandle->IsConnect())
			{
				m_socketHandle->Write((BYTE *)SendBuf,msgLen);
			}
			else{
				TRACE(_T("AudioTalkThread::Run--No socket or socket is closed,exit thread"));
				break;
			}
		}
		Sleep(15);
	}
	CMyAVPlayer::StopTalk();
}
