#include "StdAfx.h"
#include "HeartBeatThread.h"
#include "SocketThreadHandler.h"
#include "KeWebCamOCX.h"
#include <afxmt.h>
CHeartBeatThread::CHeartBeatThread(void)
{
	m_socketHandle = NULL;
	m_HeartbeatTime = 10000;//10s 一次心跳
}

CHeartBeatThread::~CHeartBeatThread(void)
{
	if (isRunning())
	{
		Stop();
	}
}

void CHeartBeatThread::Run()
{
	if ( !m_socketHandle )
		return ;
	LOG_DEBUG("Start Heartbeat thread!\n");
	//CCmdSocket * ptr = static_cast<CCmdSocket*>(arg);
	m_heartCount = 0;
	while(m_socketHandle->IsConnect() && !toStop)
	{
		if (m_heartCount > 3)
		{
			LOG_ERROR("Heartbeat Timeout, close connetion");
			m_socketHandle->CloseConnect();
			break;
		}
		KEMsgHeartBeat msg;
		msg.head.clientID = m_clientID;
		msg.head.msgLength = sizeof(KEMsgHeartBeat);
		msg.head.msgType = 0x82;
		msg.head.protocal = PROTOCOL_HEAD;
		msg.status = 0;
		m_socketHandle->Write((BYTE *)&msg,msg.head.msgLength);
		m_heartCount++;
		DWORD dw = WaitForSingleObject(m_HeartbeatWaitEvent.m_hObject,m_HeartbeatTime);
		if (dw == WAIT_TIMEOUT)
		{
			//TRACE1("Wait heart beat %d!\n",m_heartCount);
			continue;
		}
		else
		{
			LOG_DEBUG("Heart beat thread stopped!");
			break;
		}
	}
	LOG_DEBUG("Exit heartbeat thread!\n");
}

void CHeartBeatThread::Initialize( CSocketThreadHandler * socketHandle, int clientID )
{
	this->m_socketHandle = socketHandle;
	this->m_clientID = clientID;
	m_HeartbeatWaitEvent.ResetEvent();
}

void CHeartBeatThread::Stop( DWORD dwTimeout /*= INFINITE*/ )
{
	if (!isRunning())
	{
		return;
	}
	m_HeartbeatWaitEvent.SetEvent();
	toStop = true;
	if ( _thread != NULL )
	{
		if ( WaitForSingleObject(_thread, dwTimeout) == WAIT_TIMEOUT ) {
			TerminateThread(_thread, 1);
		}
		CloseHandle(_thread);
		_thread = NULL;
	}
}

void CHeartBeatThread::ResetCount()
{
	m_heartCount = 0;
}



XMLInfoThread::XMLInfoThread()
{
	m_socketHandle = NULL;
}

XMLInfoThread::~XMLInfoThread()
{

}

void XMLInfoThread::Initialize( CSocketThreadHandler * socketHandle, int clientID )
{
	this->m_socketHandle = socketHandle;
	this->m_clientID = clientID;
}

void XMLInfoThread::Run()
{
	LOG_INFO("start XMLInfoThreadProc thread");
	//CCmdSocket * ptr = (CCmdSocket *)arg;
	CMarkup g_xml;
	tstd::tstring xmlStr = str_to_tstr(AllNotesInfo);
	AllNotesInfo.clear();
	g_xml.SetDoc(xmlStr.c_str());
	if (g_xml.IsWellFormed())
	{
		//log("xml is well");
		if (g_xml.FindElem(_T("AllRootNodes")))
		{
			g_xml.IntoElem();
			upperNodes.clear();
			videoSvrNodes.clear();
			channelNodes.clear();
			DoXmlToMap(g_xml);
			if (SendVideoSvrOnline() != KE_SUCCESS)
			{
				return ;
			}
		}
	}
	else
	{
		LOG_INFO("*******************error xml not formed**********************");
	} 
	LOG_INFO("start XMLInfoThreadProc thread End");
}

void XMLInfoThread::DoXmlToMap( CMarkup &xml )
{
	while(xml.FindElem(_T("Node")) && !toStop)
	{
		CHNODE changeNode;
		changeNode.NodeType = _ttoi(xml.GetAttrib(_T("NodeType")));
		changeNode.NodeName = xml.GetAttrib(_T("Name"));
		changeNode.ParentNodeID =  _ttoi(xml.GetAttrib(_T("ParentID")));
		changeNode.NodeID = _ttoi(xml.GetAttrib(_T("ID")));
		changeNode.onLine = 0;
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

		if (videoSvrNodes.size() >3)//一次发送需要大一些，小了可能会有单个状态消息返回
		{
			int ret = SendVideoSvrOnline();
// 			if (ret != KE_SUCCESS)
// 			{
// 				LOG_ERROR("SendVideoSvrOnline Error , ErrorCode="<<ret);
// 				break;
// 			}
			Sleep(100);
		}

		if (xml.IntoElem())
		{	
			DoXmlToMap(xml);
			xml.OutOfElem();
		}
	}//while
}

int XMLInfoThread::SendVideoSvrOnline()
{

	if (!m_socketHandle->IsConnect() )
	{
		return KE_SOCKET_NOTOPEN;
	}
	if (videoSvrNodes.size() == 0 || toStop)
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
	int ret = m_socketHandle->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	ret =m_socketHandle->WaitRecvMsg(KEMSG_TYPE_VideoSvrOnline);
	if (ret != KE_SUCCESS)
	{
		LOG_ERROR("Wait SendVideoSvrOnline  time out");
		return KE_MSG_TIMEOUT;
	}
// 	ret = m_socketHandle->GetRecvMsgData(KEMSG_TYPE_VideoSvrOnline);
// 	if (ret > 0)
// 	{
// 		return KE_SUCCESS;
// 	}
	if (!toStop && theApp.g_pMainWnd->GetSafeHwnd() != NULL)
	{
		theApp.g_pMainWnd->SendMessage(WM_TREESTRUCTNOTIFY,
			KEMSG_ASKTREE_DATATYPE_AllRootNodes,(LPARAM)(&upperNodes));		
	}
	if (!toStop &&theApp.g_pMainWnd->GetSafeHwnd() != NULL)
	{
		theApp.g_pMainWnd->SendMessage(WM_TREESTRUCTNOTIFY,
			KEMSG_ASKTREE_DATATYPE_AllRootNodes,(LPARAM)(&videoSvrNodes));		
	}
	if (!toStop &&theApp.g_pMainWnd->GetSafeHwnd() != NULL)
	{
		theApp.g_pMainWnd->SendMessage(WM_TREESTRUCTNOTIFY,
			KEMSG_ASKTREE_DATATYPE_AllRootNodes,(LPARAM)(&channelNodes));	
	}

	upperNodes.clear();
	videoSvrNodes.clear();
	channelNodes.clear();

	return KE_SUCCESS;
}
