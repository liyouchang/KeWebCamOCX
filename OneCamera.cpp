// OneCamera.cpp : implementation file
//

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "OneCamera.h"
#include "WebCamPannel.h"
#include "command.h"
#include "common.h"
#include "Lock.h"
//#include "infodvr.h"
//#include "DlgPrint.h"

#include "memdc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CKeWebCamOCXApp theApp;
/////////////////////////////////////////////////////////////////////////////
// COneCamera

COneCamera::COneCamera()
{
	m_pOwner		= NULL;
	m_nCamNo		= 0;
	m_bActive		= FALSE;
	m_bPress		= FALSE;
	m_cameraID = 0;
//  	m_videoID = 0;
//  	m_channelNo = 0;
	//menu.LoadMenu(IDR_MENU_CAMSELECT);
	menu.LoadMenu(IDR_MENU_CAMSELECT);
	m_CamRef.Brightness	= 0;
	m_CamRef.Hue		= 0;
	m_CamRef.Contrast	= 128;
	m_CamRef.Saturation	= 128;
	
	m_event.bAlarm = FALSE;
	m_event.bLoss	= FALSE;
	m_event.bMotion = FALSE;
	m_bIsPlaying = FALSE;
	m_MediaSocket = NULL;
	//InitCarmera();
	m_strCaption	= "";
	m_bFull			= FALSE;
	m_bDrawable = FALSE;
	m_AVIPlayer = new CMyAVPlayer;
}

COneCamera::~COneCamera()
{
	TRACE("~COneCamera[%d]\n", m_nCamNo);
	if (m_MediaSocket != NULL)
	{
		m_MediaSocket->CloseConnect();
		delete m_MediaSocket;
	}

	if (m_AVIPlayer != NULL)
	{
		m_AVIPlayer->CloseStream();
		delete m_AVIPlayer;
	}

}


BEGIN_MESSAGE_MAP(COneCamera, CStatic)
	//{{AFX_MSG_MAP(COneCamera)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENU_SAVEAS, OnMenuSaveas)
	ON_COMMAND(ID_MENU_RESET, OnMenuResetChannel)
	ON_COMMAND(ID_MENU_RESET_ALL, OnMenuResetAll)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MENU_SHOW_WATERMARK, OnMenuShowWatermark)
	
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_MENU_PLAY, &COneCamera::OnMenuPlay)
	ON_COMMAND(ID_MENU_SLOW, &COneCamera::OnMenuSlow)
	ON_COMMAND(ID_MENU_FAST, &COneCamera::OnMenuFast)

	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_MENU_FULLSCREEN, &COneCamera::OnMenuFullscreen)
	ON_COMMAND(ID_MENU_AllFullScreen, &COneCamera::OnMenuAllfullscreen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COneCamera message handlers

void COneCamera::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	
	if (m_AVIPlayer->IsPlaying() )//如果正在播放视频,则不刷新
	{
		return;
	}


	if(m_pOwner != NULL)
	{
		COLORREF clFrame;
		clFrame = (m_pOwner->m_nActiveCamera==m_nCamNo) ? clBlue:FLOAT_CAM_FRAME_COLOR;
		//CMemDC* pMemDC = NULL;
		//pMemDC = new CMemDC(&dc, this);

		CBrush	brush;
		brush.CreateSolidBrush(clBlack);
		//CBrush*	pOldBrush = (CBrush*)pMemDC->SelectObject(&brush); 	
		//if(pMemDC->GetSafeHdc() != NULL)
		//{
		dc.FillRect(rect, &brush);
			//dc->SelectObject(pOldBrush);
		//}
		//ReleaseDC(pMemDC);//test remove 2005.05.12
		//_SAFE_DELETE(pMemDC);
	}

	// Do not call CStatic::OnPaint() for painting messages
}



