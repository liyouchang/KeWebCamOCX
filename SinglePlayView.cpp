// SinglePlayView.cpp : 实现文件
//

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "SinglePlayView.h"


// CSinglePlayView 对话框

IMPLEMENT_DYNAMIC(CSinglePlayView, CDialog)

CSinglePlayView::CSinglePlayView(CWnd* pParent /*=NULL*/)
	: CDialog(CSinglePlayView::IDD, pParent)
	, m_strLeftText(_T(""))
	, m_strMidText(_T(""))
	, m_strRightText(_T(""))
{
	bIsShowBottom = FALSE;
	m_bDrawable = FALSE;
	m_bFull =FALSE;
	bIsSelect = FALSE;
}

CSinglePlayView::~CSinglePlayView()
{
}

void CSinglePlayView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXT_LEFT, m_strLeftText);
	DDX_Text(pDX, IDC_TEXT_MID, m_strMidText);
	DDX_Text(pDX, IDC_TEXT_RIGHT, m_strRightText);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_PlayArea);
}


BEGIN_MESSAGE_MAP(CSinglePlayView, CDialog)
	ON_STN_CLICKED(IDC_STATIC_PICTURE, &CSinglePlayView::OnStnClickedStaticPicture)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CSinglePlayView 消息处理程序

void CSinglePlayView::OnStnClickedStaticPicture()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CSinglePlayView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(rcClient);
	CBrush m_brush(RGB(243, 244, 246));
	dc.FillRect(rcClient,&m_brush);
	if ( bIsSelect )
	{//画选择边框
		CBrush brSide(RGB(90, 237, 9));
		dc.FrameRect(CRect(rcClient.left-1,rcClient.top - 1,rcClient.right + 1,rcClient.bottom+1), &brSide);
		dc.FrameRect(rcClient, &brSide);

	}
	else
	{//画不选择边框
		//CBrush brSide(RGB(73, 128, 211));
	//	dc.FrameRect(CRect(rcClient.left+1,rcClient.top+1,rcClient.right-1,rcClient.bottom-1), &brSide);
	//	dc.FrameRect(rcClient, &brSide);
	}
}

