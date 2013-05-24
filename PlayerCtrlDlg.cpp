// PlayerCtrlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "PlayerCtrlDlg.h"


// CPlayerCtrlDlg 对话框

IMPLEMENT_DYNAMIC(CPlayerCtrlDlg, CDialog)

CPlayerCtrlDlg::CPlayerCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerCtrlDlg::IDD, pParent)
{

}

CPlayerCtrlDlg::~CPlayerCtrlDlg()
{
}

void CPlayerCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_POS, m_playpos);
}


BEGIN_MESSAGE_MAP(CPlayerCtrlDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CPlayerCtrlDlg::OnBnClickedButtonPlay)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_POS, &CPlayerCtrlDlg::OnNMReleasedcaptureSliderPos)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPlayerCtrlDlg 消息处理程序

void CPlayerCtrlDlg::OnBnClickedButtonPlay()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CPlayerCtrlDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	theApp.g_PlayWnd->GetParent()->SendMessage(WM_CLOSE);
//	CDialog::OnCancel();
}

BOOL CPlayerCtrlDlg::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialog::OnChildNotify(message, wParam, lParam, pLResult);
}

void CPlayerCtrlDlg::OnNMReleasedcaptureSliderPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

BOOL CPlayerCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPlayerCtrlDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

}
