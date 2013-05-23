// KeWebCamOCXCtrl.cpp : CKeWebCamOCXCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "KeWebCamOCXCtrl.h"
#include "KeWebCamOCXPropPage.h"
#include <json/json.h>
#include "PopupPannel.h"
#include "CommonUtility/tstdlibs.h"
#include "Communication/P2PCmdSocket.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CKeWebCamOCXApp theApp;

IMPLEMENT_DYNCREATE(CKeWebCamOCXCtrl, COleControl)



// 消息映射

BEGIN_MESSAGE_MAP(CKeWebCamOCXCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_RTVIDEOSTOP, &CKeWebCamOCXCtrl::OnRTVideoStop)
	ON_MESSAGE(WM_CAMSTATUSREPORT, &CKeWebCamOCXCtrl::OnCamStatusReport)
	ON_MESSAGE(WM_HEARTBEATSTOP, &CKeWebCamOCXCtrl::OnHeartbeatStop)
	ON_MESSAGE(WM_TREESTRUCTNOTIFY, &CKeWebCamOCXCtrl::OnTreeStructNotify)
	ON_WM_TIMER()
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CKeWebCamOCXCtrl, COleControl)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "SetDivision", dispidSetDivision, SetDivision, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "QueryUserCamera", dispidQueryUserCamera, QueryUserCamera, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "StartRealTimeVideo", dispidStartRealTimeVideo, StartRealTimeVideo, VT_BSTR, VTS_I4 )
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "StopRealTimeVideo", dispidStopRealTimeVideo, StopRealTimeVideo, VT_BSTR, VTS_I4 )
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "ControlPTZ", dispidControlPTZ, ControlPTZ, VT_BSTR, VTS_I4 VTS_UI1 VTS_UI1 VTS_UI1)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "LoginServer", dispidLoginServer, LoginServer, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CKeWebCamOCXCtrl, "SnapFilePath", dispidSnapFilePath, m_SnapFilePath, OnSnapFilePathChanged, VT_BSTR)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "TakeSnapshot", dispidTakeSnapshot, TakeSnapshot, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "StartRealTimeAudio", dispidStartRealTimeAudio, StartRealTimeAudio, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "StopRealTimeAudio", dispidStopRealTimeAudio, StopRealTimeAudio, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "StartAudioTalk", dispidStartAudioTalk, StartAudioTalk, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "StopAudioTalk", dispidStopAudioTalk, StopAudioTalk, VT_BSTR, VTS_I4)
	DISP_PROPERTY_NOTIFY_ID(CKeWebCamOCXCtrl, "RecordFilePath", dispidRecordFilePath, m_RecordFilePath, OnRecordFilePathChanged, VT_BSTR)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "StartRecord", dispidStartRecord, StartRecord, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "StopRecord", dispidStopRecord, StopRecord, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "InitailCtrl", dispidInitailCtrl, InitailCtrl, VT_BSTR, VTS_I4 )
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "ConnectServer", dispidConnectServer, ConnectServer, VT_BSTR, VTS_BSTR VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "QueryRecordFileList", dispidQueryRecordFileList, QueryRecordFileList, VT_BSTR, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CKeWebCamOCXCtrl, COleControl)
	EVENT_CUSTOM_ID("HeartBeatStop", eventidHeartBeatStop, HeartBeatStop, VTS_BSTR)
	EVENT_CUSTOM_ID("ReportCameraStatus", eventidReportCameraStatus, ReportCameraStatus, VTS_BSTR)
	EVENT_CUSTOM_ID("TreeStructNotify", eventidTreeStructNotify, TreeStructNotify, VTS_BSTR)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CKeWebCamOCXCtrl, 1)
	PROPPAGEID(CKeWebCamOCXPropPage::guid)
