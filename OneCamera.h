#if !defined(AFX_ONECAMERA_H__3DB88E96_8971_44E2_927B_50700BE0B987__INCLUDED_)
#define AFX_ONECAMERA_H__3DB88E96_8971_44E2_927B_50700BE0B987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OneCamera.h : header file
//

//#include "../CxImage/ximage.h"
//#include "../CxImage/xfile.h"
#include "MyAVPlayer.h"
#include "MediaSocket.h"
#include "PlayerCtrlDlg.h"

//#include "OleDropTargetEx.h"


typedef struct _event_information
{
	BOOL bAlarm;
	BOOL bMotion;
	BOOL bLoss;
} event_information;

class CWebCamPannel;
class CxImage;
class CLock;
class COnePlayer;
//class TAVI;
/////////////////////////////////////////////////////////////////////////////
// COneCamera window

class COneCamera : public CStatic
{
public:
	CMenu				menu;
	COnePlayer*		m_pOwner;
	//CMyAVPlayer *m_AVIPlayer;
	//CMediaSocket * m_MediaSocket;
	//int m_cameraID;
	bool CheckMediaExist(int videoID, int channelNo);
// 	BOOL	m_bDrag;
// 	BOOL	m_bDrawable;	//�Ƿ���ʾ 6, 8, 13 mode
// 	BOOL	m_bFull;

	COneCamera();
	void SetOwner(COnePlayer* pOwner);
	void ExchangeAVIPlayer(CMyAVPlayer *otherPlayer);
	void ExchangeCamera(COneCamera *camera);


// Attributes
public:
	CBrush m_brBkgnd; 
	//CPlayerCtrlDlg * ctrlDlg;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COneCamera)
	//}}AFX_VIRTUAL
// Implementation
public:
	virtual		~COneCamera();
	// Generated message map functions
protected:
	//{{AFX_MSG(COneCamera)
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

//	afx_msg void OnMenuResetChannel();
//	afx_msg void OnMenuResetAll();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
//	afx_msg void OnMenuFullscreen();
public:
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONECAMERA_H__3DB88E96_8971_44E2_927B_50700BE0B987__INCLUDED_)
