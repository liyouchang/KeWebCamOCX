// KeWebCamOCXCtrl.cpp : CKeWebCamOCXCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "KeWebCamOCXCtrl.h"
#include "KeWebCamOCXPropPage.h"
#include <json/json.h>
#include "PopupPannel.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CKeWebCamOCXApp theApp;

IMPLEMENT_DYNCREATE(CKeWebCamOCXCtrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CKeWebCamOCXCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_RTVIDEOSTOP, &CKeWebCamOCXCtrl::OnRTVideoStop)
	ON_MESSAGE(WM_CAMSTATUSREPORT, &CKeWebCamOCXCtrl::OnCamStatusReport)
	ON_MESSAGE(WM_HEARTBEATSTOP, &CKeWebCamOCXCtrl::OnHeartbeatStop)
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CKeWebCamOCXCtrl, COleControl)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "SetDivision", dispidSetDivision, SetDivision, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "QueryUserCamera", dispidQueryUserCamera, QueryUserCamera, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "StartRealTimeVideo", dispidStartRealTimeVideo, StartRealTimeVideo, VT_BSTR, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "StopRealTimeVideo", dispidStopRealTimeVideo, StopRealTimeVideo, VT_BSTR, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "ControlPTZ", dispidControlPTZ, ControlPTZ, VT_BSTR, VTS_I4 VTS_UI1 VTS_UI1 VTS_UI1)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "LoginServer", dispidLoginServer, LoginServer, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CKeWebCamOCXCtrl, "SnapFilePath", dispidSnapFilePath, m_SnapFilePath, OnSnapFilePathChanged, VT_BSTR)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "TakeSnapshot", dispidTakeSnapshot, TakeSnapshot, VT_BSTR, VTS_I4)
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CKeWebCamOCXCtrl, COleControl)
	EVENT_CUSTOM_ID("HeartBeatStop", eventidHeartBeatStop, HeartBeatStop, VTS_BSTR)
	EVENT_CUSTOM_ID("ReportCameraStatus", eventidReportCameraStatus, ReportCameraStatus, VTS_BSTR)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CKeWebCamOCXCtrl, 1)
	PROPPAGEID(CKeWebCamOCXPropPage::guid)
END_PROPPAGEIDS(CKeWebCamOCXCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CKeWebCamOCXCtrl, "KEWEBCAMOCX.KeWebCamOCXCtrl.1",
	0xf4418f4b, 0x4a6b, 0x4a93, 0x94, 0x8f, 0x33, 0x20, 0x25, 0xf3, 0x95, 0xe8)



// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CKeWebCamOCXCtrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

const IID BASED_CODE IID_DKeWebCamOCX =
		{ 0xDDC0C3D6, 0xE09B, 0x493D, { 0x8C, 0x6C, 0x12, 0xA9, 0x91, 0x6C, 0x60, 0xE1 } };
const IID BASED_CODE IID_DKeWebCamOCXEvents =
		{ 0xF4C17E60, 0xA7C0, 0x44FE, { 0x98, 0xB3, 0xD7, 0xDE, 0xAC, 0xE5, 0x2, 0x52 } };



// �ؼ�������Ϣ

static const DWORD BASED_CODE _dwKeWebCamOCXOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CKeWebCamOCXCtrl, IDS_KEWEBCAMOCX, _dwKeWebCamOCXOleMisc)



// CKeWebCamOCXCtrl::CKeWebCamOCXCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CKeWebCamOCXCtrl ��ϵͳע�����

BOOL CKeWebCamOCXCtrl::CKeWebCamOCXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_KEWEBCAMOCX,
			IDB_KEWEBCAMOCX,
			afxRegApartmentThreading,
			_dwKeWebCamOCXOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// ��Ȩ�ַ���

static const TCHAR BASED_CODE _szLicFileName[] = _T("KeWebCamOCX.lic");

