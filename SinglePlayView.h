#pragma once
#include "afxwin.h"


// CSinglePlayView �Ի���


const int iLineWidth = 1 ;
const int iBottomHeight = 12 ;
const int m_nTimerCruise = 10 ;


class CWebCamPannel;

class CSinglePlayView : public CDialog
{
	DECLARE_DYNAMIC(CSinglePlayView)

public:
	CSinglePlayView(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSinglePlayView();
	void MovePlayView(int iLeft, int iTop, int iWidth, int iHeigth);
	void MovePlayView(CRect rect, BOOL bRepaint =TRUE);
	void SetCtrlPosition();
	HWND GetPlayHandle();
	void SetSelectState(BOOL IsSelect);

	void SetOwner(CWebCamPannel* pOwner) { m_pOwner = pOwner; };
// �Ի�������
	enum { IDD = IDD_FORMVIEW_PLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnStnClickedStaticPicture();
	CString m_strLeftText;
	CString m_strMidText;
	CString m_strRightText;
	afx_msg void OnPaint();
public:
	BOOL m_bDrawable;
	BOOL m_bFull;
	BOOL bIsSelect;
	BOOL bIsShowBottom;
	CWebCamPannel*		m_pOwner;

private:

	HBRUSH m_brush;
	//���ڲ��ŵ��������ͣ�0����û���ţ�1��ʵʱ���ݣ�2����ʷ����
	BYTE m_byPlayType;
	
private:
	
public:
	CStatic m_PlayArea;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
