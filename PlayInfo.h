#pragma once


// CPlayInfo �Ի���

class CPlayInfo : public CDialog
{
	DECLARE_DYNAMIC(CPlayInfo)

public:
	CPlayInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayInfo();

// �Ի�������
	enum { IDD = IDD_PlayInfo };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
