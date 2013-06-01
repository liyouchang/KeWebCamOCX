#pragma once
#include "socketthreadhandler.h"
#include "MyAVPlayer.h"
#include "Recorder.h"
#include <queue>
#include <vector>
#include "Communication/CloudCtrl.h"
enum MediaType
{
	Media_None = 0x00,
	Media_Vedio = 0x01,
	Media_Listen = 0x02,
	Media_Talk = 0x04
};

enum MediaSvrType
{
	MediaSvr_DVS = 1,
	MediaSvr_Trans=2
};
enum MediaMsgType
{
	DevMsg_HeartBeat = 0x03,
	KEMSG_RecordFileList = 0x53,
	KEMSG_REQUEST_DOWNLOAD_FILE = 0x54,
	KEMSG_RecordPlayData = 0x55,
	DevMsg_GetPTZParam = 0x73,
	DevMsg_SerialData = 0x46,
	DevMsg_WifiCheck=0xD1,
	DevMsg_WifiStart = 0xD2,
	DevMsg_EncryptKey= 0xE0,
	DevMsg_Login=0xE1,
};


#pragma pack(1)


struct KEDevMsgHead
{
	BYTE protocal;
	BYTE msgType;
	unsigned int msgLength;
	int videoID;
};
struct KEDevMsgViaClient
{
	BYTE protocal;
	BYTE msgType;
	unsigned int msgLength;
	int videoID;
	int clientID;
};
struct KEDevMsgResp
{
	KEDevMsgViaClient head;
	char resp;
};
typedef struct _KEVideoServerReq
{
	BYTE protocal;
	BYTE msgType;//0x0c
	int msgLength;
	int videoID;
	int clientID;
	char channelNo;
	char video;//视频=0请求   =1 停止
	char listen;//监听=0请求   =1 停止
	char talk;// 对讲=0请求   =1 停止
	char protocalType;
	int transSvrIp;
}KEVideoServerReq,*PKEVideoServerReq;

typedef struct _KEVideoServerResp
{
	BYTE protocal;
	BYTE msgType;//0x0c
	int msgLength;
	int videoID;
	int clientID;
	char channelNo;
	char respType;// ACK/NAK
}KEVideoServerResp,*PKEVideoServerResp;

struct KETimeStruct
{
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
	BYTE second;
};

typedef struct _KEPlayRecordFileReq
{
	BYTE protocal;
	BYTE msgType;//0x54
	int msgLength;//31
	int videoID;
	int clientID;
	char channelNo;
	char fileType;//0 无用
	int fileNo;//1 无用
	KETimeStruct startTime;//无用
	int clientIp;//0或本机ip
	char protocalType;//1 无用
	char fileData[80];
}KEPlayRecordFileReq,*PKEPlayRecordFileReq;

typedef struct _KEPlayRecordDataHead
{
	BYTE protocal;
	BYTE msgType;//0x55
	int msgLength;//15
	int videoID;
	int clientID;
	char channelNo;
	char resp;//13 后面有数据 ，6 数据结束
}KEPlayRecordDataHead,*PKEPlayRecordDataHead;

typedef struct _KERecordFileListReq
{
	BYTE protocal;
	BYTE msgType;//0x53
	int msgLength;//29
	int videoID;
	int clientID;
	char channelNo;
	char startTime[6];
	char endTime[6];
	char fileType;//1计划录像；2表示传感器报警；	3移动侦测报警；4抓拍照片；
	char alarmNo;//1
}KERecordFileListReq,*PKERecordFileListReq;

typedef struct _KERecordFileListResp
{
	BYTE protocal;
	BYTE msgType;//0x53
	int msgLength;
	int videoID;
	int clientID;
	char channelNo;
	char resp;
}KERecordFileListResp,*PKERecordFileListResp;

typedef struct _KERecordFileInfo
{
	int fileNo;
	char startTime[6];
	char endTime[6];
	int fileSize;
	char data[80];
}KERecordFileInfo,*PKERecordFileInfo;