END_PROPPAGEIDS(CKeWebCamOCXCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CKeWebCamOCXCtrl, "KEWEBCAMOCX.KeWebCamOCXCtrl.1",
	0xf4418f4b, 0x4a6b, 0x4a93, 0x94, 0x8f, 0x33, 0x20, 0x25, 0xf3, 0x95, 0xe8)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CKeWebCamOCXCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID BASED_CODE IID_DKeWebCamOCX =
		{ 0xDDC0C3D6, 0xE09B, 0x493D, { 0x8C, 0x6C, 0x12, 0xA9, 0x91, 0x6C, 0x60, 0xE1 } };
const IID BASED_CODE IID_DKeWebCamOCXEvents =
		{ 0xF4C17E60, 0xA7C0, 0x44FE, { 0x98, 0xB3, 0xD7, 0xDE, 0xAC, 0xE5, 0x2, 0x52 } };



// 控件类型信息

static const DWORD BASED_CODE _dwKeWebCamOCXOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CKeWebCamOCXCtrl, IDS_KEWEBCAMOCX, _dwKeWebCamOCXOleMisc)



// CKeWebCamOCXCtrl::CKeWebCamOCXCtrlFactory::UpdateRegistry -
// 添加或移除 CKeWebCamOCXCtrl 的系统注册表项

BOOL CKeWebCamOCXCtrl::CKeWebCamOCXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

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



// 授权字符串

static const TCHAR BASED_CODE _szLicFileName[] = _T("KeWebCamOCX.lic");

static const WCHAR BASED_CODE _szLicString[] =
	L"Copyright (c) 2013 ";



// CKeWebCamOCXCtrl::CKeWebCamOCXCtrlFactory::VerifyUserLicense -
// 检查是否存在用户许可证

BOOL CKeWebCamOCXCtrl::CKeWebCamOCXCtrlFactory::VerifyUserLicense()
{
	return AfxVerifyLicFile(AfxGetInstanceHandle(), _szLicFileName,
		_szLicString);
}



// CKeWebCamOCXCtrl::CKeWebCamOCXCtrlFactory::GetLicenseKey -
// 返回运行时授权密钥

BOOL CKeWebCamOCXCtrl::CKeWebCamOCXCtrlFactory::GetLicenseKey(DWORD dwReserved,
	BSTR FAR* pbstrKey)
{
	if (pbstrKey == NULL)
		return FALSE;

	*pbstrKey = SysAllocString(_szLicString);
	return (*pbstrKey != NULL);
}



// CKeWebCamOCXCtrl::CKeWebCamOCXCtrl - 构造函数

CKeWebCamOCXCtrl::CKeWebCamOCXCtrl()
{
	InitializeIIDs(&IID_DKeWebCamOCX, &IID_DKeWebCamOCXEvents);
	//  在此初始化控件的实例数据。
	InitLogModule();
	CSocketHandle::InitLibrary( MAKEWORD(2,2) );
	LOG_DEBUG("CKeWebCamOCXCtrl::CKeWebCamOCXCtrl()");
}



// CKeWebCamOCXCtrl::~CKeWebCamOCXCtrl - 析构函数

CKeWebCamOCXCtrl::~CKeWebCamOCXCtrl()
{
	// 在此清理控件的实例数据。
	LOG_DEBUG("CKeWebCamOCXCtrl::~CKeWebCamOCXCtrl()\n");

}



// CKeWebCamOCXCtrl::OnDraw - 绘图函数

void CKeWebCamOCXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
	//pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//pdc->Ellipse(rcBounds);
	m_pannel.MoveWindow(rcBounds,TRUE);
}



// CKeWebCamOCXCtrl::DoPropExchange - 持久性支持

void CKeWebCamOCXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}



// CKeWebCamOCXCtrl::OnResetState - 将控件重置为默认状态

void CKeWebCamOCXCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。

}



// CKeWebCamOCXCtrl 消息处理程序

int CKeWebCamOCXCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	theApp.g_pMainWnd = this;
	m_pannel.Create(IDD_DIALOG_PANNEL,this);
	theApp.g_PlayWnd = &m_pannel;
	CMyAVPlayer::InitPlayer();
	//theApp.g_cmdSocket = new CCmdSocket;
	//theApp.g_cmdSocket = &m_socketSvr;
	//theApp.g_cmdSocket->Init();

	m_SnapFilePath = _T("c:\\SNAP\\");
	m_RecordFilePath = _T("D:\\Record\\");

	LOG_DEBUG("CKeWebCamOCXCtrl::OnCreate end!");
	return 0;
}

void CKeWebCamOCXCtrl::SetDivision(LONG nDivision)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	// TODO: 在此添加调度处理程序代码
	theApp.g_PlayWnd->SetPlayDivision(nDivision);
}


BSTR CKeWebCamOCXCtrl::QueryUserCamera(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码.
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



BSTR CKeWebCamOCXCtrl::StartRealTimeVideo(LONG nCameraID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	int isPlaying;
	int ret = KE_SUCCESS;
	COneCamera * pCamera = theApp.g_PlayWnd->GetOnePlayer(nCameraID,&isPlaying);
	if (isPlaying == 0)
	{
		if (pCamera->IsPlaying())
		{
			pCamera->StopRTPlay(true);
		}
		ret = pCamera->StartPlay();
	}
//pCamera->StopRTPlay(true);
//ret = pCamera->StartPlay();
	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = GetKEErrorDescriptA(ret);
	std::string out = root.toStyledString();
	strResult = out.c_str();
	return strResult.AllocSysString();
}

BSTR CKeWebCamOCXCtrl::StopRealTimeVideo(LONG nCameraID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	
	// 在此添加调度处理程序代码
	//int cameraID = MakeCameraID(videoID,channelNo);
	//COneCamera * pCamera = theApp.g_PlayWnd->GetOnePlayer(cameraID);
	//pCamera->StopRTPlay(false);
	//OnRTVideoStop(cameraID,KE_SUCCESS);
	this->PostMessage(WM_RTVIDEOSTOP,nCameraID,0);
	strResult="0";
	return strResult.AllocSysString();
}

void CKeWebCamOCXCtrl::OnDestroy()
{
	COleControl::OnDestroy();
	TRACE("CKeWebCamOCXCtrl::OnDestroy()\n");
	//  在此处添加消息处理程序代码
	theApp.g_PlayWnd->StopAllRTPlay();
	CMyAVPlayer::FreePlayer();
	//
	
}

void CKeWebCamOCXCtrl::OnSetClientSite()
{
	// TODO: 在此添加专用代码和/或调用基类

	COleControl::OnSetClientSite();
}

LRESULT CKeWebCamOCXCtrl::OnRTVideoStop( WPARAM wParam, LPARAM lParam )
{
	int cameraID = (int)wParam;
	COneCamera * camera = theApp.g_PlayWnd->GetOnePlayer(cameraID);
	camera->StopRTPlay();
	int errorCode = (int)lParam;

	Json::Value root;
	root["reportType"] = 2;
	root["cameraID"] = cameraID;
	root["retValue"] = errorCode;
	root["retDes"] = theApp.g_cmd->GetErrorDesA(errorCode);
	std::string out = root.toStyledString();
	tstd::tstring tout = str_to_tstr(out);
	//CString	strResult = 
	
	//触发事件
	ReportCameraStatus(tout.c_str());
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
		toCtrlID = theApp.g_PlayWnd->m_camarray[theApp.g_PlayWnd->m_nActiveCamera].m_cameraID;
	}
	else
	{
		toCtrlID = cameraID;
	}
	//  在此添加调度处理程序代码
	int ret = theApp.g_cmd->PTZControl(toCtrlID,PTZCmd,iSpeed,iData);
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
// 	if (!theApp.g_cmdSocket->IsConnect())
// 	{
// 		if (theApp.g_cmdSocket->ConnectToServer(svrIpAddr))
// 		{
// 			ret = theApp.g_cmdSocket->LoginServer(userName,password);	
// 		}
// 		else
// 		{
// 			ret = KE_CONNECT_SERVER_ERROR;
// 		}
// 	}
// 	else
// 	{
// 		ret = theApp.g_cmdSocket->LoginServer(userName,password);	
// 	}
	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = GetKEErrorDescriptA(ret);
	root["clientID"] = theApp.g_cmd->GetClientID();
	std::string out = root.toStyledString();
	strResult = out.c_str();

	return strResult.AllocSysString();
}

