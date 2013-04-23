#pragma once

// KeWebCamOCX.h : KeWebCamOCX.DLL 的主头文件

#if !defined( __AFXCTL_H__ )
#error "在包括此文件之前包括“afxctl.h”"
#endif

#include "resource.h"       // 主符号
#include "CmdSocket.h"
#include <map>
#include "CommonUtility/LogModule.h"
#include "KeWebCamOCXCtrl.h"
#include "CommonUtility/CommonFunctions.h"
// CKeWebCamOCXApp : 有关实现的信息，请参阅 KeWebCamOCX.cpp。

//user message define start lht message from 1029
#define WM_HEARTBEATSTOP (WM_USER+1029)
#define WM_RTVIDEOSTOP (WM_USER+1030)
#define WM_CAMSTATUSREPORT (WM_USER + 1031)
#define WM_TREESTRUCTNOTIFY (WM_USER + 1032)

class CKeWebCamOCXApp : public COleControlModule
{

public:
	BOOL InitInstance();
	int ExitInstance();

public:
	int			m_nMAXCHANNEL;
	int			m_nCurrentMappingCamera;
	BOOL		m_bOneChTransfer;
	CWebCamPannel	*g_PlayWnd;
	CCmdSocket *g_cmdSocket;
	CKeWebCamOCXCtrl * g_pMainWnd;
	//CString theLastError;
	
};




extern CKeWebCamOCXApp theApp;

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

