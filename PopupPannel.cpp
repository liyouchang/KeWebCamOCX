// PopupPannel.cpp : 实现文件
//

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "PopupPannel.h"

// CPopupPannel 对话框

IMPLEMENT_DYNAMIC(CPopupPannel, CDialog)

CPopupPannel::CPopupPannel(CWnd* pParent /*=NULL*/)
	: CDialog(CPopupPannel::IDD, pParent)
{

}

CPopupPannel::~CPopupPannel()
{

}

void CPopupPannel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPopupPannel, CDialog)
	ON_WM_DRAWITEM()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPopupPannel 消息处理程序

BOOL CPopupPannel::OnInitDialog()
{
	CDialog::OnInitDialog();

	//  在此添加额外的初始化
	 		//获取屏幕的分辨率   
	int nFullWidth=GetSystemMetrics(SM_CXSCREEN);     
	int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
	m_FullScreenRect.left=0;
	m_FullScreenRect.top=0;
	m_FullScreenRect.right=nFullWidth;
	m_FullScreenRect.bottom=nFullHeight;
	TRACE1("Pannel create start %d\n",GetTickCount());
	m_pannel.Create(IDD_DIALOG_PANNEL,this);
	TRACE1("Pannel create end %d\n",GetTickCount());
 	MoveWindow(m_FullScreenRect,TRUE);

	m_pannel.CopyPannel(m_pToFullPannel);
TRACE1("Initialize end %d\n",GetTickCount());
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPopupPannel::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CPopupPannel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	CRect rcClient;
	GetClientRect(rcClient);
	m_pannel.MoveWindow(rcClient);
}

void CPopupPannel::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	CDialog::OnCancel();
}
