#pragma once

#include "AVPlay.h"
#include <map>
// #include <afxmt.h>

//******************************��������״̬*********************************
#define STA_QUESTDATA_FREE                 0								//����״̬
#define STA_QUESTDATA_TOJR                 1								//�����������Ƶͼ���IP״̬
#define STA_CONMEDIASERVER                 2								//��ý��������������ӵ�״̬
#define STA_QUESTDATA_TOMEDIA              3								//��ý�������������Ƶ���ݵ�״̬
#define STA_PLAY                           4								//����״̬
#define STA_QUESTDATA_ACK                  5                                //�����������Ƶͼ���IP�Ļ�Ӧ
#define STA_QUESTDATA_TOMEDIA_ACK          6								//��ý�������������Ƶ���ݵĻ�Ӧ

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
	int m_nCamNo;//ӵ�д˲�������Static
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
