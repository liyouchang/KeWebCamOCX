// KeWebCamOCXPropPage.cpp : CKeWebCamOCXPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "KeWebCamOCXPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CKeWebCamOCXPropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CKeWebCamOCXPropPage, COlePropertyPage)
	
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CKeWebCamOCXPropPage, "KEWEBCAMOCX.KeWebCamOCXPropPage.1",
	0x89f2cb48, 0xbaae, 0x4205, 0x92, 0x55, 0x24, 0x56, 0xc0, 0x26, 0x54, 0x73)



// CKeWebCamOCXPropPage::CKeWebCamOCXPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CKeWebCamOCXPropPage ��ϵͳע�����

BOOL CKeWebCamOCXPropPage::CKeWebCamOCXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_KEWEBCAMOCX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CKeWebCamOCXPropPage::CKeWebCamOCXPropPage - ���캯��

CKeWebCamOCXPropPage::CKeWebCamOCXPropPage() :
	COlePropertyPage(IDD, IDS_KEWEBCAMOCX_PPG_CAPTION)
{
}



// CKeWebCamOCXPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CKeWebCamOCXPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
	
}



// CKeWebCamOCXPropPage ��Ϣ�������

