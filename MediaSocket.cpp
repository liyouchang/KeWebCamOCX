#include "StdAfx.h"
#include "MediaSocket.h"
#include "KeWebCamOCX.h"
CMediaSocket::CMediaSocket(void)
{
	m_AVPlayer = NULL;
	m_videoID = 0;
	m_channelNo = 0;
	this->m_SvrType = 0;
	m_Recorder = NULL;
	m_nSockType = SOCK_TCP;
	Init();
	
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
	const int recvBufSize = 0x10000;
	//const int recvBufSize = 4096;
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
	case 2:		return KE_RTV_VIDEOSERVEROFFLINE;
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
	int ret = m_AVPlayer->InputStream(msgData+sizeof(KERTStreamHead),pMsg->msgLength-sizeof(KERTStreamHead));
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
	return KE_SUCCESS;
}



int CMediaSocket::ReqestMediaData( int cameraID,int mediaType )
{
	int videoID = cameraID /256;
	int channelNo = cameraID %256;
	m_videoID = videoID;
	m_channelNo = channelNo;
	int ret;
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
	int ret = m_AVPlayer->InputStream(msgData+sizeof(KERTStreamHead),pMsg->msgLength-sizeof(KERTStreamHead));
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

	return ret;
}

void CMediaSocket::RecvRecordFileList( const BYTE * msgData )
{
	PKERecordFileListResp pMsg = (PKERecordFileListResp)msgData;
	if (pMsg->msgLength == 16)
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

	if (pMsg->resp == 6)
	{
			this->SetRecvMsg(KEMSG_RecordFileList,KE_SUCCESS);
	}
	
}

int CMediaSocket::RemoteRecordPlay( int cameraID,int fileNo )
{
	if (fileNo > recordFileList.size())
	{
		return KE_ERROR_PARAM;
	}
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
	pReqMsg->protocalType = 1;
	
	memcpy(pReqMsg->fileData,recordFileList[fileNo].fileData,80);
	int ret = OpenMedia(cameraID,Media_Vedio);
	if (ret != KE_SUCCESS)
	{
		return ret;
	}
	ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}

	return KE_SUCCESS;

}

void CMediaSocket::RecvRecordPlayData( const BYTE * msgData )
{
	PKEPlayRecordDataHead pMsg = (PKEPlayRecordDataHead)msgData;
	int ret = m_AVPlayer->InputStream(msgData+sizeof(PKEPlayRecordDataHead),pMsg->msgLength-sizeof(PKEPlayRecordDataHead));
	if (ret != 0)
	{
		LOG_ERROR(" input stream error: "<<ret);
	}
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
	ptzInfo.isgot = true;
	ptzInfo.protocal = 0;
	ptzInfo.addr = 0;
	if (!ptzInfo.isgot)
	{
		ret = GetPTZParam(cameraID);
		if (ret != KE_SUCCESS)		return ret;
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
			if (m_socketHandle!= NULL)
			{
				m_socketHandle->Write((BYTE *)SendBuf,msgLen);
			}
		}
		Sleep(15);
	}
	CMyAVPlayer::StopTalk();
}