void CKeWebCamOCXCtrl::OnSnapFilePathChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码
	
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
	tstd::tstring tout = str_to_tstr(out);

	//触发事件
	ReportCameraStatus(tout.c_str());
	return 0;
}

BSTR CKeWebCamOCXCtrl::TakeSnapshot(LONG nCameraID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	int ret = 0;
	COneCamera * pCamera = theApp.g_PlayWnd->GetCamera(nCameraID);
	CString fullPath;
	if (pCamera == NULL)
	{
		ret = KE_FAILED;
	}
	else
	{
// 		SYSTEMTIME sys;
// 		GetLocalTime( &sys );
// 		//镜头id+年月日时分秒+毫秒
// 		CString fileName ;
// 		fileName.Format(_T("%d_%d%d%d%d%d%d%d.bmp"), pCamera->m_cameraID,
// 			sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
// 		CString filePath;
// 		if (m_SnapFilePath.GetAt(m_SnapFilePath.GetLength()-1)!=_T('\\'))
// 		{
// 			m_SnapFilePath.AppendChar(_T('\\'));
// 		}
// 		filePath.Format(_T("%s%d\\"),m_SnapFilePath,pCamera->m_cameraID);
// 		
// 		if (!FolderExist(filePath))
// 		{
// 			CreateFolderEx(filePath);
// 		}
// 
// 		fullPath = filePath + fileName;
		fullPath = GetCamTimeFileName(pCamera->m_cameraID,_T("bmp"),m_SnapFilePath);
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
	root["filePath"] = tstr_to_str(fullPath.GetString());
	std::string out = root.toStyledString();
	strResult = out.c_str();

	return strResult.AllocSysString();
}

LRESULT CKeWebCamOCXCtrl::OnTreeStructNotify( WPARAM wParam, LPARAM lParam )
{
	int noteType = wParam;
	if(noteType == KEMSG_ASKTREE_DATATYPE_AllRootNodes)
	{
		std::vector<CHNODE> * nodes  = (std::vector<CHNODE> *)lParam;
		std::vector<CHNODE>::iterator nodeIter = nodes->begin();
		for (;nodeIter != nodes->end();nodeIter++)
		{
			CHNODE aNode =*nodeIter ;
			Json::Value root;
			root["OperType"] = KEMSG_ASKTREE_DATATYPE_AllRootNodes;
			root["NodeType"] = aNode.NodeType;
			root["NodeName"] = tstr_to_str(aNode.NodeName);
			root["NodeID"] = aNode.NodeID;
			root["ParentNodeID"]= aNode.ParentNodeID;
			root["onLine"] = aNode.onLine;
			std::string out = root.toStyledString();
			tstd::tstring tout = str_to_tstr(out);
			//触发事件
			TreeStructNotify(tout.c_str());
		}
	}
	return 0;
}

BSTR CKeWebCamOCXCtrl::StartRealTimeAudio(LONG cameraID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	
	int ret = 0;
	COneCamera * pCamera = theApp.g_PlayWnd->GetCamera(cameraID);
	if (pCamera == NULL || !pCamera->IsPlaying())
	{
		ret = KE_RTV_NOPLAY;
	}
	else
	{
		ret = pCamera->m_MediaSocket->ReqestMediaData(pCamera->m_cameraID,Media_Vedio|Media_Listen);
	}
	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = GetKEErrorDescriptA(ret);
	std::string out = root.toStyledString();
	strResult = out.c_str();
	
	return strResult.AllocSysString();
}

BSTR CKeWebCamOCXCtrl::StopRealTimeAudio(LONG cameraID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	
	int ret = 0;
	COneCamera * pCamera = theApp.g_PlayWnd->GetCamera(cameraID);
	if (pCamera == NULL || !pCamera->IsPlaying())
	{
		ret = KE_RTV_NOPLAY;
	}
	else
	{
		pCamera->m_AVIPlayer->CloseSound();
		ret = pCamera->m_MediaSocket->ReqestMediaData(pCamera->m_cameraID,Media_Vedio);
	}
	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = GetKEErrorDescriptA(ret);
	std::string out = root.toStyledString();
	strResult = out.c_str();
	return strResult.AllocSysString();
}

BSTR CKeWebCamOCXCtrl::StartAudioTalk(LONG cameraID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	int ret = 0;
	COneCamera * pCamera = theApp.g_PlayWnd->GetCamera(cameraID);
	if (pCamera == NULL || !pCamera->IsPlaying())
	{
		ret = KE_RTV_NOPLAY;
	}
	else
	{
		ret = pCamera->m_MediaSocket->ReqestMediaData(pCamera->m_cameraID,Media_Vedio|Media_Listen|Media_Talk);
		if (ret == KE_SUCCESS)
		{
			AudioTalkThread::GetInstanse()->Stop();
			AudioTalkThread::GetInstanse()->Initialize(pCamera->m_MediaSocket,cameraID);
			AudioTalkThread::GetInstanse()->Start();
		}
	}
	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = GetKEErrorDescriptA(ret);
	std::string out = root.toStyledString();
	strResult = out.c_str();


	return strResult.AllocSysString();
}

BSTR CKeWebCamOCXCtrl::StopAudioTalk(LONG cameraID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	int ret = 0;
	COneCamera * pCamera = theApp.g_PlayWnd->GetCamera(cameraID);
	if (pCamera == NULL || !pCamera->IsPlaying())
	{
		ret = KE_RTV_NOPLAY;
	}
	else
	{
		pCamera->m_AVIPlayer->CloseSound();
		ret = pCamera->m_MediaSocket->ReqestMediaData(pCamera->m_cameraID,Media_Vedio);
		if (ret == KE_SUCCESS)
		{
			AudioTalkThread::GetInstanse()->Stop();
		}
	}
	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = GetKEErrorDescriptA(ret);
	std::string out = root.toStyledString();
	strResult = out.c_str();

	return strResult.AllocSysString();
}

void CKeWebCamOCXCtrl::OnRecordFilePathChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加属性处理程序代码

	SetModifiedFlag();
}

