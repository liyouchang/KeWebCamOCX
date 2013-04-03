#include "StdAfx.h"
#include "MediaSocket.h"
#include "KeWebCamOCX.h"
CMediaSocket::CMediaSocket(void)
{
	m_AVPlayer = NULL;
	//Init();
}

CMediaSocket::~CMediaSocket(void)
{
}

void CMediaSocket::HandleMessage( const BYTE* msgData )
{
	PKEMsgHead pHead = (PKEMsgHead)msgData;

	switch(pHead->msgType)
	{
	case KEMSG_TYPE_MEDIATRANS:
		RecvMediaTransResp(msgData);
		break;
	case KEMSG_TYPE_LOGIN:
		
		break;
	case KEMSG_TYPE_REALTIMEDATA:
		
		break;
	case KEMSG_TYPE_MalfunctionAlert:
	
		break;
	default:
		LOG_INFO("Receive unkown message: " <<pHead->msgType);

	}
}

bool CMediaSocket::Init()
{
	const int recvBufSize = 0x80000;
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
	//char pointIP[MAX_PATH];
	//inet_ntop(AF_INET,&transIp,pointIP,MAX_PATH);
	//CString dstp = pointIP;
	return true;
}

int CMediaSocket::ReqestMediaTrans( int clientID,int videoID,int channelNo,int mediaType )
{
	if (!m_SocketClient.IsOpen())
	{
		return KE_SOCKET_NOTOPEN;
	}
	m_clientID = clientID;

	std::vector<BYTE> msgSend;
	int msgLen = sizeof(KEMediaTransReq);
	msgSend.resize(msgLen,0);

	PKEMediaTransReq pReqMsg;
	pReqMsg = (PKEMediaTransReq)&msgSend[0];
	
	pReqMsg->head.protocal = PROTOCOL_HEAD;
	pReqMsg->head.msgType = KEMSG_TYPE_REALTIMEDATA;
	pReqMsg->head.msgLength = msgLen;
	pReqMsg->head.clientID = m_clientID;
	pReqMsg->channelNo = channelNo;
	pReqMsg->videoID = videoID;
	pReqMsg->video =(mediaType & Media_Vedio)?0: 1;
	pReqMsg->listen = (mediaType & Media_Listen)?0: 1;
	pReqMsg->talk = (mediaType & Media_Talk)?0: 1;
	pReqMsg->devType = 2;
	mediaEvent.ResetEvent();
	int ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	DWORD dw = WaitForSingleObject(mediaEvent.m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		LOG_ERROR("请求转发服务器视频消息超时!");
		return KE_MSG_TIMEOUT;
	}
	
	return KE_SUCCESS;
}

void CMediaSocket::RecvMediaTransResp( const BYTE* msgData )
{
	PKEMediaTransResp pMsg = (PKEMediaTransResp)msgData;
	
	respData = pMsg->respType;
	Sleep(0);
	mediaEvent.SetEvent();
	
	if (respData == 0)
	{
		int videoID = pMsg->videoID;
		int channelNo = pMsg->channelNo;
		m_AVPlayer = theApp.g_PlayWnd->GetOnePlayer(videoID,channelNo);
		
	}

}
