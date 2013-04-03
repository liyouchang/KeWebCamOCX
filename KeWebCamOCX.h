#pragma once

// KeWebCamOCX.h : KeWebCamOCX.DLL ����ͷ�ļ�

#if !defined( __AFXCTL_H__ )
#error "�ڰ������ļ�֮ǰ������afxctl.h��"
#endif

#include "resource.h"       // ������
#include "WebCamPannel.h"
#include "CmdSocket.h"
#include "MediaSocket.h"
#include <map>
#include "CommonUtility/LogModule.h"

// CKeWebCamOCXApp : �й�ʵ�ֵ���Ϣ������� KeWebCamOCX.cpp��

class CKeWebCamOCXApp : public COleControlModule
{

public:
	BOOL InitInstance();
	int ExitInstance();

public:
	int			m_nMAXCHANNEL;
	int			m_nCurrentMappingCamera;
	BOOL		m_bOneChTransfer;
	BOOL		m_bConnectOK;
	CWebCamPannel	*g_PlayWnd;
	std::map<int,CMediaSocket *> g_mediaMap;
	int SetMediaSocket(int videoID,int channelNo,CMediaSocket * media);
	CMediaSocket * GetMediaSocket(int videoID,int channelNo);
	
	CCmdSocket m_cmdSocket;
};
extern CKeWebCamOCXApp theApp;

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

