#pragma once


// CPopupPannel �Ի���

class CPopupPannel : public CDialog
{
	DECLARE_DYNAMIC(CPopupPannel)

public:
	CPopupPannel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPopupPannel();

// �Ի�������
	enum { IDD = IDD_DLG_PLAYCONTROL };
	CRect m_FullScreenRect;
	CWebCamPannel m_pannel;
	CWebCamPannel *m_pToFullPannel;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPaint();
protected:
	virtual void OnCancel();
};
