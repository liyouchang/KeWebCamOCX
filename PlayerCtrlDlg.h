#pragma once
#include "afxwin.h"


// CPlayerCtrlDlg �Ի���

class CPlayerCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlayerCtrlDlg)

public:
	CPlayerCtrlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayerCtrlDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_Player };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPlay();
protected:
	virtual void OnCancel();
public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
};
