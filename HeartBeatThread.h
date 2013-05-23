#pragma once
#include "CommonUtility/SimpleThreadBase.h"
#include <afxmt.h>
#include "CommonUtility/Markup.h"
#include <string>
#include <vector>
#include "Common.h"
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

class XMLInfoThread:public SimpleThreadBase
{
public:
	XMLInfoThread();
	virtual ~XMLInfoThread();
	void Initialize(CSocketThreadHandler * socketHandle, int clientID);
	virtual void Run();
	std::string AllNotesInfo;
	short currentPackage;
	std::vector<CHNODE> upperNodes;
	std::vector<CHNODE> videoSvrNodes;
	std::vector<CHNODE> tmpVideoSvrNodes;
	std::vector<CHNODE> channelNodes;
protected:
	void DoXmlToMap(CMarkup &xml);
	int SendVideoSvrOnline();

	CSocketThreadHandler * m_socketHandle;
	int m_clientID;

};