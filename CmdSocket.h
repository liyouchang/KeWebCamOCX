/************************************************************************/
/* 2008平台消息实现
	
*/
/************************************************************************/
#pragma once

#include <afxmt.h>
#include "SocketThreadHandler.h"
#include "HeartBeatThread.h"
#include "CenterCommand.h"


class CCmdSocket :public CSocketThreadHandler,public CenterCommand
{
public:
	CCmdSocket(void);
	virtual ~CCmdSocket(void);

	virtual bool Init();
	virtual int ConnectServer(CString svrIp, int svrPort);
	virtual int LoginServer(CString userName,CString pwd);
	virtual int StartView(int cameraID);
	virtual int StopView(int cameraID);
	virtual int PTZControl(int cameraID, BYTE ctrlType ,BYTE speed ,BYTE data);
	virtual int GetRecordFileList(int cameraID,int startTime,int endTime,int fileType,int targetType,vector<RecordFileInfo> & fileInfoList);
	virtual int PlayRemoteRecord(int cameraID,int fileNo);
	virtual int HeartBeat();
	virtual int GetClientID(){return m_clientID;}

	bool ConnectToServer(CString severAddr);
	virtual void CloseConnect();
	int SendRealTimeDataMsg(int vedioID, char channelNo, char reqType, char dataType);
	int SendPTZControlMsg(int cameraID, BYTE ctrlType, BYTE speed, BYTE data);
	int SendAskTreeMsg(int dataType,char * data,int dataLen);
		
	int AskAllRootNodes(short packageNo = 0);

protected:
	virtual void HandleMessage(const BYTE* msgData);
	int ConnectToMedia(int cameraID);
	
	
		
protected:
	int RequestHistoryData(int cameraID,int startTime,int endTime,int fileType,int targetType= 2);
	void AskKeyMsgResp(const BYTE* msgData);
	void LoginMsgResp(const BYTE* msgData);

protected:
	void RecvRealTimeMedia(const BYTE* msgData);
	void RecvMalfunctionAlert(const BYTE *msgData);
	void RecvHeartBeat(const BYTE * msgData);
	void RecvAskTreeMsg(const BYTE * msgData);
	void RecvVideoSvrOnline(const BYTE * msgData);
	void RecvHistoryData(const BYTE * msgData);
	//heartbeat
	//CHeartBeatThread  m_HeartbeatThread;
	XMLInfoThread m_xmlInfoThread;
protected:
	static unsigned int __stdcall XMLInfoThreadProc(void* arg);
	int AskKeyt(char * keyt);
	int SendLoginMsg(const char * userName, const char *encryptData);
private:
	int m_msgWaitTime;
	int m_clientID;
	int mediaSvrIp;
	int mediaTransIp;
	int mediaOnline;
	int mediaTarget;
	volatile   int m_heartCount;
private:
	void EncryptData(const char * userName, const char * pwd, const char * keyt, char * encryptedData);
	int FindChNodeByID(const std::vector<CHNODE> & nodes,int nID);

};


enum KEMSG_08_TYPE
{
	KEMSG_TYPE_ASKKEY = 0xD0,
	KEMSG_TYPE_LOGIN = 0x80,
	KEMSG_TYPE_AskTreeStruct = 0x81,
	KEMSG_TYPE_HEARTBEAT=0x82,
	KEMSG_08_REALTIMEDATA = 0x83,
	KEMSG_TYPE_PTZControl = 0x85,
	KEMSG_TYPE_MalfunctionAlert = 0x8A,
	KEMSG_08_HistoryData = 0x8D,
};

#pragma pack(1)

struct KEHistoryDataReq
{
	BYTE protocal;
	BYTE msgType;//0x8d
	int msgLength;//29
	int videoID;
	int clientID;
	char channelNo;
	char targetType; //2视频服务器  3=录像服务器;data = 0）
	char startTime[6];
	char endTime[6];
	char fileType;//=0全部,1计划录像,2传感器报警,3移动侦测报警,4查看远程快照,5传感器联动报警,6平安互助联动报警,10终端串口输出日志.
	char alarmNo;//1
	char mediaType;// 0-主媒体 1-备用媒体
};

struct KEHistoryDataResp
{
	BYTE protocal;
	BYTE msgType;//0x8d
	int msgLength;
	int videoID;
	int clientID;
	char channelNo;
	int transIp;
	int vsIp;
	char target;
	char resp;
	char online;// 0=都在线  1=视频服务器在线  2=转发服务器在线 3都不在线
};


#pragma pack()
