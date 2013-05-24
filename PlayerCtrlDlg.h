#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CPlayerCtrlDlg 对话框

class CPlayerCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlayerCtrlDlg)

public:
	CPlayerCtrlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlayerCtrlDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_Player };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPlay();
protected:
	virtual void OnCancel();
public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	afx_msg void OnNMReleasedcaptureSliderPos(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_playpos;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
