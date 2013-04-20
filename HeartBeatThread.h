#pragma once
#include "CommonUtility/SimpleThreadBase.h"
#include <afxmt.h>

class CSocketThreadHandler;

class CHeartBeatThread :
	public SimpleThreadBase
{
public:
	CHeartBeatThread(void);
	virtual ~CHeartBeatThread(void);
	void Initialize(CSocketThreadHandler * socketHandle, int clientID);
	virtual void Run();
	virtual void Stop(DWORD dwTimeout = INFINITE);
	void ResetCount();
protected:
	CSocketThreadHandler * m_socketHandle;
	int m_clientID;
	volatile int m_heartCount;
	int m_HeartbeatTime;
	CEvent m_HeartbeatWaitEvent;
};
