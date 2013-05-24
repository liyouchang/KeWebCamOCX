// WebCamPannel.cpp : 实现文件
//

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "WebCamPannel.h"
#include "Common.h"
#include "memdc.h"
#include "PopupPannel.h"
#define NORMAL_FontColor	RGB(255, 255, 255)	// White
#define TOGGLED_FontColor	RGB(255, 255, 25)	// White Gray
#define NORMAL_FrameColor	RGB(255, 255, 255)	// Black
#define TOGGLED_FrameColor	RGB(200, 200, 200)	// Silver

extern CKeWebCamOCXApp theApp;

// CWebCamPannel 对话框

IMPLEMENT_DYNAMIC(CWebCamPannel, CDialog)

CWebCamPannel::CWebCamPannel(CWnd* pParent /*=NULL*/)
	: CDialog(CWebCamPannel::IDD, pParent)
{
	m_nRotation_01		= -1;
	m_nRotation_04		= -4;
	m_nRotation_09		= -7;
	m_nRotation_06		= -5;
	m_nRotation_08		= -8;
	m_nRotation_10		= -6;
	m_bFullScreen = FALSE;
	m_nActiveCamera =0;
	m_brBkgnd.CreateSolidBrush(clBlack);
	LOG_DEBUG("CWebCamPannel::CWebCamPannel()");
}

CWebCamPannel::~CWebCamPannel()
{
	LOG_DEBUG("~CWebCamPannel()");
}

void CWebCamPannel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMERA0, m_camarray[0]);
	DDX_Control(pDX, IDC_CAMERA1, m_camarray[1]);
	DDX_Control(pDX, IDC_CAMERA2, m_camarray[2]);
	DDX_Control(pDX, IDC_CAMERA3, m_camarray[3]);
	DDX_Control(pDX, IDC_CAMERA4, m_camarray[4]);
	DDX_Control(pDX, IDC_CAMERA5, m_camarray[5]);
	DDX_Control(pDX, IDC_CAMERA6, m_camarray[6]);
	DDX_Control(pDX, IDC_CAMERA7, m_camarray[7]);
	DDX_Control(pDX, IDC_CAMERA8, m_camarray[8]);
	DDX_Control(pDX, IDC_CAMERA9, m_camarray[9]);
	DDX_Control(pDX, IDC_CAMERA10, m_camarray[10]);
	DDX_Control(pDX, IDC_CAMERA11, m_camarray[11]);
	DDX_Control(pDX, IDC_CAMERA12, m_camarray[12]);
	DDX_Control(pDX, IDC_CAMERA13, m_camarray[13]);
	DDX_Control(pDX, IDC_CAMERA14, m_camarray[14]);
	DDX_Control(pDX, IDC_CAMERA15, m_camarray[15]);
}


BEGIN_MESSAGE_MAP(CWebCamPannel, CDialog)
	ON_STN_CLICKED(IDC_CAMERA0, &CWebCamPannel::OnStnClickedCamera0)
	ON_WM_DESTROY()
	ON_WM_PAINT()

	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()

ON_WM_CTLCOLOR()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

COLORREF CWebCamPannel::GetCamFontColor()
{
	return DIV_TOGGLED == m_FnDivision ? TOGGLED_FontColor : NORMAL_FontColor;
}

void CWebCamPannel::InvalidateAll()
{
	for(int i=0; i < CAM_MAX; i++)
		m_camarray[i].Invalidate();
	//DrawCameraFrame();
	this->Invalidate();
}

CRect CWebCamPannel::GetFullPaintRect()
{
	CRect r(0, 0, m_nFullPaintHeight, m_nFullPaintWidth);
	return r;
}

// CWebCamPannel 消息处理程序

void CWebCamPannel::OnStnClickedCamera0()
{
	// TODO: 在此添加控件通知处理程序代码
}

