#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "SliderGdiCtrlSource/SliderGDICtrl.h"
#include "CButtonST_src/BtnST.h"
#include "resource.h"
// CPlayerCtrlDlg �Ի���

class CPlayerCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlayerCtrlDlg)

public:
	CPlayerCtrlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayerCtrlDlg();
public: 

	int m_iTimeHandle;                      //1s��ʱ��������û���ȡ���ؽ��ȺͲ��Ž���
	void SetPlayState(int id);
	void SetWindowEnable(int id);
	void GetPlayPos();
	void PlayStart();
	void PlayStop();
// �Ի�������
	enum { IDD = IDD_DIALOG_Player };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	virtual void OnCancel();
public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	afx_msg void OnNMReleasedcaptureSliderPos(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonFast();
	afx_msg void OnBnClickedButtonSlow();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonOpenfile();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMCustomdrawSliderPos(NMHDR *pNMHDR, LRESULT *pResult);

	CButtonST m_btnPlay;
	CButtonST m_btnFast;
	CButtonST m_btnSlow;
	CButtonST m_btnPause;
	CButtonST m_btnStop;
	CButtonST m_btnOpen;
	afx_msg LRESULT  FilePlayOver(WPARAM wParam, LPARAM lParam);
	CSliderCtrl m_playpos;
//	afx_msg void OnStnClickedSlowplay();
};