static const WCHAR BASED_CODE _szLicString[] =
	L"Copyright (c) 2013 ";



// CKeWebCamOCXCtrl::CKeWebCamOCXCtrlFactory::VerifyUserLicense -
// ����Ƿ�����û����֤

BOOL CKeWebCamOCXCtrl::CKeWebCamOCXCtrlFactory::VerifyUserLicense()
{
	return AfxVerifyLicFile(AfxGetInstanceHandle(), _szLicFileName,
		_szLicString);
}



// CKeWebCamOCXCtrl::CKeWebCamOCXCtrlFactory::GetLicenseKey -
// ��������ʱ��Ȩ��Կ

BOOL CKeWebCamOCXCtrl::CKeWebCamOCXCtrlFactory::GetLicenseKey(DWORD dwReserved,
	BSTR FAR* pbstrKey)
{
	if (pbstrKey == NULL)
		return FALSE;

	*pbstrKey = SysAllocString(_szLicString);
	return (*pbstrKey != NULL);
}



// CKeWebCamOCXCtrl::CKeWebCamOCXCtrl - ���캯��

CKeWebCamOCXCtrl::CKeWebCamOCXCtrl()
{
	InitializeIIDs(&IID_DKeWebCamOCX, &IID_DKeWebCamOCXEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
	m_bFullScreen = FALSE;
	
}



// CKeWebCamOCXCtrl::~CKeWebCamOCXCtrl - ��������

CKeWebCamOCXCtrl::~CKeWebCamOCXCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
}



// CKeWebCamOCXCtrl::OnDraw - ��ͼ����

void CKeWebCamOCXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
	//pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//pdc->Ellipse(rcBounds);
	if (!m_bFullScreen)
	{
		m_pannel.MoveWindow(rcBounds,TRUE);
	}
	else
	{
		//m_pannel.MoveWindow(m_FullScreenRect,TRUE);
	}
	
	

}



// CKeWebCamOCXCtrl::DoPropExchange - �־���֧��

void CKeWebCamOCXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}



// CKeWebCamOCXCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CKeWebCamOCXCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��

}



// CKeWebCamOCXCtrl ��Ϣ�������

int CKeWebCamOCXCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	theApp.g_pMainWnd = this;
	InitLogModule();
	m_pannel.Create(IDD_DIALOG_PANNEL,this);
	theApp.g_PlayWnd = &m_pannel;
	CSocketHandle::InitLibrary( MAKEWORD(2,2) );
	theApp.g_cmdSocket = new CCmdSocket;
	theApp.g_cmdSocket->Init();

	m_SnapFilePath = _T("d:\\SNAP\\");

	return 0;
}

void CKeWebCamOCXCtrl::SetDivision(LONG nDivision)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	// TODO: �ڴ���ӵ��ȴ���������
	m_pannel.SetPlayDivision(nDivision);
}


BSTR CKeWebCamOCXCtrl::QueryUserCamera(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: �ڴ���ӵ��ȴ���������.
	using namespace std;

	std::string strValue = "{\"name\":\"json\",\"array\":[{\"cpp\":\"jsoncpp\"},{\"java\":\"jsoninjava\"},{\"php\":\"support\"}]}";

	Json::Value root;
	Json::Value arrayObj;
	Json::Value item;

	item["cpp"] ="jsoncpp";
	item["java"] = "jsoninjava";
	item["php"] = "support";
	arrayObj.append(item);

	root["name"] = "json";
	root["array"] = arrayObj;

	root.toStyledString();
	std::string out = root.toStyledString();
	//std::cout << out << std::endl;

	strResult = out.c_str();

	return strResult.AllocSysString();
}



