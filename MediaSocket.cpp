#include "StdAfx.h"
#include "MediaSocket.h"
#include "KeWebCamOCX.h"
CMediaSocket::CMediaSocket(void)
{
	m_AVPlayer = NULL;
	m_videoID = 0;
	m_channelNo = 0;
	//Init();
}

CMediaSocket::~CMediaSocket(void)
{
	CloseConnect();
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
		break;
	default:
		break;
		//LOG_INFO("Receive unkown message: " <<pHead->msgType);

	}
}

bool CMediaSocket::Init()
{
	const int recvBufSize = 0x40000;
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
	return true;
}

bool CMediaSocket::ConnectToServer( int serverAddr,int serverPort )
{
	TCHAR pointIP[MAX_PATH];
	if (!CSocketHandle::FormatIP(pointIP,MAX_PATH,serverAddr,false))
	{
		TRACE("ConnectToServer--FormatIP Error!");
		return false;
	}
	TCHAR strPort[8];
	_itot(serverPort,strPort,10);

	return ConnectToServer(pointIP,strPort);

}

int CMediaSocket::ReqestMediaTrans( int clientID,int videoID,int channelNo,int mediaType )
{
	if (!m_SocketClient.IsOpen())
	{
		return KE_SOCKET_NOTOPEN;
	}
	m_clientID = clientID;

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
	pReqMsg->devType = 3;
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
		{
			int videoID = pReqMsg->videoID;
			int channelNo = pReqMsg->channelNo;
			int cameraID = MakeCameraID(videoID,channelNo);
			m_AVPlayer = theApp.g_PlayWnd->GetOnePlayer(cameraID)->m_AVIPlayer;
			int ret = m_AVPlayer->OpenStream();
			if (ret != 0)
			{
				LOG_ERROR(" Open stream error: "<<ret);
				return KE_RTV_OpenStreamFailed;
			}
		}
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
	PKEVideoStreamHead pMsg = (PKEVideoStreamHead)msgData;
	//TRACE2("Receive %d Msg Len %d\n",pMsg->msgType,pMsg->msgLength);
	int videoID = pMsg->videoID;
	int channelNo = pMsg->channelNo;
	int ret = m_AVPlayer->InputStream(msgData+sizeof(KEVideoStreamHead),pMsg->msgLength-sizeof(KEVideoStreamHead));
	if (ret != 0)
	{
		LOG_ERROR(" input stream error: "<<ret);
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
		if (m_recvBuf.GetCount() == 0 )
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
	m_SocketClient.Terminate();
	//清空读缓冲
	m_recvBuf.Drain(m_recvBuf.GetCount() );
}

int CMediaSocket::ReqestVideoServer( int clientID,int videoID,int channelNo ,int mediaType)
{
	if (!m_SocketClient.IsOpen())
	{
		return KE_SOCKET_NOTOPEN;
	}
	m_clientID = clientID;

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
	pReqMsg->video =(mediaType & Media_Vedio)?0: 1;
	pReqMsg->listen = (mediaType & Media_Listen)?0: 1;
	pReqMsg->talk = (mediaType & Media_Talk)?0: 1;
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
		LOG_ERROR("ReqestVideoServer timeout !");
		return KE_MSG_TIMEOUT;
	}
	ret = respData;
	switch(ret)
	{
	case  0x0d://请求成功
		{
			int videoID = pReqMsg->videoID;
			int channelNo = pReqMsg->channelNo;
			int cameraID = MakeCameraID(videoID,channelNo);
			m_AVPlayer = theApp.g_PlayWnd->GetOnePlayer(cameraID)->m_AVIPlayer;
			int ret = m_AVPlayer->OpenStream();
			if (ret != 0)
			{
				LOG_ERROR(" Open stream error: "<<ret);
				return KE_RTV_OpenStreamFailed;
			}
		}
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
