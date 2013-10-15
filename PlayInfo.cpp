// PlayInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "PlayInfo.h"
#include "afxdialogex.h"


// CPlayInfo �Ի���

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


// CPlayInfo ��Ϣ�������


BOOL CPlayInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetTimer(1,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPlayInfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
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
	// TODO: �ڴ����ר�ô����/����û���
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
		tmpStr.Format(_T("��ʽ:%s,֡��:%d,����:%.02f"),encode,frame,flux);
		
		if (pCamera->IsRecording())
		{
			recordStr = _T("����¼��");
			
		}
	}
	else{
		tmpStr = _T("����Ƶ");
	}
	this->SetDlgItemText(IDC_PlayInfo,tmpStr);
	this->SetDlgItemText(IDC_RecordInfo,recordStr);
	CDialog::OnTimer(nIDEvent);
}
