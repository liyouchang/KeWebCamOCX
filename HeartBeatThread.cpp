#include "StdAfx.h"
#include "HeartBeatThread.h"
#include "SocketThreadHandler.h"
#include <afxmt.h>
CHeartBeatThread::CHeartBeatThread(void)
{
	m_socketHandle = NULL;
	m_HeartbeatTime = 10000;//10s Ò»´ÎÐÄÌø
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
		msg.head.msgType = KEMSG_TYPE_HEARTBEAT;
		msg.head.protocal = PROTOCOL_HEAD;
		msg.status = 0;
		m_socketHandle->Write((BYTE *)&msg,msg.head.msgLength);
		m_heartCount++;
		DWORD dw = WaitForSingleObject(m_HeartbeatWaitEvent.m_hObject,m_HeartbeatTime);
		if (dw == WAIT_TIMEOUT)
		{
			TRACE1("Wait heart beat %d!\n",m_heartCount);
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