BOOL CWebCamPannel::OnInitDialog()
{
	CDialog::OnInitDialog();
	for(int i = 0; i < CAM_MAX; i++)
	{
		m_camarray[i].m_nCamNo		 = i;
		m_camarray[i].SetOwner(this);
		m_camarray[i].m_bFull = FALSE;
	}
	SetPlayDivision(4);
	LOG_DEBUG("Pannel OnInitDialog end " << GetTickCount());
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CWebCamPannel::OnDestroy()
{

	CDialog::OnDestroy();
}

void CWebCamPannel::MoveCameraRect( int nMode )
{
	//LOG_DEBUG("GetCameraRect Row="<< RowCount<<" Col="<<ColCount);
	if (nMode == 0)
	{
		nMode = m_FnDivision;
	}
	CRect	rect;
	int CamID = 0;
	int nBottomMargin = 1;
	int nRightMargin  = 1;
	int nLeftMargin = 1;
	int nTopMargin = 1;
	int nSplitLineWidth = 1;
	if (nMode == DIV_PLAYER)
	{
		 nBottomMargin = 0;
		 nRightMargin  = 0;
		 nLeftMargin = 0;
		 nTopMargin = 0;
	}
	int dx, dy;
	GetClientRect(rect);
	dx = (rect.Width()  - nLeftMargin - nRightMargin)  / RowCount;
	dy = (rect.Height() - nTopMargin - nBottomMargin) / ColCount;
	m_nFullPaintWidth  = rect.Height() -nLeftMargin - nBottomMargin;
	m_nFullPaintHeight = rect.Width()  -nTopMargin - nRightMargin;
	
	for(int y = 0; y < RowCount; ++y)
	{
		for(int x = 0; x < ColCount; ++x)
		{
			CRect destRect(x*dx +nLeftMargin , y*dy + nTopMargin, (x+1)*dx  +nLeftMargin , (y+1)*dy+ nTopMargin);
			switch(nMode)
			{
			case DIV_CH6:
				if(x == 0 && y == 0)
				{
					destRect = CRect(x*dx+nLeftMargin, y*dy + nTopMargin, 
						(x+2)*dx+nLeftMargin, (y+2)*dy+nTopMargin);
				}
				else if(x <= 1 && y <= 1)
				{
				}
				break;
			case DIV_CH8:
				if(x == 0 && y == 0)
				{
					destRect = CRect(x*dx+nLeftMargin, y*dy+ nTopMargin,
						(x+3)*dx+nLeftMargin, (y+3)*dy+ nTopMargin);
				}
				else if(x >=0 && y >= 0 && x <= 2 && y <= 2)
				{
				}
				break;
			case DIV_CH10:
				if(x == 0 && y == 0)
				{
					destRect = CRect(x*dx+nLeftMargin, y*dy+ nTopMargin, 
						(x+2)*dx+nLeftMargin, (y+2)*dy+ nTopMargin);
				}
				if(x == 2 && y == 0)
				{
					destRect = CRect(x*dx+nLeftMargin, y*dy+ nTopMargin, 
						(x+2)*dx+nLeftMargin, (y+2)*dy+ nTopMargin);
				}
				break;
			case DIV_CH13:
				if(x == 1 && y == 1)
				{
					destRect = CRect(x*dx+nLeftMargin, y*dy+ nTopMargin, 
						(x+2)*dx+nLeftMargin, (y+2)*dy+ nTopMargin);
				}
				else if(x >= 1 && y >= 1 && x <=2 && y <= 2)
				{
				}
				break;
			}
			
			CRect showRect(destRect.left+nSplitLineWidth,destRect.top + nSplitLineWidth,
				destRect.right - nSplitLineWidth,destRect.bottom - nSplitLineWidth);

			if(nMode == DIV_TOGGLED)	// One channel db-click 
			{
				if( m_camarray[m_nActiveCamera].m_bDrawable )
				{
					m_camarray[m_nActiveCamera].MoveWindow(showRect, m_camarray[m_nActiveCamera].m_bDrawable);
				}
			}
			else
			{
				if( m_camarray[x + y*RowCount].m_bDrawable )
				{
					m_camarray[x + y*RowCount].MoveWindow(showRect, m_camarray[x + y*RowCount].m_bDrawable);
				}
			}
		}
	}
}

void CWebCamPannel::DrawCameraFrame( CDC *pDC)
{
	//MoveCameraRect();
	if (pDC == NULL)
	{
		CClientDC dc(this);
		DrawFrame(&dc);
	}
	else
	{
		DrawFrame(pDC);
	}
}

void CWebCamPannel::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	MoveCameraRect();
	DrawCameraFrame(&dc);
	CDialog::OnPaint();
}

void CWebCamPannel::SetPlayDivision( int nDiv )
{
	if (nDiv != DIV_CH1 && nDiv != DIV_CH4 && nDiv != DIV_CH6 &&
		nDiv != DIV_CH8 && nDiv != DIV_TOGGLED && nDiv != DIV_PLAYER)
	{
		LOG_WARN("Not supported division function --"<<nDiv);
		return;
	}
	TRACE1("SetPlayDivision start %d\n",GetTickCount());
	m_FnDivision	= nDiv;
	if (m_FnDivision == DIV_PLAYER)
	{
		SetCamDrawable(1);
		
	}
	else
	{
		SetCamDrawable();
		if (nDiv != DIV_TOGGLED)
		{
			MoveCameraPlay();
		}

		MoveCameraRect();
		DrawCameraFrame();
	}
	this->Invalidate();
	TRACE1("SetPlayDivision end %d\n",GetTickCount());
}

