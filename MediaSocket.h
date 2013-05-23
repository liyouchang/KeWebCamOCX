#pragma once
#include "socketthreadhandler.h"
#include "MyAVPlayer.h"
#include "Recorder.h"
#include <queue>
#include <vector>

enum MediaType
{
	Media_None = 0x00,
	Media_Vedio = 0x01,
	Media_Listen = 0x02,
	Media_Talk = 0x04
};


enum MediaMsgType
{
	KEMSG_RecordFileList = 0x53,
	KEMSG_REQUEST_DOWNLOAD_FILE = 0x54,
	KEMSG_RecordPlayData = 0x55,

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
	char video;//��Ƶ=0����   =1 ֹͣ
	char listen;//����=0����   =1 ֹͣ
	char talk;// �Խ�=0����   =1 ֹͣ
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
	char fileType;//0 ����
	int fileNo;//1 ����
	KETimeStruct startTime;//����
	int clientIp;//0�򱾻�ip
	char protocalType;//1 ����
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
	char resp;//13 ���������� ��6 ���ݽ���
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
	char fileType;//1�ƻ�¼��2��ʾ������������	3�ƶ���ⱨ����4ץ����Ƭ��
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
	int fileNo;
	char startTime[6];
	char endTime[6];
	int fileSize;
	char data[80];
}KERecordFileListResp,*PKERecordFileListResp;

#pragma pack()

struct RecordFileInfo
{
	int fileNo;
	int startTime;
	int endTime;
	int fileSize;
	char fileData[80];
};


class AudioTalkThread:public SimpleThreadBase
{
public:
	AudioTalkThread();
	virtual ~AudioTalkThread();
	void Initialize(CSocketThreadHandler * socketHandle,int cameraID);
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
	
protected:
	int ReqestMediaTrans( int videoID, int channelNo, int mediaType);
	int ReqestVideoServer(int videoID, int channelNo, int mediaType);
	
	virtual bool CheckMessage(const BYTE* data, DWORD dwCount);
	virtual void HandleMessage(const BYTE* msgData);
	virtual void Run();
	virtual int OpenMedia(int cameraID,int mediaType);

	void RecvMediaTransResp(const BYTE* msgData);
	void RecvVideoStream(const BYTE * msgData);
	void RecvVideoServer(const BYTE *msgData);
	void RecvAudioStream(const BYTE *msgData);
	void RecvRecordFileList(const BYTE * msgData);
	void RecvRecordPlayData(const BYTE * msgData);
protected:
	CRecorder * m_Recorder;
	CMyAVPlayer * m_AVPlayer;
	int m_clientID;
	CEvent mediaEvent;
	int respData;
	int m_videoID;
	int m_channelNo;
	int m_SvrType;//1,��Ƶ��������2��ý������� 
	
public:
	std::vector<RecordFileInfo> recordFileList;
};
