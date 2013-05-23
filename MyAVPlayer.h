#pragma once

#include "AVPlay.h"
#include <map>
// #include <afxmt.h>

//******************************请求数据状态*********************************
#define STA_QUESTDATA_FREE                 0								//空闲状态
#define STA_QUESTDATA_TOJR                 1								//向接入请求视频图像的IP状态
#define STA_CONMEDIASERVER                 2								//向媒体服务器发起连接的状态
#define STA_QUESTDATA_TOMEDIA              3								//向媒体服务器请求视频数据的状态
#define STA_PLAY                           4								//播放状态
#define STA_QUESTDATA_ACK                  5                                //向接入请求视频图像的IP的回应
#define STA_QUESTDATA_TOMEDIA_ACK          6								//向媒体服务器请求视频数据的回应

enum PlayStatus
{
	PLAYSTATUS_Free = 0,
	PLAYSTATUS_Play,
};


class CMyAVPlayer
{
public:
	CMyAVPlayer(void);
	virtual ~CMyAVPlayer(void);
	void SetPlayWnd(HWND hWnd);
	virtual int OpenStream();
	virtual int OpenSound();
	virtual int CloseSound();
	int InputStream(const BYTE * data,int dataLen);
	int CloseStream();
	void PlayFile(HWND hWnd);
	bool IsPlaying();	
	int CapPic(LPCTSTR pFileName);
	int m_nCamNo;//拥有此播放器的Static
	int m_videoID;
	int m_channelNo;
	static int StartTalk();
	static int StopTalk();
	static int GetAudioData(char * pBuf);
	static int InitPlayer();
	static int FreePlayer();
private:
	long m_lPlayHandle;
	long  m_lPause;
	HWND m_hPlayWnd;
	int m_PlayStatus;

	static int isInitail ;
	//CCriticalSection m_cs; // sinchronicity
public:

	static int StreamID;
};
