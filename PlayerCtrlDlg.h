#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "SliderGdiCtrlSource/SliderGDICtrl.h"
#include "CButtonST_src/BtnST.h"
#include "resource.h"
#include "NiceSlider.h"
// CPlayerCtrlDlg 对话框

class CPlayerCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlayerCtrlDlg)

public:
	CPlayerCtrlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlayerCtrlDlg();
public: 

	int m_iTimeHandle;                      //1s定时器句柄，用户获取下载进度和播放进度
	void SetPlayState(int id);
	void SetWindowEnable(int id);
	void GetPlayPos();
	void PlayStart();
	void PlayStop();

// 对话框数据
	enum { IDD = IDD_DIALOG_Player };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	virtual void OnCancel();
public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
//	afx_msg void OnNMReleasedcaptureSliderPos(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonFast();
	afx_msg void OnBnClickedButtonSlow();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonOpenfile();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CButtonST m_btnPlay;
	CButtonST m_btnFast;
	CButtonST m_btnSlow;
	CButtonST m_btnPause;
	CButtonST m_btnStop;
	CButtonST m_btnOpen;
	afx_msg LRESULT  FilePlayOver(WPARAM wParam, LPARAM lParam);
	CNiceSliderCtrl m_playpos;
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnNMCustomdrawSliderMp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
