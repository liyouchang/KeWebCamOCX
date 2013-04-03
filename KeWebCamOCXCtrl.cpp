// KeWebCamOCXCtrl.cpp : CKeWebCamOCXCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "KeWebCamOCXCtrl.h"
#include "KeWebCamOCXPropPage.h"
#include <json/json.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CKeWebCamOCXApp theApp;

IMPLEMENT_DYNCREATE(CKeWebCamOCXCtrl, COleControl)



// 消息映射

BEGIN_MESSAGE_MAP(CKeWebCamOCXCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CKeWebCamOCXCtrl, COleControl)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "SetDivision", dispidSetDivision, SetDivision, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "KE_UserLoginServer", dispidKE_UserLoginServer, KE_UserLoginServer, VT_I4, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "QueryUserCamera", dispidQueryUserCamera, QueryUserCamera, VT_BSTR, VTS_NONE)

	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "PlayRTV", dispidPlayRTV, PlayRTV, VT_BSTR, VTS_BSTR)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CKeWebCamOCXCtrl, COleControl)
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
	// TODO: 在此初始化控件的实例数据。

	InitLogModule();
}



// CKeWebCamOCXCtrl::~CKeWebCamOCXCtrl - 析构函数

CKeWebCamOCXCtrl::~CKeWebCamOCXCtrl()
{
	// TODO: 在此清理控件的实例数据。
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

	m_pannel.Create(IDD_DIALOG_PANNEL,this);
	
	theApp.g_PlayWnd = &m_pannel;

	return 0;
}

void CKeWebCamOCXCtrl::SetDivision(LONG nDivision)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	// TODO: 在此添加调度处理程序代码
	m_pannel.SetPlayDivision(nDivision);
}

LONG CKeWebCamOCXCtrl::KE_UserLoginServer(LPCTSTR userName, LPCTSTR password, LPCTSTR ipAddr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (theApp.m_cmdSocket.IsConnect())
	{
		theApp.m_cmdSocket.CloseConnect();
	}
	if (!theApp.m_cmdSocket.ConnectToServer(ipAddr))
	{
		AfxMessageBox(_T("connet to server error"));
		return -1;
	}
	int ret = theApp.m_cmdSocket.LoginServer(userName,password);
	if (ret != KE_SUCCESS)
	{
		//AfxMessageBox(_T("login server error"));
	}
	return ret;
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

BSTR CKeWebCamOCXCtrl::PlayRTV(LPCTSTR cameralName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码
	


	return strResult.AllocSysString();
}
