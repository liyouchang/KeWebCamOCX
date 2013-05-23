#pragma once

#include <afxmt.h>
#include "SocketThreadHandler.h"
#include "HeartBeatThread.h"



class CCmdSocket :public CSocketThreadHandler
{
public:
	CCmdSocket(void);
	virtual ~CCmdSocket(void);

	virtual bool Init();
	int GetClientID(){return m_clientID;}
	bool ConnectToServer(CString severAddr);
	virtual void CloseConnect();
	int LoginServer(CString userName,CString pwd);
	int StartView(int vedioID,int ChannelID,long PlayHwd );
	int SendRealTimeDataMsg(int vedioID, char channelNo, char reqType, char dataType);
	int SendPTZControlMsg(int cameraID, BYTE ctrlType, BYTE speed, BYTE data);
	int SendAskTreeMsg(int dataType,char * data,int dataLen);
	int AskAllRootNodes(short packageNo = 0);
	

protected:
	virtual void HandleMessage(const BYTE* msgData);
	virtual void Run();
	
	void AskKeyMsgResp(const BYTE* msgData);
	void LoginMsgResp(const BYTE* msgData);
	void RecvRealTimeMedia(const BYTE* msgData);
	void RecvMalfunctionAlert(const BYTE *msgData);
	void RecvHeartBeat(const BYTE * msgData);
	void RecvAskTreeMsg(const BYTE * msgData);
	void RecvVideoSvrOnline(const BYTE * msgData);
	//heartbeat
	CHeartBeatThread  m_HeartbeatThread;
	XMLInfoThread m_xmlInfoThread;
protected:
	static unsigned int __stdcall XMLInfoThreadProc(void* arg);

	int AskKeyt(char * keyt);
	int SendLoginMsg(const char * userName, const char *encryptData);

private:
	int m_msgWaitTime;
	int m_clientID;

private:
	void EncryptData(const char * userName, const char * pwd, const char * keyt, char * encryptedData);
	int FindChNodeByID(const std::vector<CHNODE> & nodes,int nID);
};
