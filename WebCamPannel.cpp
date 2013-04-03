// WebCamPannel.cpp : 实现文件
//

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "WebCamPannel.h"
#include "Common.h"
#include "memdc.h"
#include "command.h"

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
	m_nWatermark		= FALSE;
	m_nRotation_01		= -1;
	m_nRotation_04		= -4;
	m_nRotation_09		= -7;
	m_nRotation_06		= -5;
	m_nRotation_08		= -8;
	m_nRotation_10		= -6;
	m_bDivision			= FALSE;
	m_nMaxChannel = 16;
	m_nActiveCamera =16;
}

CWebCamPannel::~CWebCamPannel()
{
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
END_MESSAGE_MAP()

COLORREF CWebCamPannel::GetCamFontColor()
{
	return DIV_TOGGLED == FnDivision ? TOGGLED_FontColor : NORMAL_FontColor;
}

void CWebCamPannel::InvalidateAll()
{
	for(int i=0; i < CAM_MAX; i++)
		m_camarray[i].Invalidate();
	DrawAllCameraImages();
}

CRect CWebCamPannel::GetFullPaintRect()
{
	CRect r(0, 0, m_nFullPaintHeight, m_nFullPaintWidth);
	return r;
}

void CWebCamPannel::SendMsg( UINT nMajorType, UINT nSubType, UINT nMsgType, LPARAM lParam, UINT nSendFlag/*=0*/ )
{

}


// CWebCamPannel 消息处理程序

void CWebCamPannel::OnStnClickedCamera0()
{
	// TODO: 在此添加控件通知处理程序代码
}

BOOL CWebCamPannel::OnInitDialog()
{
	CDialog::OnInitDialog();

	HBITMAP		hBitmap;
	CBitmap		bmpObj;

	bmpObj.LoadBitmap(IDB_DIVLOGO);
	hBitmap = (HBITMAP)bmpObj.m_hObject;

	//	temp_image->Load(IDB_DIVLOGO, CXIMAGE_FORMAT_BMP);
	temp_image.CreateFromHBITMAP(hBitmap);

	for(int i = 0; i < CAM_MAX; i++)
	{
		m_camarray[i].SetOwner(this);
		m_camarray[i].m_nCamNo		 = i;
		m_camarray[i].m_bEvent		 = 0;
		m_camarray[i].m_bSaveToAVI	 = FALSE;
		//m_camarray[i].m_AVI			 = NULL;

		if(i < theApp.m_nMAXCHANNEL)
			m_camarray[i].m_strCaption.Format(_T("CamNo %02d "), i+1);
		else
			m_camarray[i].m_strCaption.Empty();

		m_camarray[i].m_strDateTime.Empty();
		m_camarray[i].m_bFull		= FALSE;
		//		m_Imagearray[i].Load(DIVLOG, CXIMAGE_FORMAT_BMP);
		m_Imagearray[i].Copy(temp_image);
		m_camarray[i].RegisterDrop();
	}
//	SetPlayDivision(4);
	m_nWatermark	= 0;


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CWebCamPannel::OnDestroy()
{

	CDialog::OnDestroy();
}

void CWebCamPannel::GetCameraRect( int nMode )
{
	CRect	rect;
	int		CamID = 0;
	int		nBottomMargin = 1;
	int		nRightMargin  = 1;
	int nLeftMargin = 1;
	int nTopMargin = 1;
	int nSplitLineWidth = 1;
	int		dx, dy;
	
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
					m_camarray[m_nActiveCamera].MoveWindows(showRect, m_camarray[m_nActiveCamera].m_bDrawable);
				}
			}
			else
			{

				if( m_camarray[x + y*RowCount].m_bDrawable )
				{
					m_camarray[x + y*RowCount].MoveWindows(showRect, m_camarray[x + y*RowCount].m_bDrawable);
				}
			}
		}
	}

	
}

void CWebCamPannel::DrawAllCameraImages( CDC *pDC)
{
	GetCameraRect(FnDivision);
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

	CRect rcClient;
	GetClientRect(rcClient);
	CBrush m_brush(NORMAL_FrameColor);
	dc.FillRect(rcClient,&m_brush);
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()


	DrawAllCameraImages(&dc);
	
	CDialog::OnPaint();
}

