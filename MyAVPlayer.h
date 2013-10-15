#pragma once

#include "AVPlay.h"
#include <map>
// #include <afxmt.h>
#include "PlayerCtrlDlg.h"

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
	int m_nCamNo;//ӵ�д˲�������Static
	int m_videoID;
	int m_channelNo;

	//�ļ����ŷ���
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
public://��̬����
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
	int m_iPlaySpeed;                       //�����ٶ� 0���������ţ�-5����ͣ
	static int isInitail ;
	//CCriticalSection m_cs; // sinchronicity
	CPlayerCtrlDlg * m_CtrlDlg;
public:

	static int StreamID;
};
