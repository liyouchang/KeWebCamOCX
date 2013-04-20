#pragma once
#include "socketthreadhandler.h"
#include "MyAVPlayer.h"
#include <queue>
enum MediaType
{
	Media_None = 0x00,
	Media_Vedio = 0x01,
	Media_Listen = 0x02,
	Media_Talk = 0x04
};

class CMediaSocket :
	public CSocketThreadHandler
{
public:
	CMediaSocket(void);
	virtual ~CMediaSocket(void);
	virtual bool Init();
	bool ConnectToServer(CString severAddr, CString serverPort);
	bool ConnectToServer(int serverAddr, int serverPort);
	virtual void CloseConnect();
	int ReqestMediaTrans(int clientID, int videoID, int channelNo, int mediaType);
	int ReqestVideoServer(int clientID,int videoID,int channelNo,int mediaType);
protected:
	virtual bool CheckMessage(const BYTE* data, DWORD dwCount);
	virtual void HandleMessage(const BYTE* msgData);
	virtual void Run();
	void RecvMediaTransResp(const BYTE* msgData);
	void RecvVideoStream(const BYTE * msgData);
	void RecvVideoServer(const BYTE *msgData);
	CMyAVPlayer * m_AVPlayer;
	int m_clientID;
	CEvent mediaEvent;
	int respData;
	int m_videoID;
	int m_channelNo;
public:
// 	static std::map<int,CMediaSocket *> g_MediaMap;
// 	//static int GetMediaIndex(int videoID,int channelNo);
// 	static CMediaSocket * GetMediaSocket(int videoID,int channelNo,bool noCreate = false);
// 	static void SetMediaSocket(int videoID,int channelNo,CMediaSocket * mediaSocket);
	
};
