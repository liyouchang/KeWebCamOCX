#pragma once
#include "socketthreadhandler.h"
#include "MyAVPlayer.h"

enum MediaType
{
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
	bool ConnectToServer(int serverAddr,int serverPort);
	int ReqestMediaTrans(int clientID,int videoID,int channelNo,int mediaType);
protected:
	virtual void HandleMessage(const BYTE* msgData);
	void RecvMediaTransResp(const BYTE* msgData);
	CMyAVPlayer * m_AVPlayer;
	int m_clientID;
	CEvent mediaEvent;
	int respData;
};
