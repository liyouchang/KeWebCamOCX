#if !defined(AFX_ONECAMERA_H__3DB88E96_8971_44E2_927B_50700BE0B987__INCLUDED_)
#define AFX_ONECAMERA_H__3DB88E96_8971_44E2_927B_50700BE0B987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OneCamera.h : header file
//

#include "../CxImage/ximage.h"
#include "../CxImage/xfile.h"
#include "MyAVPlayer.h"
#include "OleDropTargetEx.h"

#define FLOAT_CAM_FONT_COLOR     RGB(0, 0, 0)//clWhite
#define FLOAT_CAM_FRAME_COLOR    RGB(128, 128, 128)//clGray
#define clBlue	RGB(0, 0, 255)
#define TOGGLED_FrameColor RGB(200, 200, 200)//clSilver

typedef struct _cam_reference
{
	int Brightness;
	int Contrast;
	int Hue;
	int Saturation;
} cam_reference;

typedef struct _event_information
{
	BOOL bAlarm;
	BOOL bMotion;
	BOOL bLoss;
} event_information;

class CWebCamPannel;
class CxImage;
class CLock;
//class TAVI;
/////////////////////////////////////////////////////////////////////////////
// COneCamera window

class COneCamera : public CStatic
{
private:
	int		FCamNo;
// Construction
public:
	CMenu				menu;
	CWebCamPannel*		m_pOwner;
	CRect				m_defaultArea;
	CPoint				m_DownPos;
	CMyAVPlayer m_AVIPlayer;
	
	int videoID;
	int channelNo;

	BOOL				m_bPress;
	int					m_nTag;
    int					m_nCamNo;//在父窗口中的镜头数组中的位置
	BOOL				m_bDrawable;	//是否显示 6, 8, 13 mode
	BOOL				m_bActive;		
	int					m_nRec_Type;	// CIF mode type
	CString				m_strCaption;
	CString				m_strDateTime;
    cam_reference		m_CamRef;
	event_information	m_event;
	int					m_nAviIndex;
	BOOL				m_bMotion;
	BOOL				m_bAlaram;
	BOOL				m_bLoss;
	BOOL				m_bFull;
    BYTE				m_bEvent;
    BOOL				m_bSaveToAVI;
	BOOL m_bIsPlaying;

	COneCamera();

	void SetOwner(CWebCamPannel* pOwner) { m_pOwner = pOwner; };
	virtual BOOL RegisterDrop();
protected:
	COleDropTargetEx m_dropEx;
	//处理OnDropEx消息是必须的，否则OnDrop函数不会被执行
	//当然也可以在OnDropEx函数中处理数据
	virtual LRESULT OnDrop(WPARAM pDropInfoClass, LPARAM lParm);
	virtual LRESULT OnDropEx(WPARAM pDropInfoClass, LPARAM lParm);
	virtual LRESULT OnDragOver(WPARAM pDropInfoClass,LPARAM lParm);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COneCamera)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ResetAVI();
	void __fastcall DrawCameraImage(CDC *pdc, CRect destFrame, CString caption, COLORREF FontColor, COLORREF FrameColor);
	void InitCarmera();
	void MoveWindows(CRect rect, BOOL Visible);
	int GetCameraCanvasTextHeight();
	virtual ~COneCamera();

	void LButtonDown(UINT nFlags, CPoint point);
	void LButtonDblClk(UINT nFlags, CPoint point);

	// Generated message map functions
protected:
	//{{AFX_MSG(COneCamera)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuSaveas();
	afx_msg void OnMenuSaveall();
	afx_msg void OnMenuResetChannel();
	afx_msg void OnMenuResetAll();

	afx_msg void OnMenuShowWatermark();

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMenuPlay();
	afx_msg void OnMenuSlow();
	afx_msg void OnMenuFast();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONECAMERA_H__3DB88E96_8971_44E2_927B_50700BE0B987__INCLUDED_)
