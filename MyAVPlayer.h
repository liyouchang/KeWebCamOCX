#pragma once

#include "AVPlay.h"
#include <map>
// #include <afxmt.h>
#include "PlayerCtrlDlg.h"

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
	PLAYSTATUS_PlayStream,
	PLAYSTATUS_PlayFile,
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
	int InputStream(const BYTE * data, int dataLen);
	int CloseStream();
	
	bool IsPlaying();	
	int CapPic(LPCTSTR pFileName);
	int m_nCamNo;//拥有此播放器的Static
	int m_videoID;
	int m_channelNo;

	//文件播放方法
	int PlayFile(const char * fileName,int fileSize = 0,HWND endMsgWnd = NULL);
	//int SetFileEndMsgWnd(HWND fileWnd);
	int SetSpeed(int speed);
	int GetSpeed(){return m_iPlaySpeed;}
	int ChgSpeed(int chgSpeed);
	int SetPlayPos(int pos);
	int GetPlayPos(int &pos);
	int PausePlay();
	int CloseFile();
	int NormalPlay();
	int SetFileSize(long fileSize);
	void SetCtrlDlg(CPlayerCtrlDlg * dlg);
	float GetPlayFluxRate();
	int GetPlayFrameRate();
	int GetEncodeFormat();
	CString GetEncodeFormatName();
public://静态方法
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
	int m_iPlaySpeed;                       //播放速度 0、正常播放；-5、暂停
	static int isInitail ;
	//CCriticalSection m_cs; // sinchronicity
	CPlayerCtrlDlg * m_CtrlDlg;
public:

	static int StreamID;
};
