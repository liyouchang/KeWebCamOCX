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
}


BEGIN_MESSAGE_MAP(CPlayerCtrlDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CPlayerCtrlDlg::OnBnClickedButtonPlay)
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