struct KEDevGetPTZParamReq
{
	BYTE protocal;
	BYTE msgType;//0x73
	int msgLength;
	int videoID;
	int clientID;
	char channelNo;
};
struct KEDevGetSerialDataHead
{
	BYTE protocal;
	BYTE msgType;//0x46
	int msgLength;
	int videoID;
	int clientID;
	short dataLen;
};
struct KEDevAPListItem
{
	char essid[32];
	char encryptType;//0/1/2/3 关闭/wep/WPA-PSK/WPA2-PSK
	char wepPlace;//WEP:1-4的数字 PSK:1- TKIP;2-AES
	char pwdFormat;//0/1 WEP 16禁制/ASCII
	char wifiStart;//0/1=关/开
	char password[32];
};
struct KEDevWifiCheckReq{
	BYTE protocal;
	BYTE msgType;//0xD1
	int msgLength;
	int videoID;
	int clientID;
};
struct KEDevWifiCheckResp{
	BYTE protocal;
	BYTE msgType;//0xD1
	int msgLength;
	int videoID;
	int clientID;
	char resp;
};
struct KEDevWifiStartReq{
	BYTE protocal;
	BYTE msgType;//0xD2
	int msgLength;
	int videoID;
	int clientID;
	KEDevAPListItem APItem;
	char  pppoeUse;//0/1关/开
	char pppoeAccount[30];
	char pppoePWD[30];
};

struct KEDevEncryptKeyResp{
	KEDevMsgViaClient head;
	char keyt[8];
};
struct KEDevLoginReq{
	KEDevMsgViaClient head;
	char md5Str[16];
};

#pragma pack()

struct RecordFileInfo
{
	int fileNo;
	int startTime;
	int endTime;
	int fileSize;
	char fileData[80];
};
struct DevPTZInfo
{
	bool isgot;
	char protocal;
	char addr;
};

class AudioTalkThread:public SimpleThreadBase
{
public:
	AudioTalkThread();
	virtual ~AudioTalkThread();
	void Initialize(CSocketThreadHandler * socketHandle , int cameraID);
	virtual void Run();
	static AudioTalkThread * GetInstanse();
protected:
	CSocketThreadHandler * m_socketHandle;
	int m_cameraID;
};

class CMediaSocket :
	public CSocketThreadHandler
{
public:
	CMediaSocket(void);
	virtual ~CMediaSocket(void);
	virtual bool Init();
	int  GetMediaSvrType(){return m_SvrType;}
	bool ConnectToServer(CString severAddr, CString serverPort);
	bool ConnectToServer(int serverAddr, int serverPort, int svrType = 1, int clientID = 0);
	virtual void CloseConnect();
	int ReqestMediaData(int cameraID,int mediaType);
	int RemoteRecordPlay(int cameraID,int fileNo);
	int StartRecord(const char * fileName);
	int StopRecord();
	int GetRecordFileList(int cameraID,int startTime,int endTime,int fileType,std::vector<RecordFileInfo> & fileInfoList);
	int PTZControl(int cameraID, BYTE ctrlType ,BYTE speed );
	int GetDevWifiAPList(int cameraID);
	int SetDevWifi(int cameraID,int apListNum,KEDevWifiStartReq wifiStart);
	//通过发送心跳协议，检查链接是否正常；
	int CheckHeartBeat(int devID);
protected://parents functions inherit
	virtual bool CheckMessage(const BYTE* data, DWORD dwCount);
	virtual void HandleMessage(const BYTE* msgData);
	virtual void Run();
	virtual bool GetMessageData();
	virtual void OnThreadExit(CSocketHandle* pSH);
	virtual void OnConnectionDropped(CSocketHandle* pSH);
	virtual void OnConnectionError(CSocketHandle* pSH, DWORD dwError);
protected:
	int ReqestMediaTrans( int videoID, int channelNo, int mediaType);
	int ReqestVideoServer(int videoID, int channelNo, int mediaType);
	int GetPTZParam(int cameraID);
	virtual int OpenMedia(int cameraID,int mediaType);
	int RequestEncryptKey(char * keyt);
protected://received message functions
	void RecvMediaTransResp(const BYTE * msgData);
	void RecvVideoStream(const BYTE * msgData);
	void RecvVideoServer(const BYTE * msgData);
	void RecvAudioStream(const BYTE * msgData);
	void RecvRecordFileList(const BYTE * msgData);
	void RecvRecordPlayData(const BYTE * msgData);
	void RecvGetPTZParam(const BYTE * msgData);
	void RecvDevWifiCheck(const BYTE * msgData);
	void RecvSetDevWifiResp(const BYTE * msgData);
	void RecvHeartBeat(const BYTE * msgData);
protected:
	CRecorder * m_Recorder;
	CMyAVPlayer * m_AVPlayer;
	int m_clientID;
	CEvent mediaEvent;
	int respData;
	int m_videoID;
	int m_channelNo;
	int m_SvrType;//1,视频服务器；2，媒体服务器 
	DevPTZInfo ptzInfo;
public:
	std::vector<RecordFileInfo> recordFileList;
	std::vector<KEDevAPListItem> APList;
};