void COneCamera::LButtonDblClk(UINT nFlags, CPoint point)
{
	CRect	rect = m_pOwner->GetFullPaintRect();
	int		dx	 = rect.Width()  / 4;
	int		dy	 = rect.Height() / 4;
	if(m_pOwner->m_nActiveCamera == m_nCamNo)
	{
		if( m_pOwner->FnDivision == DIV_TOGGLED)
		{
			int	nRotation = 0;
			switch(m_pOwner->m_nToggledDivision)
			{
				case DIV_CH1:		nRotation = m_pOwner->m_nRotation_01;		break;
				case DIV_CH4:		nRotation = m_pOwner->m_nRotation_04;		break;
				case DIV_CH6:		nRotation = m_pOwner->m_nRotation_06;		break;
				case DIV_CH8:		nRotation = m_pOwner->m_nRotation_08;		break;
				case DIV_CH9:		nRotation = m_pOwner->m_nRotation_09;		break;
				case DIV_CH10:	nRotation = m_pOwner->m_nRotation_10;		break;
				default :		nRotation = -1;
			}
			m_pOwner->SetPlayDivision(m_pOwner->m_nToggledDivision);
			TRACE("m_nActiveCamera %d\n", m_pOwner->m_nActiveCamera);
			TRACE("DIV_TOGGLED= %d\n", m_pOwner->m_nToggledDivision);
		}
		else
		{
			if(m_pOwner->FnDivision == DIV_CH1)
				return;
			int	mapped_camno = m_nCamNo;//m_nCamNo;
			//m_pOwner->DrawAllCameraImages(DIV_CH1);
			// daeny2@ Channel ID Mapping
			if(theApp.m_nMAXCHANNEL == DIV_DEFAULT_CH9)
			{
				mapped_camno = new_MappiingID[m_nCamNo];
				if(m_nCamNo >= DIV_DEFAULT_CH9)
					return;
			}
			else if(theApp.m_nMAXCHANNEL == DIV_DEFAULT_CH8)
			{
				mapped_camno = new_MappiingID[m_nCamNo];
				if(m_nCamNo >= DIV_DEFAULT_CH8)
					return;
			}
			if(m_nRec_Type == 3)
			{
				if((mapped_camno % 2) != 0)
					mapped_camno--;
			}
			theApp.m_nCurrentMappingCamera = mapped_camno;
			TRACE("MAP CH-%d\n", mapped_camno);
			long	value = MAKELONG(full_screen_on, mapped_camno);
			CRect r;
			r = m_pOwner->GetFullPaintRect();
			//m_pOwner->m_nActiveCamera = m_nCamNo;
			m_pOwner->m_camarray[m_nCamNo].MoveWindows(r, TRUE);
			m_pOwner->m_nToggledDivision = m_pOwner->FnDivision;
			m_pOwner->SetPlayDivision(DIV_TOGGLED);
			TRACE("LButtonDblClk m_nCamNo = %d\n", m_nCamNo);
			TRACE("m_nActiveCamera %d\n", m_pOwner->m_nActiveCamera);
			TRACE("DIV_TOGGLED else = %d\n", m_pOwner->m_nToggledDivision);
		}
	}
}

void COneCamera::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( m_pOwner->m_nActiveCamera != m_nCamNo )
	{
		m_pOwner->SetActiveCamera(m_nCamNo);
		//m_pOwner->DrawAllCameraImages();
		m_pOwner->InvalidateAll();
	}
	CStatic::OnLButtonDown(nFlags, point);
	m_bDrag = FALSE;
	Sleep(200);
	if (m_pOwner->PeekLButtonUpMsg())
	{
		return;
	}
	m_bDrag = TRUE;
	::SetCursor(::LoadCursor(NULL,IDC_SIZEALL));
}


void COneCamera::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	LButtonDblClk(nFlags, point);
	CStatic::OnLButtonDblClk(nFlags, point);
}

void COneCamera::InitCarmera()
{
	
}

int COneCamera::GetCameraCanvasTextHeight()
{
	CDC* pDC = GetDC();
	int textheight = pDC->GetTextExtent(_T("ThisIsTestHeight00")).cy + 2;
	return textheight;
}

