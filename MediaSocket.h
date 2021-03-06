#pragma once
#include "socketthreadhandler.h"
#include "MyAVPlayer.h"
#include "Recorder.h"
#include <queue>
#include <vector>
#include <map>
#include <list>
#include "Communication/CloudCtrl.h"
#include "KeMsg.h"

enum MediaSvrType
{
	MediaSvr_Reuse = 0,
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
	KEMSG_TYPE_VideoSvrOnline = 0x8B,//13.	请求视频服务器的状态
	KEMSG_TYPE_MEDIATRANS = 0x8F,
	KEMSG_TYPE_VIDEOSERVER = 0x0C,
	KEMSG_TYPE_AUDIOSTREAM = 0x38,
	KEMSG_TYPE_VIDEOSTREAM = 0x39,
	DevMsg_SETVIDEOPARAM= 0x3B,
	DevMsg_GETVIDEOPARAM = 0x3E,
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
struct KEDevMsgCommonReq{
	BYTE protocal;
	BYTE msgType;
	unsigned int msgLength;
	int videoID;
	int clientID;
	char chanelNo;
};
struct KEDevMsgCommonResp{
	BYTE protocal;
	BYTE msgType;
	unsigned int msgLength;
	int videoID;
	int clientID;
	char chanelNo;
	char resp;
};
struct KEDevMsgResp
{
	KEDevMsgViaClient head;
	char resp;
};
struct KEDevAPListItem
{
	char essid[32];
	char encryptType; // 0/1/2/3 关闭/wep/WPA-PSK/WPA2-PSK
	char wepPlace;      //WEP:1-4的数字 PSK:1- TKIP;2-AES
	char pwdFormat;  //0/1 WEP 16禁制/ASCII
	char wifiStart;        //0/1=关/开
	char password[32];
};

struct KEDevWifiStartReq{
	BYTE protocal;
	BYTE msgType; //0xD2
	int msgLength;
	int videoID;
	int clientID;
	KEDevAPListItem APItem;
	char  pppoeUse;  //0/1关/开
	char pppoeAccount[30];
	char pppoePWD[30];
};


struct KEVedioParam{
	char videoFormat;//视频图像大小（P制）：0-D1，1-qcif，2-CIF，3-hd1，4-QVGA，5-SVGA(800*600)，6-FULL HD，7-VGA(640*480)，8-QQVGA(160*112)，9-XGA(1024*768)，10-D4/720p(1280*720)，11-WXGA(1280*800)，12-XVGA(1280*960)，13-UXGA(1600*1200)，14-D2/480p(720*480)（N制）：0-VGA, 1-QQVGA, 2-QVGA
	char videoFramerate;//视频帧率
	char mainIFrameInterval;// 关键桢间隔
	short maxRate;// 最大码率：16K~3000  
	short targetRate;//目标码率：16K~3000
	char rateControlMethod;//码率控制方式：0/1=VBR/CBR
	char preferredRate;// 码率控制优先选择 0/1=祯率/图像质量 默认1图像质量
	char dummy[5];
	char subVideoFormat;//子视频图像格式
	char subVideoFramerate;
	char subIFrameInterval;
	short subMaxRate;
	short subTargetRate;
	char subDummy[5];
	char subChannelEnable;//通道使能1表示关闭使能，其他值表示使能
	char subStream;//0传输/1存储
	char centerStorage;//中心存储0是不启用，1是启用
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
struct PlayFileInfo
{
	std::string fileName;
	int fileSize;
	int GetFileSizeKB(){return fileSize/1000;}
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
	enum MediaType
	{
		Media_None = 0x00,
		Media_Vedio = 0x01,
		Media_Listen = 0x02,
		Media_Talk = 0x04
	};
public:
	CMediaSocket(void);
	virtual ~CMediaSocket(void);
	virtual bool Init();
	int  GetMediaSvrType(){return m_SvrType;}
	bool ConnectToServer(CString severAddr, CString serverPort);
	bool ConnectToServer(int serverAddr, int serverPort, int svrType = 0, int clientID = 0);
	bool ConnectToServer();
	virtual void CloseConnect();
	int ReqestMediaData(int cameraID,int mediaType);
	int RemoteRecordPlay(int cameraID,int fileNo);
	int StartRecord(const char * fileName);
	int StartRecord(int cameraID,int fileNo=-1);
	int StopRecord();
	bool IsRecording();
	int GetRecordFileList(int cameraID,int startTime,int endTime,int fileType,std::vector<RecordFileInfo> & fileInfoList);
	int PTZControl(int cameraID, BYTE ctrlType ,BYTE speed );
	int GetDevWifiAPList(int cameraID);
	int SetDevWifi(int cameraID,int apListNum,KEDevWifiStartReq wifiStart);
	int GetVideoParam(int cameraID,KEVedioParam & videoParam);
	int SetVideoParam(int cameraID,const KEVedioParam & videoParam);
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
	virtual void OnDataReceived(CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr);
protected:
	int ReqestMediaTrans( int videoID, int channelNo, int mediaType);
	int ReqestVideoServer(int videoID, int channelNo, int mediaType);
	int RequestRecordPlayData(int cameraID,int fileNo);
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
	void RecvGetVideoParam(const BYTE * msgData);
	void RecvSetVideoParam(const BYTE * msgData);
protected:
	
	bool toPlay;
	CMyAVPlayer * m_AVPlayer;
	int m_clientID;
	CEvent mediaEvent;
	int respData;
	int m_videoID;
	int m_channelNo;
	int m_SvrType;//1,视频服务器；2，媒体服务器 
	DevPTZInfo ptzInfo;
	
public:
	int m_mediaType;
	CRecorder * m_Recorder;
	std::vector<RecordFileInfo> recordFileList;
	std::vector<KEDevAPListItem> APList;
	
	std::list<int> downloadFileNoList;
	std::list<PlayFileInfo> PlayFileInfoList;

	int totalFile;
	int downloaded;
	int playedFile;
	KEVedioParam videoParam;
public:
	static std::map<int ,CMediaSocket*> videoSvrMediaMap;
	static CMediaSocket * GetVideoSvrMedia(int videoSvrID);
	static void DelVideoSvrMedia(int videoSvrID);
	
};


#pragma pack(1)
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
	char startTime[6];//无用
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

struct KEDevEncryptKeyResp{
	KEDevMsgViaClient head;
	char keyt[8];
};

struct KEDevLoginReq{
	KEDevMsgViaClient head;
	char md5Str[16];
};

struct KEVideoParamMsg{
	KEDevMsgCommonReq head;
	KEVedioParam param;
};
#pragma pack()