BSTR CKeWebCamOCXCtrl::StartRealTimeVideo(LONG videoID, LONG channelNo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	int isPlaying;
	int ret = 0;
	int cameraID = MakeCameraID(videoID,channelNo);
	COneCamera * pCamera = theApp.g_PlayWnd->GetOnePlayer(cameraID,&isPlaying);
	if (isPlaying == 0)
	{
		if (pCamera->IsPlaying())
		{
			pCamera->StopRTPlay(true);
		}
		ret = pCamera->StartPlay();
	}

	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = GetKEErrorDescriptA(ret);
	std::string out = root.toStyledString();
	strResult = out.c_str();

	return strResult.AllocSysString();
}

BSTR CKeWebCamOCXCtrl::StopRealTimeVideo(LONG videoID, LONG channelNo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	
	// �ڴ���ӵ��ȴ���������
	int cameraID = MakeCameraID(videoID,channelNo);
	//COneCamera * pCamera = theApp.g_PlayWnd->GetOnePlayer(cameraID);
	//pCamera->StopRTPlay(false);
	//OnRTVideoStop(cameraID,KE_SUCCESS);
	this->PostMessage(WM_RTVIDEOSTOP,cameraID,0);
	strResult="0";
	return strResult.AllocSysString();
}

void CKeWebCamOCXCtrl::OnDestroy()
{
	COleControl::OnDestroy();

	//  �ڴ˴������Ϣ����������
	if (theApp.g_cmdSocket != NULL)
	{
		delete theApp.g_cmdSocket;
	}
}

void CKeWebCamOCXCtrl::OnSetClientSite()
{
	// TODO: �ڴ����ר�ô����/����û���

	COleControl::OnSetClientSite();
}

LRESULT CKeWebCamOCXCtrl::OnRTVideoStop( WPARAM wParam, LPARAM lParam )
{
	int cameraID = (int)wParam;
	COneCamera * camera = m_pannel.GetOnePlayer(cameraID);
	camera->StopRTPlay();
	int errorCode = (int)lParam;

	Json::Value root;
	root["reportType"] = 2;
	root["cameraID"] = cameraID;
	root["retValue"] = errorCode;
	root["retDes"] = GetKEErrorDescriptA(errorCode);
	std::string out = root.toStyledString();
	std::wstring wout = str_to_wstr(out);
	//CString	strResult = 
	
	//�����¼�
	ReportCameraStatus(wout.c_str());
	//FireRTVideoStop(strResult);
	return 0;
}

LRESULT CKeWebCamOCXCtrl::OnHeartbeatStop( WPARAM wParam, LPARAM lParam )
{
	return 0;
}

BSTR CKeWebCamOCXCtrl::ControlPTZ(LONG cameraID, BYTE PTZCmd, BYTE iSpeed, BYTE iData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	int toCtrlID = 0;
	if (cameraID == 0)
	{
		toCtrlID = m_pannel.m_camarray[m_pannel.m_nActiveCamera].m_cameraID;
	}
	else
	{
		toCtrlID = cameraID;
	}
	//  �ڴ���ӵ��ȴ���������
	int ret = theApp.g_cmdSocket->SendPTZControlMsg(toCtrlID,PTZCmd,iSpeed,iData);
	TRACE1("SendPTZControlMsg %d!\n",PTZCmd);
	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = GetKEErrorDescriptA(ret);
	std::string out = root.toStyledString();
	strResult = out.c_str();

	return strResult.AllocSysString();
}

BSTR CKeWebCamOCXCtrl::LoginServer(LPCTSTR userName, LPCTSTR password, LPCTSTR svrIpAddr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	int ret = 0;
	if (!theApp.g_cmdSocket->IsConnect())
	{
		if (theApp.g_cmdSocket->ConnectToServer(svrIpAddr))
		{
			ret = theApp.g_cmdSocket->LoginServer(userName,password);	
		}
		else
		{
			ret = KE_CONNECT_SERVER_ERROR;
		}
	}
	else
	{
		ret = theApp.g_cmdSocket->LoginServer(userName,password);	
	}
	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = GetKEErrorDescriptA(ret);
	root["clientID"] = theApp.g_cmdSocket->GetClientID();
	std::string out = root.toStyledString();
	strResult = out.c_str();

	return strResult.AllocSysString();
}

