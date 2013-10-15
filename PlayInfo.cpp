// PlayInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "PlayInfo.h"
#include "afxdialogex.h"


// CPlayInfo 对话框

IMPLEMENT_DYNAMIC(CPlayInfo, CDialog)

CPlayInfo::CPlayInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayInfo::IDD, pParent)
{

}

CPlayInfo::~CPlayInfo()
{
}

void CPlayInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlayInfo, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPlayInfo 消息处理程序


BOOL CPlayInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(1,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPlayInfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect rect;
	GetClientRect(rect);
	CRect infoRect;
	GetDlgItem(IDC_PlayInfo)->GetWindowRect(infoRect);
	this->ScreenToClient(infoRect);
	infoRect.MoveToX(rect.Width()-infoRect.Width());
	GetDlgItem(IDC_PlayInfo)->MoveWindow(infoRect);

}


void CPlayInfo::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage(WM_LBUTTONDOWN);
	CDialog::OnLButtonDown(nFlags, point);
}


void CPlayInfo::OnLButtonUp(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage(WM_LBUTTONUP);
	CDialog::OnLButtonUp(nFlags, point);
}


void CPlayInfo::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	//this->GetClientRect(ctrlRect);
	

	GetWindowRect( &rect );
	GetParent()->ScreenToClient(&rect);
	point.Offset(rect.left,rect.top);
	LPARAM lp = MAKELONG(point.x,point.y);
	GetParent()->SendMessage(WM_RBUTTONDOWN,nFlags,lp);
	CDialog::OnRButtonDown(nFlags, point);
}


void CPlayInfo::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	GetParent()->SendMessage(WM_CLOSE);
	CDialog::OnCancel();
}


void CPlayInfo::OnTimer(UINT_PTR nIDEvent)
{
	COnePlayer * pCamera = (COnePlayer *)this->GetParent();
	CString tmpStr;
	CString recordStr = _T("");
	if (pCamera->IsPlaying())
	{
		int frame = pCamera->m_AVIPlayer->GetPlayFrameRate();
		float flux = pCamera->m_AVIPlayer->GetPlayFluxRate();
		CString encode = pCamera->m_AVIPlayer->GetEncodeFormatName();
		tmpStr.Format(_T("格式:%s,帧率:%d,码率:%.02f"),encode,frame,flux);
		
		if (pCamera->IsRecording())
		{
			recordStr = _T("正在录像");
			
		}
	}
	else{
		tmpStr = _T("无视频");
	}
	this->SetDlgItemText(IDC_PlayInfo,tmpStr);
	this->SetDlgItemText(IDC_RecordInfo,recordStr);
	CDialog::OnTimer(nIDEvent);
}
