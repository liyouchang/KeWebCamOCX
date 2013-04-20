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
	int SendRealTimeDataMsg(int vedioID,char channelNo,char reqType,char dataType);
	int SendPTZControlMsg(int cameraID,BYTE ctrlType,BYTE speed,BYTE data);
protected:
	virtual void HandleMessage(const BYTE* msgData);
	virtual void Run();

	void AskKeyMsgResp(const BYTE* msgData);
	void LoginMsgResp(const BYTE* msgData);
	void RecvRealTimeMedia(const BYTE* msgData);
	void RecvMalfunctionAlert(const BYTE *msgData);
	void RecvHeartBeat(const BYTE * msgData);
	
	//heartbeat
	CHeartBeatThread  m_HeartbeatThread;

protected:
	int AskKeyt(char * keyt);
	int SendLoginMsg(const char * userName, const char *encryptData);
	
private:
	int m_msgWaitTime;
	int m_clientID;
	CEvent keEvent[KEMSG_EVENT_MAX];		//申请key
	int respData[KEMSG_EVENT_MAX];	//传递消息读取的数据,resp函数生成,使用者释放

private:
	void EncryptData(const char * userName, const char * pwd, const char * keyt, char * encryptedData);
};