void CKeWebCamOCXCtrl::OnSnapFilePathChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������

	SetModifiedFlag();
}

LRESULT CKeWebCamOCXCtrl::OnCamStatusReport( WPARAM wParam, LPARAM lParam )
{
	CamStatusReport * report = (CamStatusReport *)lParam;
	Json::Value root;
	root["reportType"] = report->reportType;
	root["cameraID"] = report->cameraID;
	root["retValue"] = report->errorCode;
	root["retDes"] = GetKEErrorDescriptA(report->errorCode);

	std::string out = root.toStyledString();
	std::wstring wout = str_to_wstr(out);

	//�����¼�
	ReportCameraStatus(wout.c_str());
	return 0;
}

BSTR CKeWebCamOCXCtrl::TakeSnapshot(LONG nCameraID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	int ret = 0;
	COneCamera * pCamera = m_pannel.GetCamera(nCameraID);
	CString fullPath;
	if (pCamera == NULL)
	{
		ret = KE_FAILED;
	}
	else
	{
		SYSTEMTIME sys;
		GetLocalTime( &sys );
		//��ͷid+������ʱ����+����
		CString fileName ;
		fileName.Format(_T("%d_%d%d%d%d%d%d%d.bmp"), pCamera->m_cameraID,
			sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
		CString filePath;
		if (m_SnapFilePath.GetAt(m_SnapFilePath.GetLength()-1)!=_T('\\'))
		{
			m_SnapFilePath.AppendChar(_T('\\'));
		}
		filePath.Format(_T("%s%d\\"),m_SnapFilePath,pCamera->m_cameraID);
		
		if (!FolderExist(filePath))
		{
			CreateFolderEx(filePath);
		}

		fullPath = filePath + fileName;
		ret = pCamera->m_AVIPlayer->CapPic(fullPath.GetBuffer(0));
		if (ret != 0)
		{
			ret = KE_FAILED;
		}
		ret = KE_SUCCESS;
	}

	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = GetKEErrorDescriptA(ret);
    root["filePath"] = wstr_to_str(fullPath.GetString());
	std::string out = root.toStyledString();
	strResult = out.c_str();

	return strResult.AllocSysString();
}

void CKeWebCamOCXCtrl::ShowCamPannel( BOOL bFull /*= FALSE*/ )
{
	CRect rect;
	if (!bFull)
	{
		GetClientRect(rect);
		GetWindowRect(rect);
	} 
	else
	{
		m_bFullScreen=TRUE;   //����ȫ����ʾ��־ΪTRUE
		//CPopupPannel dlg;
		//m_pannel.SetParent(&dlg);
		//dlg.DoModal();

		//m_pannel.MoveWindow(0,0,100,100,TRUE);
		//m_pannel.SetParent(this);
// 		GetClientRect(m_rcRect);
// 		GetWindowPlacement(&m_OldWndPlacement);     
// 	
// 		//m_OldWndParent = ::GetParent(m_hWnd);
// 		//::SetParent(m_pannel.m_hWnd,::GetDesktopWindow() );
// 	

		//����ȫ����ʾ״̬   
//  		WINDOWPLACEMENT wndpl;
// 		ZeroMemory(&wndpl, sizeof(WINDOWPLACEMENT));
//  		wndpl.length=sizeof(WINDOWPLACEMENT);     
//  		wndpl.flags=0;     
//  		wndpl.showCmd=SW_SHOWNORMAL;     
//  		wndpl.rcNormalPosition=m_FullScreenRect;     
// 		SetWindowPlacement(&wndpl);
		//::SetForegroundWindow(::GetDesktopWindow());
		//m_pannel.SetForegroundWindow();
 		
 		//::SetForegroundWindow(m_hWnd);
	}
}