BSTR CKeWebCamOCXCtrl::StartRecord(LONG cameraID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	// 在此添加调度处理程序代码
	int ret = 0;
	COneCamera * pCamera = theApp.g_PlayWnd->GetCamera(cameraID);
	CString fullPath;
	if (pCamera == NULL)
	{
		ret = KE_FAILED;
	}
	else
	{
// 		SYSTEMTIME sys;
// 		GetLocalTime( &sys );
// 		//镜头id+年月日时分秒+毫秒
// 		CString fileName ;
// 		fileName.Format(_T("%d_%04d%02d%02d%02d%02d%02d%03d.h264"), pCamera->m_cameraID,
// 			sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
// 		CString filePath;
// 		if (m_RecordFilePath.GetAt(m_RecordFilePath.GetLength()-1)!=_T('\\'))
// 		{
// 			m_RecordFilePath.AppendChar(_T('\\'));
// 		}
// 		filePath.Format(_T("%s%d\\"),m_RecordFilePath,pCamera->m_cameraID);
// 
// 		if (!FolderExist(filePath))
// 		{
// 			CreateFolderEx(filePath);
// 		}
// 
// 		fullPath = filePath + fileName;
		fullPath = GetCamTimeFileName(pCamera->m_cameraID,_T("h264"),m_RecordFilePath);
		std::string strPath = tstr_to_str(fullPath.GetBuffer(0));
		ret = pCamera->m_MediaSocket->StartRecord(strPath.c_str());
		if (ret != 0)
		{
			ret = KE_FAILED;
		}
		ret = KE_SUCCESS;
	}

	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = GetKEErrorDescriptA(ret);
	root["filePath"] = tstr_to_str(fullPath.GetString());
	std::string out = root.toStyledString();
	strResult = out.c_str();

	return strResult.AllocSysString();
}

