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

	// TODO:  在此添加额外的初始化
	 		//获取屏幕的分辨率   
	int nFullWidth=GetSystemMetrics(SM_CXSCREEN);     
	int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
	 
	 		//将除控制条外的客户区全屏显示到从(0,0)到(nFullWidth, nFullHeight)区域, 
	 		//将(0,0)和(nFullWidth, nFullHeight)两个点外扩充原窗口和除控制条之外的
			//客户区位置间的差值, 就得到全屏显示的窗口位置    
	m_FullScreenRect.left=0;
	m_FullScreenRect.top=0;
	m_FullScreenRect.right=nFullWidth;
	m_FullScreenRect.bottom=nFullHeight;
	m_pannel.Create(IDD_DIALOG_PANNEL,this);
 	MoveWindow(m_FullScreenRect,TRUE);
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
}
