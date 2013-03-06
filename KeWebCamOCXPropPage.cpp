// KeWebCamOCXPropPage.cpp : CKeWebCamOCXPropPage 属性页类的实现。

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "KeWebCamOCXPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CKeWebCamOCXPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CKeWebCamOCXPropPage, COlePropertyPage)
	
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CKeWebCamOCXPropPage, "KEWEBCAMOCX.KeWebCamOCXPropPage.1",
	0x89f2cb48, 0xbaae, 0x4205, 0x92, 0x55, 0x24, 0x56, 0xc0, 0x26, 0x54, 0x73)



// CKeWebCamOCXPropPage::CKeWebCamOCXPropPageFactory::UpdateRegistry -
// 添加或移除 CKeWebCamOCXPropPage 的系统注册表项

BOOL CKeWebCamOCXPropPage::CKeWebCamOCXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_KEWEBCAMOCX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CKeWebCamOCXPropPage::CKeWebCamOCXPropPage - 构造函数

CKeWebCamOCXPropPage::CKeWebCamOCXPropPage() :
	COlePropertyPage(IDD, IDS_KEWEBCAMOCX_PPG_CAPTION)
{
}



// CKeWebCamOCXPropPage::DoDataExchange - 在页和属性间移动数据

void CKeWebCamOCXPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
	
}



// CKeWebCamOCXPropPage 消息处理程序

