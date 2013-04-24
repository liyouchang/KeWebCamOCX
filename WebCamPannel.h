#pragma once
#include "afxwin.h"

#include "common.h"
#include "OneCamera.h"
#include "ClientDemon.h"

// CWebCamPannel 对话框
#include <map>
#define DIVISION_MAX_CH16		4
#define DIVISION_MAX_CH9		3
#define DIVISION_MAX_CH8		3
#define DIVISION_MAX_CH4		2
#define DIVISION_MAX_CH1		1


#define IMAGE_MAX_CH16			(DIVISION_MAX_CH16 * DIVISION_MAX_CH16)
#define IMAGE_MAX_CH9			(DIVISION_MAX_CH9  * DIVISION_MAX__CH9)
#define IMAGE_MAX_CH8			(DIVISION_MAX_CH8  * DIVISION_MAX__CH8)
#define IMAGE_MAX_CH4			(DIVISION_MAX_CH4  * DIVISION_MAX_CH4)
#define IMAGE_MAX_CH1			(DIVISION_MAX_CH1  * DIVISION_MAX_CH1)


class CWebCamPannel : public CDialog
{
	DECLARE_DYNAMIC(CWebCamPannel)
public://重要的属性，需要被复制
	int FnDivision;	
	int m_nActiveCamera;//当前选中的镜头
	BOOL m_bFullScreen;
	int m_nToggledDivision;
	int m_nCameraNumber;
public:
	int m_DragCam;
	int m_nWatermark;

	//CxImage 	temp_image;
	//CClientDemon demon;
	int	ColCount;
	int	RowCount;
	BOOL	m_bDivision;
	int	m_nRotation_01;
	int	m_nRotation_04;
	int	m_nRotation_09;
	int	m_nRotation_06;
	int	m_nRotation_08;
	int	m_nRotation_10;
	void ShowFullScreen();
	void CopyPannel( CWebCamPannel * pannel);
	void SetPlayDivision(int nDiv);
	COLORREF GetCamFontColor();
	void InvalidateAll();
	CRect GetFullPaintRect();
	int m_nFullPaintHeight;
	int m_nFullPaintWidth;
	bool PeekLButtonUpMsg();
	void DrawAllCameraImages(CDC *pDC=NULL);
	void GetCameraRect(int nMode);
	void DrawFrame(CDC *pDC);
	void SetActiveCamera(int nCamNo);
	void StopAllRTPlay();
public:
	CWebCamPannel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWebCamPannel();
	// 对话框数据
	enum { IDD = IDD_DIALOG_PANNEL };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	int			m_nMaxChannel;
	COneCamera * GetOnePlayer(int cameraID,int * isPlaying = NULL);
	COneCamera * GetCamera(int cameraID);
	//CSinglePlayView  m_playViewArray[CAM_MAX+1];
	COneCamera		m_camarray[CAM_MAX+1];
	//CxImage			m_Imagearray[CAM_MAX+1];
	afx_msg void OnStnClickedCamera0();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
