#pragma once

// KeWebCamOCX.h : KeWebCamOCX.DLL ����ͷ�ļ�

#if !defined( __AFXCTL_H__ )
#error "�ڰ������ļ�֮ǰ������afxctl.h��"
#endif

#include "resource.h"       // ������
#include "WebCamPannel.h"

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
	//CWebCamPannel	*m_pMainWnd;
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