void __fastcall COneCamera::DrawCameraImage(CDC *pDC, CRect destFrame, CString caption, COLORREF FontColor, COLORREF FrameColor)
{
	ASSERT(!destFrame.IsRectNull() || !destFrame.IsRectEmpty());

	CRect dest(destFrame.left+1, destFrame.top+1, destFrame.right, destFrame.bottom);

	CMemDC* pMemDC = NULL;
	pDC = pMemDC = new CMemDC(pDC, &destFrame);

	CBrush	brush;
	brush.CreateSolidBrush(FrameColor);
	CBrush*	pOldBrush = (CBrush*)pDC->SelectObject(&brush);
	CFont	font;

// 	if(caption)
// 	{
// 		VERIFY(font.CreateFont(
// 		   12,                        // nHeight
// 		   0,                         // nWidth
// 		   0,                         // nEscapement
// 		   0,                         // nOrientation
// 		   FW_NORMAL,                 // nWeight
// 		   FALSE,                     // bItalic
// 		   FALSE,                     // bUnderline
// 		   0,                         // cStrikeOut
// 		   ANSI_CHARSET,              // nCharSet
// 		   OUT_DEFAULT_PRECIS,        // nOutPrecision
// 		   CLIP_DEFAULT_PRECIS,       // nClipPrecision
// 		   DEFAULT_QUALITY,           // nQuality
// 		   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
// 		   _T("Arial")));                 // lpszFacename
// 
// 		CFont* pOldFont	= pDC->SelectObject(&font);
// 		int textheight	= pDC->GetTextExtent(_T("thisisTESTjg")).cy + 2;
// 		dest = CRect(destFrame.left+1, destFrame.top+textheight, destFrame.right, destFrame.bottom);
// 		CRect stringArea	= destFrame;
// 		stringArea.bottom	= stringArea.top + textheight;
// 		pDC->FillRect(stringArea, &brush);
// 		pDC->SetBkMode(TRANSPARENT);
// 		pDC->SetTextColor(FontColor);
// 		pDC->TextOut(destFrame.left + 2, destFrame.top, caption);
// 		pDC->SelectObject(pOldFont);
// 	}

// 	if(m_pOwner->m_Imagearray[m_nCamNo].IsValid())
// 	{
// 		m_pOwner->m_Imagearray[m_nCamNo].Stretch(pDC->GetSafeHdc(), dest);
// 	}

	if(pDC->GetSafeHdc() != NULL)
	{
		pDC->FillRect(destFrame, &brush);
		pDC->FrameRect(destFrame, &brush);
		pDC->SelectObject(pOldBrush);
	}

	_SAFE_DELETE(pMemDC);

	font.DeleteObject();
	DeleteDC(pDC->m_hDC);
	DeleteObject(pDC->m_hDC);
	ReleaseDC(pDC);//test remove 2005.05.12
}

void COneCamera::MoveWindows(CRect rect, BOOL Visible)
{
	MoveWindow(rect, Visible);
}

void COneCamera::OnRButtonDown(UINT nFlags, CPoint point) 
{

	if( m_pOwner->m_nActiveCamera != m_nCamNo )
	{
		m_pOwner->SetActiveCamera(m_nCamNo);
		//m_pOwner->DrawAllCameraImages();
		m_pOwner->InvalidateAll();
	}

	CURSORINFO ci;
	CString ciInfo;
	ci.cbSize = sizeof(CURSORINFO);
	CRect			rect;
	GetWindowRect(rect);
	int x = rect.TopLeft().x + point.x;
	int y = rect.TopLeft().y + point.y;
	TRACE(TEXT("menu x= %d, y=%d\n"), x, y);
	
	if (m_pOwner->m_bFullScreen)
	{
		menu.GetSubMenu(0)->DeleteMenu(ID_MENU_AllFullScreen,MF_BYCOMMAND );
	}
	if(!IsPlaying())
	{
		menu.GetSubMenu(0)->EnableMenuItem(ID_MENU_RESET,MF_BYCOMMAND|MF_GRAYED);  
	}
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, x, y, this);

	CStatic::OnRButtonDown(nFlags, point);
	
}


void COneCamera::OnMenuResetChannel()
{
	theApp.g_pMainWnd->SendMessage(WM_RTVIDEOSTOP,m_cameraID);
	//StopRTPlay(false);
	//m_pOwner->m_Imagearray[m_nCamNo].Destroy();
	//m_pOwner->m_Imagearray[m_nCamNo].Copy(m_pOwner->temp_image);
	m_pOwner->m_camarray[m_nCamNo].m_bLoss = TRUE;
	m_pOwner->m_camarray[m_nCamNo].m_strDateTime.Empty();
	m_pOwner->m_camarray[m_nCamNo].Invalidate();
}

void COneCamera::OnMenuShowWatermark() 
{
	// Show or Hide Watermark
	m_pOwner->m_nWatermark = ~m_pOwner->m_nWatermark;
}


