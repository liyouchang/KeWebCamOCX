// KeWebCamOCXCtrl.cpp : CKeWebCamOCXCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "KeWebCamOCXCtrl.h"
#include "KeWebCamOCXPropPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CKeWebCamOCXCtrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CKeWebCamOCXCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CKeWebCamOCXCtrl, COleControl)
	DISP_FUNCTION_ID(CKeWebCamOCXCtrl, "SetDivision", dispidSetDivision, SetDivision, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CKeWebCamOCXCtrl, COleControl)
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

	m_pannel.MoveWindow(rcBounds,TRUE);


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

	// TODO:  �ڴ������ר�õĴ�������
	m_pannel.Create(IDD_DIALOG_PANNEL,this);

	return 0;
}

void CKeWebCamOCXCtrl::SetDivision(LONG nDivision)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	// TODO: �ڴ���ӵ��ȴ���������
	m_pannel.SetDivision(nDivision,-1);
}
