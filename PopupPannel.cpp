// PopupPannel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "PopupPannel.h"

// CPopupPannel �Ի���

IMPLEMENT_DYNAMIC(CPopupPannel, CDialog)

CPopupPannel::CPopupPannel(CWnd* pParent /*=NULL*/)
	: CDialog(CPopupPannel::IDD, pParent)
{

}

CPopupPannel::~CPopupPannel()
{

}

void CPopupPannel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPopupPannel, CDialog)
	ON_WM_DRAWITEM()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPopupPannel ��Ϣ�������

BOOL CPopupPannel::OnInitDialog()
{
	CDialog::OnInitDialog();

	//  �ڴ���Ӷ���ĳ�ʼ��
	 		//��ȡ��Ļ�ķֱ���   
	int nFullWidth=GetSystemMetrics(SM_CXSCREEN);     
	int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
	m_FullScreenRect.left=0;
	m_FullScreenRect.top=0;
	m_FullScreenRect.right=nFullWidth;
	m_FullScreenRect.bottom=nFullHeight;
	TRACE1("Pannel create start %d\n",GetTickCount());
	m_pannel.Create(IDD_DIALOG_PANNEL,this);
	TRACE1("Pannel create end %d\n",GetTickCount());
 	MoveWindow(m_FullScreenRect,TRUE);

	m_pannel.CopyPannel(m_pToFullPannel);
TRACE1("Initialize end %d\n",GetTickCount());
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPopupPannel::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CPopupPannel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	CRect rcClient;
	GetClientRect(rcClient);
	m_pannel.MoveWindow(rcClient);
}

void CPopupPannel::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	
	CDialog::OnCancel();
}