void COneCamera::OnMenuResetAll()
{
	for(int i = 0; i < CAM_MAX; i++)
	{
		//m_pOwner->m_Imagearray[i].Destroy();
		//m_pOwner->m_Imagearray[i].Copy(m_pOwner->temp_image);
		m_pOwner->m_camarray[i].m_bLoss = TRUE;
		m_pOwner->m_camarray[i].m_strDateTime.Empty();
		m_pOwner->m_camarray[i].Invalidate();
	}
}

void COneCamera::OnMenuSaveas() 
{
	/*
	WCHAR		file_name[16];
	static WCHAR BASED_CODE OpenFilter[] = _T("JPG Files (*.jpg)|*.jpg; *.jpg||");

	swprintf(file_name, _T("image_%02d.jpg"), m_nCamNo+1);

	CFileDialog FileOpenDialog(
		FALSE,
		NULL,
		file_name,
		OFN_FILEMUSTEXIST |	OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
		OpenFilter,
		this);

	if(FileOpenDialog.DoModal() == IDOK)
	{
		CString strFilePath = FileOpenDialog.GetPathName();

#if 1
		// daeny2@ 2005/08/17 Image resize
		CxImage* iDst = new CxImage;

		int		nWidth	= 720;
		int		nHeight = 480;

		//if(m_pOwner->m_Imagearray[m_nCamNo].GetWidth() > 600)
		//{
		//	nWidth	= 720;
			//nHeight = theApp.m_nVideoFormat == NTSC ? 480 : 576;
		//}
		//else
		//{
		//	nWidth	= 320;
		//	nHeight = theApp.m_nVideoFormat == NTSC ? 240 : 288;
		//}

		//m_pOwner->m_Imagearray[m_nCamNo].Resample(nWidth, nHeight, 1, iDst);
		//iDst->Save(strFilePath, CXIMAGE_FORMAT_JPG);
		_SAFE_DELETE(iDst);
#else
		m_pOwner->m_Imagearray[m_nCamNo].Save(strFilePath, CXIMAGE_FORMAT_JPG);
#endif
	}
	*/
}

void COneCamera::OnMenuSaveall() 
{
	/*
	static WCHAR		BASED_CODE OpenFilter[] = _T("JPG Files (*.jpg)|*.jpg; *.jpg||");
	CString			strIndex;

	CFileDialog FileOpenDialog(	FALSE,
								NULL,
								_T("image"),
								OFN_FILEMUSTEXIST |	OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
								OpenFilter,
								this );

	if(FileOpenDialog.DoModal() == IDOK)
	{
		for(int i = 0; i < theApp.m_nMAXCHANNEL; i++)
		{
			CString strFilePath = FileOpenDialog.GetPathName();
			strIndex.Format(_T("_%02d.jpg"), i+1);

#if 1
			// daeny2@ 2005/08/17 Image resize
			CxImage* iDst = new CxImage;

			int		nWidth	= 720;
			int		nHeight = 480;

			if(m_pOwner->m_Imagearray[i].GetWidth() > 600)
			{
				nWidth	= 720;
				//nHeight = theApp.m_nVideoFormat == NTSC ? 480 : 576;
			}
			else
			{
				nWidth	= 320;
				//nHeight = theApp.m_nVideoFormat == NTSC ? 240 : 288;
			}

			m_pOwner->m_Imagearray[i].Resample(nWidth, nHeight, 1, iDst);
			iDst->Save(strFilePath + strIndex, CXIMAGE_FORMAT_JPG);
			_SAFE_DELETE(iDst);
#else
			m_pOwner->m_Imagearray[i].Save(strFilePath + strIndex, CXIMAGE_FORMAT_JPG);
#endif
		}
	}
	*/
}

void COneCamera::ResetAVI()
{
	
}


void COneCamera::OnMenuPlay()
{
	m_AVIPlayer->PlayFile(this->GetSafeHwnd());
	m_bIsPlaying = TRUE;
}

void COneCamera::OnMenuSlow()
{
	// TODO: 在此添加命令处理程序代码
}

void COneCamera::OnMenuFast()
{
	// TODO: 在此添加命令处理程序代码
}

void COneCamera::SetOwner( CWebCamPannel* pOwner )
{
	m_pOwner = pOwner;
	this->m_AVIPlayer->SetPlayWnd(this->GetSafeHwnd(),true);
}

