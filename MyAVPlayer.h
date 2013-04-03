#pragma once

#include "AVPlay.h"


//******************************��������״̬*********************************
#define STA_QUESTDATA_FREE                 0								//����״̬
#define STA_QUESTDATA_TOJR                 1								//�����������Ƶͼ���IP״̬
#define STA_CONMEDIASERVER                 2								//��ý��������������ӵ�״̬
#define STA_QUESTDATA_TOMEDIA              3								//��ý�������������Ƶ���ݵ�״̬
#define STA_PLAY                           4								//����״̬
#define STA_QUESTDATA_ACK                  5                                //�����������Ƶͼ���IP�Ļ�Ӧ
#define STA_QUESTDATA_TOMEDIA_ACK          6								//��ý�������������Ƶ���ݵĻ�Ӧ


class CMyAVPlayer
{
public:
	CMyAVPlayer(void);
	virtual ~CMyAVPlayer(void);
	void SetPlayWnd(HWND hWnd);
	int OpenStream(int videoID,int channelNo);
	int InputStream(char * data,int dataLen);
	void PlayFile(HWND hWnd);
private:
	long m_lPlayHandle;
	long  m_lPause;
	HWND m_hPlayWnd;
	int m_PlayStatus;
	int m_videoID;
	int m_channelNo;
};
