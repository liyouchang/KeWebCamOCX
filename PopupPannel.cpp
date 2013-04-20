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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	 		//��ȡ��Ļ�ķֱ���   
	int nFullWidth=GetSystemMetrics(SM_CXSCREEN);     
	int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
	 
	 		//������������Ŀͻ���ȫ����ʾ����(0,0)��(nFullWidth, nFullHeight)����, 
	 		//��(0,0)��(nFullWidth, nFullHeight)������������ԭ���ںͳ�������֮���
			//�ͻ���λ�ü�Ĳ�ֵ, �͵õ�ȫ����ʾ�Ĵ���λ��    
	m_FullScreenRect.left=0;
	m_FullScreenRect.top=0;
	m_FullScreenRect.right=nFullWidth;
	m_FullScreenRect.bottom=nFullHeight;
	m_pannel.Create(IDD_DIALOG_PANNEL,this);
 	MoveWindow(m_FullScreenRect,TRUE);
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
}