void COneCamera::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( m_pOwner->m_nActiveCamera != m_nCamNo )
	{
 		COneCamera * tmpCam = &m_pOwner->m_camarray[m_pOwner->m_nActiveCamera];
		if (tmpCam->m_bDrag)
		{
			SwapVideo(tmpCam);
			m_pOwner->SetActiveCamera(m_nCamNo);
			m_pOwner->InvalidateAll();
		}
	}
	::SetCursor(::LoadCursor(NULL,IDC_ARROW));
	CStatic::OnLButtonUp(nFlags, point);
}

void COneCamera::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CStatic::OnMouseMove(nFlags, point);
}

BOOL COneCamera::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void COneCamera::ExchangeAVIPlayer( CMyAVPlayer *otherPlayer )
{
	this->m_AVIPlayer = otherPlayer;
	m_AVIPlayer->SetPlayWnd(this->GetSafeHwnd());
}

bool COneCamera::IsPlaying()
{
	return this->m_AVIPlayer->IsPlaying();
}

void COneCamera::StopRTPlay(bool reUse)
{
	if (m_MediaSocket != NULL)
	{
		m_MediaSocket->CloseConnect();
	}
	if (m_AVIPlayer->IsPlaying())
	{
		m_AVIPlayer->CloseStream();
	}
	 if (!reUse)
 	 {
 		 m_cameraID = 0;
 	 }
	 m_pOwner->InvalidateAll();

	 //m_pOwner->GetParent()->PostMessage(WM_RTVIDEOSTOP);
}

int COneCamera::StartPlay()
{
	int videoID = m_cameraID >> 8;
	int channelNo = m_cameraID & 0xFF;
	int ret = theApp.g_cmdSocket->SendRealTimeDataMsg(videoID,channelNo,0,0);
	if (ret != KE_SUCCESS)
	{
		m_cameraID = 0;
	}
	return ret;
}

bool COneCamera::CheckMediaExist( int videoID,int channelNo )
{
	if (m_cameraID == ((videoID<<8)+channelNo))
	{
		return true;
	}
	return false;
}


void COneCamera::SetPlayIndex( int videoID,int channelNo )
{
	this->m_cameraID = (videoID<<8)+channelNo;
}

void COneCamera::DrawCameraBack( CDC *pDC, CRect destFrame, COLORREF rectColor, COLORREF FrameColor )
{
	ASSERT(!destFrame.IsRectNull() || !destFrame.IsRectEmpty());

	//CRect dest(destFrame.left+1, destFrame.top+1, destFrame.right, destFrame.bottom);

	CMemDC* pMemDC = NULL;
 	pMemDC = new CMemDC(pDC, &destFrame);
// 
 	CBrush	brush;
 	brush.CreateSolidBrush(FrameColor);
 	CBrush*	pOldBrush = (CBrush*)pMemDC->SelectObject(&brush);
 	if(pMemDC->GetSafeHdc() != NULL)
 	{
// 		pMemDC->FillRect(clBlack, &brush);
 		//pMemDC->FrameRect(destFrame, &brush);
 		pMemDC->SelectObject(pOldBrush);
 	}
// 
// 	//font.DeleteObject();
// 	DeleteDC(pMemDC->m_hDC);
// 	DeleteObject(pMemDC->m_hDC);
 	ReleaseDC(pMemDC);//test remove 2005.05.12
 	_SAFE_DELETE(pMemDC);
}



//std::map<int,COneCamera*> COneCamera::g_CameraMap;



void COneCamera::OnMenuFullscreen()
{

	m_pOwner->ShowFullScreen();
}

void COneCamera::SwapVideo( COneCamera * camera )
{
		int tmpID = camera->m_cameraID;
		camera->m_cameraID = this->m_cameraID;
		this->m_cameraID = tmpID;

		CMyAVPlayer * pOtherPlayer =  camera->m_AVIPlayer;
		camera->ExchangeAVIPlayer(m_AVIPlayer);
		this->ExchangeAVIPlayer(pOtherPlayer);

		CMediaSocket * tmpMedia = camera->m_MediaSocket;
		camera->m_MediaSocket = this->m_MediaSocket;
		this->m_MediaSocket = tmpMedia;
}

void COneCamera::OnMenuAllfullscreen()
{
	m_pOwner->ShowFullScreen();
}

BOOL COneCamera::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
// 	if (pMsg->message == WM_LBUTTONUP)
// 	{
// 		
// 	}
	
	return CStatic::PreTranslateMessage(pMsg);
}
