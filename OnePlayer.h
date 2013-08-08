#pragma once
#include "afxwin.h"
#include "OneCamera.h"

// COnePlayer 对话框
class CWebCamPannel;

class COnePlayer : public CDialog
{
	DECLARE_DYNAMIC(COnePlayer)

public:
	COnePlayer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COnePlayer();

// 对话框数据
	enum { IDD = IDD_FORMVIEW_PLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	COneCamera m_camera;
	CMyAVPlayer *m_AVIPlayer;
	int m_cameraID;
	BOOL	m_bDrawable;	//是否显示 6, 8, 13 mode
	BOOL	m_bFull;
	BOOL	m_bDrag;
	int	m_nCamNo; //在父窗口中的镜头数组中的位置
	CWebCamPannel * m_pOwner;
	CPlayerCtrlDlg * ctrlDlg;
	bool showCtrlDlg;
	CMenu	menu;

		CBrush m_brBkgnd; 
public:
	bool IsPlaying();
	void ExchangeAVIPlayer(CMyAVPlayer *otherPlayer);
	void SwapVideo(COnePlayer * otherPlayer);
	void StopRTPlay(bool reUse = false);
	int StartPlay();
	void SetActive();
	bool IsActive();
public:
	void SetOwner(CWebCamPannel* pOwner);
	
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMenuAllfullscreen();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnMenuReset();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