void CWebCamPannel::DrawFrame(CDC *pDC)
{
	CRect	rect;
	CBrush	brush;
	brush.CreateSolidBrush(TOGGLED_FrameColor);
	for(int i = 0; i < CAM_MAX; i++)
	{
		if (m_camarray[i].m_bDrawable)
		{
			m_camarray[i].GetWindowRect(rect);
			ScreenToClient(rect);
			pDC->FrameRect(CRect(rect.left - 1,rect.top -1,rect.right+1,rect.bottom+1), &brush);
			pDC->FrameRect(CRect(rect.left - 2,rect.top -2,rect.right+2,rect.bottom+2), &brush);
		}
	}
	if (m_camarray[m_nActiveCamera].m_bDrawable)
	{
		CBrush selectSide(clBlue);
		m_camarray[m_nActiveCamera].GetWindowRect(rect);
		ScreenToClient(rect);
		pDC->FrameRect(CRect(rect.left - 1,rect.top -1,rect.right+1,rect.bottom+1), &selectSide);
		pDC->FrameRect(CRect(rect.left - 2,rect.top -2,rect.right+2,rect.bottom+2), &selectSide);
	}
}



//************************************
// Method:    GetOnePlayer
// FullName:  CWebCamPannel::GetOnePlayer
// Access:    public 
// Returns:   CMyAVPlayer	*
// Qualifier:
// Parameter: int cameraID
// Parameter: int * isPlaying
//************************************
COneCamera 	* CWebCamPannel::GetOnePlayer( int cameraID,int * isPlaying /*= NULL*/ )
{
	int nIndex;
	int nFind = 0;
	for (nIndex=0;nIndex< CAM_MAX;nIndex++)
	{
		if (m_camarray[nIndex].m_cameraID == cameraID)
		{
			break;
		}
	}
	if (nIndex < CAM_MAX)
	{
		SetActiveCamera(nIndex);
		nFind = 1;
	}
	if (isPlaying != NULL)
	{
		*isPlaying = nFind;
	}
	//DrawAllCameraImages();
	m_camarray[m_nActiveCamera].m_cameraID = cameraID;
	return &m_camarray[m_nActiveCamera];
}


BOOL CWebCamPannel::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// 直接返回以保持设置的鼠标样式
	return TRUE;
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CWebCamPannel::SetActiveCamera( int nCamNo )
{
	if (m_nActiveCamera != nCamNo)
	{
		m_nActiveCamera = nCamNo;
		CamStatusReport report;
		report.cameraID = m_camarray[nCamNo].m_cameraID;
		report.reportType = 1;
		DrawCameraFrame();
		theApp.g_pMainWnd->SendMessage(WM_CAMSTATUSREPORT,0,(LPARAM)&report);
	}
}
//返回拥有cameraID的镜头，若cameraID为0，则返回当前选中的镜头
COneCamera * CWebCamPannel::GetCamera( int cameraID )
{
	if (cameraID == 0)
	{
		return &m_camarray[m_nActiveCamera];
	}
	int nIndex;
	for (nIndex=0;nIndex< CAM_MAX;nIndex++)
	{
		if (m_camarray[nIndex].m_cameraID == cameraID)
		{
			break;
		}
	}
	if (nIndex < CAM_MAX)
	{
		return &m_camarray[nIndex];
	}
	return NULL;
}

void CWebCamPannel::CopyPannel( CWebCamPannel * pannel )
{
	this->SetPlayDivision(pannel->m_FnDivision);
	this->m_nActiveCamera = pannel->m_nActiveCamera;
	this->m_bFullScreen = pannel->m_bFullScreen;
	for(int i = 0; i < CAM_MAX; i++)
	{
		m_camarray[i].SwapVideo(&pannel->m_camarray[i]);
	}
}

void CWebCamPannel::ShowFullScreen()
{
	m_bFullScreen=TRUE;   //设置全屏显示标志为TRUE
	CPopupPannel dlg;
	dlg.m_pToFullPannel = this;
	theApp.g_PlayWnd = &dlg.m_pannel;
	dlg.DoModal();
	this->CopyPannel(&dlg.m_pannel);
	theApp.g_PlayWnd = this;
	m_bFullScreen=FALSE;   

}

void CWebCamPannel::OnLButtonUp(UINT nFlags, CPoint point)
{
	::SetCursor(::LoadCursor(NULL,IDC_ARROW));
	CDialog::OnLButtonUp(nFlags, point);
}
//截取所有Camera控件的WM_LBUTTONUP消息
bool CWebCamPannel::PeekLButtonUpMsg()
{
	MSG msg;
	for(int i = 0; i < CAM_MAX; i++)
	{
		::PeekMessage(&msg,m_camarray[i].GetSafeHwnd(),WM_LBUTTONUP,WM_LBUTTONUP,PM_NOREMOVE);
		//随意点击而已,返回
		if( msg.message == WM_LBUTTONUP )
			return true;
	}
	return false;
	
}

