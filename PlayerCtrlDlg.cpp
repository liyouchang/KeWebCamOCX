// PlayerCtrlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "PlayerCtrlDlg.h"


// CPlayerCtrlDlg �Ի���

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


// CPlayerCtrlDlg ��Ϣ�������

void CPlayerCtrlDlg::OnBnClickedButtonPlay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CPlayerCtrlDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	theApp.g_PlayWnd->GetParent()->SendMessage(WM_CLOSE);
//	CDialog::OnCancel();
}

BOOL CPlayerCtrlDlg::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialog::OnChildNotify(message, wParam, lParam, pLResult);
}

void CPlayerCtrlDlg::OnNMReleasedcaptureSliderPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

BOOL CPlayerCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPlayerCtrlDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

}