void CWebCamPannel::SetPlayDivision( int nDiv )
{
	int			nRow			= -1;
	int			nCamNo			= 0;
	int			nVirtualDisp	= 0;
	int			i;
	int			mod_chid		= 0;
	long		value			= MAKELONG(full_screen_on, 0);

	for(i = 0; i < CAM_MAX; i++)
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
		for(i = 0; i < DIV_CH4; i++)
			m_camarray[i].m_bDrawable = TRUE;
		break;

	case DIV_CH6:
		nRow = 3;
		m_camarray[0].m_bDrawable = TRUE;
		m_camarray[2].m_bDrawable = TRUE;
		for(i = 5; i < 9; i++)
			m_camarray[i].m_bDrawable = TRUE;
		break;

	case DIV_CH8:
		nRow = 4;
		m_camarray[0].m_bDrawable  = TRUE;
		m_camarray[3].m_bDrawable  = TRUE;
		m_camarray[7].m_bDrawable  = TRUE;
		m_camarray[11].m_bDrawable = TRUE;
		for(i = 12; i < IMAGE_MAX_CH16; i++)
			m_camarray[i].m_bDrawable = TRUE;
		break;

	case DIV_CH9:
		nRow = 3;
		for(i = 0; i < DIV_CH9; i++)
			m_camarray[i].m_bDrawable = TRUE;
		break;

	case DIV_CH10:
		nRow = 4;
		m_camarray[0].m_bDrawable = TRUE;
		m_camarray[2].m_bDrawable = TRUE;
		for(i = 8; i < DIV_CH16; i++)
			m_camarray[i].m_bDrawable = TRUE;
		break;

	case DIV_CH13:  
		nRow = 4;
		nVirtualDisp = 1;
		for(i = 0; i < IMAGE_MAX_CH16; i++)
			m_camarray[i].m_bDrawable = TRUE;
		m_camarray[6].m_bDrawable = FALSE;
		m_camarray[9].m_bDrawable = FALSE;
		m_camarray[10].m_bDrawable = FALSE;
		break;

	case DIV_CH16: 
		nRow = 4;
		for(i = 0; i < DIV_CH16; i++)
			m_camarray[i].m_bDrawable = TRUE;
		break;

	case DIV_TOGGLED:
		nRow = 1;
		TRACE("m_nActiveCamera = %d\n", m_nActiveCamera);
		m_camarray[m_nActiveCamera].m_bDrawable = TRUE;
		m_camarray[m_nActiveCamera].m_bFull		= TRUE;

		break;
	}

	if (nRow >= 1 && nRow <= DIVISION_MAX_CH16)
	{
		RowCount = nRow;
		ColCount = nRow;
	}
	/* resize static windows */
	if(nDiv == DIV_CH13)
	{
		for(i=0; i < CAM_PER_DVR; ++i)
		{
			if(i == 5)
			{
				m_camarray[i].ShowWindow(SW_SHOW);
				//m_playViewArray[i].m_nChannel = 0;
				m_camarray[i].Invalidate();
				continue;
			}

			if(m_camarray[i].m_bDrawable)
			{
				m_camarray[i].ShowWindow(SW_SHOW);
			//	m_camarray[i].m_nChannel = j++;
			}
			else
				m_camarray[i].ShowWindow(SW_HIDE);
			m_camarray[i].Invalidate();
		}
	}
	else
	{
		for(i=0; i < CAM_PER_DVR; ++i)
		{
			if(m_camarray[i].m_bDrawable)
			{
				m_camarray[i].ShowWindow(SW_SHOW);
//				m_playViewArray[i].m_nChannel = j++;
			}
			else
				m_camarray[i].ShowWindow(SW_HIDE);
			m_camarray[i].Invalidate();
		}
	}
	
	FnDivision	= nDiv;
	m_bDivision	= TRUE;
	DrawAllCameraImages();
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

CMyAVPlayer* CWebCamPannel::GetOnePlayer( int videoID,int channelNo )
{
	return &m_camarray[0].m_AVIPlayer;
}
