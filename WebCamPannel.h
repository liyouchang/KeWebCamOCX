#pragma once
#include "afxwin.h"

#include "common.h"
#include "OneCamera.h"
#include "ClientDemon.h"
#include "OnePlayer.h"
// CWebCamPannel �Ի���
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
public://��Ҫ�����ԣ���Ҫ������
		
	int m_nActiveCamera;//��ǰѡ�еľ�ͷ
	BOOL m_bFullScreen;
	int m_FnDivision;//��ǰ�ķ�������
	COnePlayer		m_camarray[CAM_MAX];
	//COnePlayer	m_player[CAM_MAX];

public:
	CBrush m_brBkgnd;
	int	ColCount;
	int	RowCount;

	int m_nFullPaintHeight;
	int m_nFullPaintWidth;

	int m_nToggledDivision;

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
	
	bool PeekLButtonUpMsg();
	void DrawCameraFrame(CDC *pDC=NULL);
	void MoveCameraRect(int nMode = 0);
	void DrawFrame(CDC *pDC);
	void MoveCameraPlay();
	void SetCamDrawable(int nDiv = 0);
	void SetActiveCamera(int nCamNo);
	void StopAllRTPlay();
	//ֹͣȫ������
	void StopAllAudio();
	//ֹͣȫ���Խ�
	void StopAllTalk();
public:
	CWebCamPannel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWebCamPannel();
	// �Ի�������
	enum { IDD = IDD_DIALOG_PANNEL };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	COnePlayer * GetOnePlayer(int cameraID);
	COnePlayer * ReuseActivePlayer(int cameraID);
	COnePlayer * GetActivePlayer();

	afx_msg void OnStnClickedCamera0();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	virtual void OnCancel();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
