#pragma once


// CPopupPannel 对话框

class CPopupPannel : public CDialog
{
	DECLARE_DYNAMIC(CPopupPannel)

public:
	CPopupPannel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPopupPannel();

// 对话框数据
	enum { IDD = IDD_DLG_PLAYCONTROL };
	CRect m_FullScreenRect;
	CWebCamPannel m_pannel;
	CWebCamPannel *m_pToFullPannel;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPaint();
protected:
	virtual void OnCancel();
};