HBRUSH CSinglePlayView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//   在此更改 DC 的任何属性
	if (nCtlColor== CTLCOLOR_DLG&&this->m_byPlayType==0)
	{
		pDC->SetBkColor(RGB(113,111,100));
		pDC->SetBkMode(TRANSPARENT);
		DeleteObject(m_brush);
		m_brush=CreateSolidBrush(RGB(113,111,100));
		hbr = m_brush;
	}
	if(pWnd->GetDlgCtrlID() == IDC_TEXT_LEFT||
		pWnd->GetDlgCtrlID() == IDC_TEXT_MID||
		pWnd->GetDlgCtrlID() == IDC_TEXT_RIGHT)
	{
		pDC->SetBkColor(RGB(216,216,216));
		pDC->SetBkMode(TRANSPARENT);
		DeleteObject(m_brush);
		m_brush=CreateSolidBrush(RGB(216,216,216));
		hbr = m_brush;
	}
	 if(pWnd->GetDlgCtrlID() == IDC_STATIC_PICTURE  )
	{
		pDC->SetBkColor(RGB(192,213,244));
		pDC->SetBkMode(TRANSPARENT);
		DeleteObject(m_brush);
		m_brush=CreateSolidBrush(RGB(192,213,244));
		hbr = m_brush;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CSinglePlayView::SetCtrlPosition()
{
	CRect rcClient;
	GetClientRect(rcClient);//获得窗口大小
	int iLeft, iTop, iWidth, iHeight;
	iLeft = rcClient.left + iLineWidth ;
	iTop =  rcClient.top + iLineWidth ;
	iWidth = rcClient.right - 2*iLineWidth;
	iHeight = rcClient.bottom -2*iLineWidth;
	if (!bIsShowBottom)//不显示工具条
	{
		GetDlgItem(IDC_STATIC_PICTURE)->MoveWindow(iLeft,iTop,iWidth,iHeight);//移动图画控件
		GetDlgItem(IDC_TEXT_LEFT)->ShowWindow(FALSE);
		GetDlgItem(IDC_TEXT_MID)->ShowWindow(FALSE);
		GetDlgItem(IDC_TEXT_RIGHT)->ShowWindow(FALSE);

	}
	else
	{
		iHeight = rcClient.bottom - iLineWidth - iBottomHeight;
		GetDlgItem(IDC_STATIC_PICTURE)->MoveWindow(iLeft,iTop,iWidth,iHeight);//移动图画控件

		CString strText;
		GetDlgItem(IDC_TEXT_RIGHT)->GetWindowText(strText);
		iTop =  rcClient.bottom - iLineWidth - iBottomHeight ;

		TEXTMETRIC FtMsg;
		CDC *pDC = GetDlgItem(IDC_TEXT_RIGHT)->GetWindowDC();
		//获得文本长度
		BOOL bl = ::GetTextMetrics(pDC->m_hDC, &FtMsg);
		if (bl)
		{
			int iLen = FtMsg.tmAveCharWidth*strText.GetLength();
			if (m_byPlayType==2)
			{
				iLen = 0;
			}
			GetDlgItem(IDC_TEXT_RIGHT)->MoveWindow(iWidth-iLen,iTop,iLen,iBottomHeight);
			GetDlgItem(IDC_TEXT_RIGHT)->SetWindowText(strText);
			GetDlgItem(IDC_TEXT_RIGHT)->ShowWindow(TRUE);

			GetDlgItem(IDC_TEXT_LEFT)->MoveWindow(iLeft,iTop,iWidth-iLen,iBottomHeight);
			GetDlgItem(IDC_TEXT_LEFT)->SetWindowText(_T("hahahahahahha"));
			GetDlgItem(IDC_TEXT_LEFT)->ShowWindow(TRUE);		   
			GetDlgItem(IDC_TEXT_MID)->ShowWindow(FALSE);
		}
		ReleaseDC(pDC);
	}
}

void CSinglePlayView::MovePlayView( int iLeft, int iTop, int iWidth, int iHeigth )
{
	this->MoveWindow(iLeft,iTop,iWidth,iHeigth);
	SetCtrlPosition();
}

void CSinglePlayView::MovePlayView( CRect rect, BOOL bRepaint /*=TRUE*/ )
{
	this->MoveWindow(rect,bRepaint);
	SetCtrlPosition();
}

HWND CSinglePlayView::GetPlayHandle()
{
	return GetDlgItem(IDC_STATIC_PICTURE)->GetSafeHwnd();
}

void CSinglePlayView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialog::OnRButtonDown(nFlags, point);
}

BOOL CSinglePlayView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSinglePlayView::OnLButtonDown(UINT nFlags, CPoint point)
{
	
	SetSelectState(TRUE);
	//ShowCtrlState();
	CDialog::OnLButtonDown(nFlags, point);
}

void CSinglePlayView::SetSelectState( BOOL IsSelect )
{
	bIsSelect = IsSelect ;
	CClientDC dc(this); 
	CRect rcClient;
	GetClientRect(rcClient);
	if ( bIsSelect )
	{//画选择边框
		CBrush brSide(RGB(90, 237, 9));
		dc.FrameRect(CRect(rcClient.left-1,rcClient.top - 1,rcClient.right + 1,rcClient.bottom+1), &brSide);
		dc.FrameRect(rcClient, &brSide);
		//CMyViewDlg *pParentWnd = GetViewPointer();
		//pParentWnd->StopTalkAndListen();
	}
	else
	{//画不选择边框
		//CBrush brSide(RGB(73, 128, 211));
		//dc.FrameRect(CRect(rcClient.left+1,rcClient.top+1,rcClient.right-1,rcClient.bottom-1), &brSide);
		//dc.FrameRect(rcClient, &brSide);
	}
}