BSTR CKeWebCamOCXCtrl::StopRecord(LONG cameraID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	//  在此添加调度处理程序代码
	int ret;
	COneCamera * pCamera = theApp.g_PlayWnd->GetCamera(cameraID);
	if (pCamera == NULL)
	{
		ret = KE_FAILED;
	}
	else
	{
		ret = pCamera->m_MediaSocket->StopRecord();
	}
	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = GetKEErrorDescriptA(ret);
	std::string out = root.toStyledString();
	strResult = out.c_str();

	return strResult.AllocSysString();
}

void CKeWebCamOCXCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1://心跳定时器
		theApp.g_cmd->HeartBeat();
		break;
	}
	COleControl::OnTimer(nIDEvent);
}

//************************************
// Method:    InitailCtrl
// FullName:  CKeWebCamOCXCtrl::InitailCtrl
// Access:    protected 
// Returns:   BSTR
// Qualifier:
// Parameter: LONG platform
// Parameter: LONG viewType
//************************************
BSTR CKeWebCamOCXCtrl::InitailCtrl(LONG platform)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	// 在此添加调度处理程序代码
	int ret = KE_SUCCESS;
	switch(platform)
	{
	case 1:theApp.g_cmd = new CP2PCmdSocket();break;
	default:
		theApp.g_cmd = new CenterCommand();break;
	}
	theApp.g_cmd->platformType = platform;
	//建立心跳定时器,时间间隔为30s  
	m_HeartBeatTimer=SetTimer(1,10000,NULL);  

	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = theApp.g_cmd->GetErrorDesA(ret);
	std::string out = root.toStyledString();
	strResult = out.c_str();

	return strResult.AllocSysString();
}

BSTR CKeWebCamOCXCtrl::ConnectServer(LPCTSTR svrAddr, LONG svrPort, LONG clientID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// 在此添加调度处理程序代码
	int ret  = theApp.g_cmd->ConnectServer(svrAddr,svrPort);
	if (ret == KE_SUCCESS)
	{
		ret = theApp.g_cmd->SetClientID(clientID);
	}

	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = theApp.g_cmd->GetErrorDesA(ret);
	std::string out = root.toStyledString();
	strResult = out.c_str();

	return strResult.AllocSysString();
}

BSTR CKeWebCamOCXCtrl::QueryRecordFileList(LONG cameraID, LONG startTime, LONG endTime, LONG fileType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	int ret = KE_SUCCESS;

	std::vector<RecordFileInfo> recordFileList;
	
	ret  = theApp.g_cmd->GetRecordFileList(cameraID,startTime,endTime,fileType,recordFileList);
	
	
	Json::Value root;
	root["retValue"] = ret;
	root["retDes"] = theApp.g_cmd->GetErrorDesA(ret);
	Json::Value arrayObj;
	Json::Value item;
	for (int i=0;i< recordFileList.size();i++)
	{
		RecordFileInfo info = recordFileList[i];
		item["fileNo"] = i;
		item["fileSize"] = info.fileSize;
		item["startTime"] = info.startTime;
		item["endTime"] = info.endTime;
		//item["data"] = info.data;
		arrayObj.append(item);
	}
	root["fileList"] = arrayObj;
	std::string out = root.toStyledString();
	strResult = out.c_str();
	return strResult.AllocSysString();
}
