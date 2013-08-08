#pragma once

// KeWebCamOCX.h : KeWebCamOCX.DLL ����ͷ�ļ�

#if !defined( __AFXCTL_H__ )
#error "�ڰ������ļ�֮ǰ������afxctl.h��"
#endif

#include "resource.h"       // ������
//#include "CmdSocket.h"
#include "CenterCommand.h"
#include <map>
#include "CommonUtility/LogModule.h"
#include "KeWebCamOCXCtrl.h"
#include "CommonUtility/CommonFunctions.h"
// CKeWebCamOCXApp : �й�ʵ�ֵ���Ϣ������� KeWebCamOCX.cpp��

//user message define start lht message from 1029


class CKeWebCamOCXApp : public COleControlModule
{

public:
	BOOL InitInstance();
	int ExitInstance();


public:
	int			m_nMAXCHANNEL;
	BOOL		m_bOneChTransfer;
	CWebCamPannel	*g_PlayWnd;
	//CCmdSocket *g_cmdSocket;
	CKeWebCamOCXCtrl * g_pMainWnd;
	CenterCommand * g_cmd;
	//CString theLastError;
	tstd::tstring iniFile;
	
};

extern CKeWebCamOCXApp theApp;

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

