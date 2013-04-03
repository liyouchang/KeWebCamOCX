#pragma once

#include <afxmt.h>
#include "SocketThreadHandler.h"


#define WM_HEARTBEATSTOP WM_USER+1029
 


class CCmdSocket :public CSocketThreadHandler
{
	#define  RECV_BUF_SIZE  0x40000
public:
	CCmdSocket(void);
	virtual ~CCmdSocket(void);

	virtual bool Init();

	
	bool ConnectToServer(CString severAddr);
	int LoginServer(CString userName,CString pwd);
	int StartView(int vedioID,int ChannelID,long PlayHwd );
	int SendRealTimeDataMsg(int vedioID,char channelNo,char reqType,char dataType);

protected:
	virtual void HandleMessage(const BYTE* msgData);

	//heartbeat
	static unsigned int __stdcall ThreadHeartBeat(void* arg);
	HANDLE heartbeatThread;
	void StartHeartBeat();
	void StopHeartBeat();
	int m_heartCount;
	bool heartBeatStart;
	int m_HeartbeatTime;

protected:
	int AskKeyt(char * keyt);
	int SendLoginMsg(const char * userName,const char *encryptData);
	void AskKeyMsgResp(const BYTE* msgData);
	void LoginMsgResp(const BYTE* msgData);
	void RecvRealTimeMedia(const BYTE* msgData);
	void RecvMalfunctionAlert(const BYTE *msgData);
private:

	int m_clientID;
	CEvent keEvent[KEMSG_EVENT_MAX];		//申请key
	int respData[KEMSG_EVENT_MAX];	//传递消息读取的数据,resp函数生成,使用者释放
private:
	void EncryptData(const char * userName,const char * pwd,const char * keyt,char * encryptedData);
};