void CWebCamPannel::StopAllRTPlay()
{
	for(int i = 0; i < CAM_MAX; i++)
	{
		m_camarray[i].StopRTPlay(false);
	}
}

void CWebCamPannel::MoveCameraPlay()
{
	for(int i = 0; i < CAM_MAX; i++)
	{
		if (!m_camarray[i].m_bDrawable && m_camarray[i].IsPlaying())
		{
			for(int j = 0; j < CAM_MAX; j++)
			{
				if (m_camarray[j].m_bDrawable && !m_camarray[j].IsPlaying())
				{
					m_camarray[j].SwapVideo(&m_camarray[i]);
					break;
				}
			}
		}
		
	}
}

void CWebCamPannel::SetCamDrawable( int nDiv /*= 0*/ )
{
	if (nDiv == 0)
	{
		nDiv = m_FnDivision;
	}
	int	nRow	= 0;
	int	nCamNo	= 0;
	int	nVirtualDisp	= 0;
	int	mod_chid = 0;

	for(int i = 0; i < CAM_MAX; i++)
	{
		m_camarray[i].m_bDrawable	 = FALSE;
		m_camarray[i].m_bFull	 = FALSE;
	}
	switch(nDiv)
	{
	case DIV_CH1:
		nRow = 1;	
		m_camarray[0].m_bDrawable = TRUE;
		break;
	case DIV_CH4:
		nRow = 2;
		for(int i = 0; i < DIV_CH4; i++)
			m_camarray[i].m_bDrawable = TRUE;
		break;
	case DIV_CH6:
		nRow = 3;
		m_camarray[0].m_bDrawable = TRUE;
		m_camarray[2].m_bDrawable = TRUE;
		for(int i = 5; i < 9; i++)
			m_camarray[i].m_bDrawable = TRUE;
		break;

	case DIV_CH8:
		nRow = 4;
		m_camarray[0].m_bDrawable  = TRUE;
		m_camarray[3].m_bDrawable  = TRUE;
		m_camarray[7].m_bDrawable  = TRUE;
		m_camarray[11].m_bDrawable = TRUE;
		for(int i = 12; i < IMAGE_MAX_CH16; i++)
			m_camarray[i].m_bDrawable = TRUE;
		break;
	case DIV_CH9:
		nRow = 3;
		for(int i = 0; i < DIV_CH9; i++)
			m_camarray[i].m_bDrawable = TRUE;
		break;

	case DIV_CH10:
		nRow = 4;
		m_camarray[0].m_bDrawable = TRUE;
		m_camarray[2].m_bDrawable = TRUE;
		for(int i = 8; i < DIV_CH16; i++)
			m_camarray[i].m_bDrawable = TRUE;
		break;
	case DIV_CH13:  
		nRow = 4;
		nVirtualDisp = 1;
		for(int i = 0; i < IMAGE_MAX_CH16; i++)
			m_camarray[i].m_bDrawable = TRUE;
		m_camarray[6].m_bDrawable = FALSE;
		m_camarray[9].m_bDrawable = FALSE;
		m_camarray[10].m_bDrawable = FALSE;
		break;
	case DIV_CH16: 
		nRow = 4;
		for(int i = 0; i < DIV_CH16; i++)
			m_camarray[i].m_bDrawable = TRUE;
		break;
	case DIV_TOGGLED:
		nRow = 1;
		m_camarray[m_nActiveCamera].m_bDrawable = TRUE;
		m_camarray[m_nActiveCamera].m_bFull = TRUE;
		break;
	default:
		
		return;
	}

	if (nRow >= 1 && nRow <= DIVISION_MAX_CH16)
	{
		RowCount = nRow;
		ColCount = nRow;
	}

	for(int i=0; i < CAM_MAX; ++i)
	{
		if(m_camarray[i].m_bDrawable)
		{
			m_camarray[i].ShowWindow(SW_SHOW);
		}
		else
		{
			m_camarray[i].ShowWindow(SW_HIDE);
		}
	}

	//设置选中镜头，选中镜头有可能不在显示镜头中
	if (m_camarray[m_nActiveCamera].m_bDrawable == FALSE)
	{
		SetActiveCamera(0);	
	}

}



HBRUSH CWebCamPannel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	
	// 设置背景色，和Camera的背景色一致，以防止闪屏
	hbr = (HBRUSH)m_brBkgnd.GetSafeHandle();
	return hbr;
}

void CWebCamPannel::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialog::OnMouseMove(nFlags, point);
}

void CWebCamPannel::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}
