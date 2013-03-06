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
	m_nChannelLive	= 0;
	m_nAviIndex		= 0;

	menu.LoadMenu(IDR_MENU_CAMSELECT);

	m_CamRef.Brightness	= 0;
	m_CamRef.Hue		= 0;
	m_CamRef.Contrast	= 128;
	m_CamRef.Saturation	= 128;
	
	m_event.bAlarm		= FALSE;
	m_event.bLoss		= FALSE;
	m_event.bMotion		= FALSE;

	InitCarmera();

}

COneCamera::~COneCamera()
{
	TRACE("~COneCamera[%d]\n", m_nCamNo);
}


BEGIN_MESSAGE_MAP(COneCamera, CStatic)
	//{{AFX_MSG_MAP(COneCamera)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENU_SAVEAS, OnMenuSaveas)
	ON_COMMAND(ID_MENU_SAVEALL, OnMenuSaveall)
	ON_COMMAND(ID_MENU_RESET, OnMenuResetChannel)
	ON_COMMAND(ID_MENU_RESET_ALL, OnMenuResetAll)
	ON_COMMAND(ID_MENU_PRINT_IMAGE, OnMenuPrintImage)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MENU_SHOW_WATERMARK, OnMenuShowWatermark)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COneCamera message handlers

void COneCamera::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);

	if(m_pOwner != NULL)
	{
		COLORREF clFrame;

		if(!m_bFull)
		{
			clFrame = (m_pOwner->m_nActiveCamera==m_nChannel) ? clBlue:FLOAT_CAM_FRAME_COLOR;
			if(m_nChannel < theApp.m_nMAXCHANNEL)
				m_strCaption.Format(_T("CamNo %02d "), m_nChannel+1);
			else
				m_strCaption.Empty();
		}
		else
		{
			clFrame = (m_pOwner->m_nActiveCamera==m_nCamNo) ? clBlue:FLOAT_CAM_FRAME_COLOR;
			if(m_nCamNo < theApp.m_nMAXCHANNEL)
				m_strCaption.Format(_T("CamNo %02d "), m_nCamNo+1);
			else
				m_strCaption.Empty();

		}

		DrawCameraImage(&dc, rect, m_strCaption + m_strDateTime, m_pOwner->GetCamFontColor(), clFrame);
	}

	// Do not call CStatic::OnPaint() for painting messages
}

void COneCamera::LButtonDown(UINT nFlags, CPoint point) 
{
	m_DownPos = point;

	if( m_pOwner->m_nActiveCamera != m_nChannel )
	{
		m_pOwner->m_nActiveCamera = m_nChannel;
		m_pOwner->InvalidateAll();
	}
}

void COneCamera::LButtonDblClk(UINT nFlags, CPoint point)
{
	CRect	rect = m_pOwner->GetFullPaintRect();
	int		dx	 = rect.Width()  / 4;
	int		dy	 = rect.Height() / 4;

	if(m_pOwner->m_nActiveCamera == m_nChannel)
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
				case DIV_CH10:		nRotation = m_pOwner->m_nRotation_10;		break;
				default :			nRotation = -1;
			}
			m_pOwner->SetDivision(m_pOwner->m_nToggledDivision, nRotation, FALSE);
			TRACE("m_nActiveCamera %d\n", m_pOwner->m_nActiveCamera);
			TRACE("DIV_TOGGLED= %d\n", m_pOwner->m_nToggledDivision);
		}
		else
		{
			// 1Div 일 때 DClick 금지
			if(m_pOwner->FnDivision == DIV_CH1)
				return;

			int	mapped_camno = m_nChannel;//m_nCamNo;


			//m_pOwner->DrawAllCameraImages(DIV_CH1);

			// daeny2@ Channel ID Mapping
			if(theApp.m_nMAXCHANNEL == DIV_DEFAULT_CH9)
			{
				mapped_camno = new_MappiingID[m_nChannel];

				// 정상 채널이 아닌 경우
				if(m_nChannel >= DIV_DEFAULT_CH9)
					return;
			}
			else if(theApp.m_nMAXCHANNEL == DIV_DEFAULT_CH8)
			{
				mapped_camno = new_MappiingID[m_nChannel];

				// 정상 채널이 아닌 경우
				if(m_nChannel >= DIV_DEFAULT_CH8)
					return;
			}

			// CIF Mode 일때 channel mapping
			if(m_nRec_Type == 3)
			{
				if((mapped_camno % 2) != 0)
					mapped_camno--;
			}

			theApp.m_nCurrentMappingCamera = mapped_camno;

			TRACE("MAP CH-%d\n", mapped_camno);
			// daeny2@ 2005/08/18	Full screen 처리
			long	value = MAKELONG(full_screen_on, mapped_camno);
			if(theApp.m_bOneChTransfer)
				m_pOwner->SendMsg(dvr_cmd_msg, dvrsub_cmd_msg, full_screen_request, value);

			CRect r;
			r = m_pOwner->GetFullPaintRect();
			m_pOwner->m_nActiveCamera = m_nChannel;
			m_pOwner->m_camarray[m_nChannel].MoveWindows(r, TRUE);

			m_pOwner->m_nToggledDivision = m_pOwner->FnDivision;
			m_pOwner->SetDivision(DIV_TOGGLED, -1, FALSE);

			TRACE("LButtonDblClk m_nCamNo = %d\n", m_nCamNo);
			TRACE("m_nActiveCamera %d\n", m_pOwner->m_nActiveCamera);
			TRACE("DIV_TOGGLED else = %d\n", m_pOwner->m_nToggledDivision);
		}
	}
}

void COneCamera::OnLButtonDown(UINT nFlags, CPoint point) 
{
	LButtonDown(nFlags, point);
	CStatic::OnLButtonDown(nFlags, point);
}


void COneCamera::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	LButtonDblClk(nFlags, point);
	CStatic::OnLButtonDblClk(nFlags, point);
}

void COneCamera::InitCarmera()
{
	m_strCaption	= "";
	m_bEvent		= 0;
	m_bSaveToAVI	= FALSE;
	m_bFull			= FALSE;
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

	if(caption)
	{
		VERIFY(font.CreateFont(
		   12,                        // nHeight
		   0,                         // nWidth
		   0,                         // nEscapement
		   0,                         // nOrientation
		   FW_NORMAL,                 // nWeight
		   FALSE,                     // bItalic
		   FALSE,                     // bUnderline
		   0,                         // cStrikeOut
		   ANSI_CHARSET,              // nCharSet
		   OUT_DEFAULT_PRECIS,        // nOutPrecision
		   CLIP_DEFAULT_PRECIS,       // nClipPrecision
		   DEFAULT_QUALITY,           // nQuality
		   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		   _T("Arial")));                 // lpszFacename

		CFont* pOldFont	= pDC->SelectObject(&font);
		int textheight	= pDC->GetTextExtent(_T("thisisTESTjg")).cy + 2;
		dest = CRect(destFrame.left+1, destFrame.top+textheight, destFrame.right, destFrame.bottom);
		CRect stringArea	= destFrame;
		stringArea.bottom	= stringArea.top + textheight;
		pDC->FillRect(stringArea, &brush);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(FontColor);

		pDC->TextOut(destFrame.left + 2, destFrame.top, caption);
		pDC->SelectObject(pOldFont);
	}

	if(m_pOwner->m_Imagearray[m_nCamDisp].IsValid())
	{
		m_pOwner->m_Imagearray[m_nCamDisp].Stretch(pDC->GetSafeHdc(), dest);
	}

	if(pDC->GetSafeHdc() != NULL)
	{
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
	TCHAR			menu_string1[32];
	TCHAR			menu_string2[32];
	TCHAR			menu_string3[32];
	TCHAR			menu_string4[32];
	CRect			rect;

	GetWindowRect(rect);

	int x = rect.TopLeft().x + point.x;
	int y = rect.TopLeft().y + point.y;

	TRACE(TEXT("menu x= %d, y=%d\n"), x, y);

	memset(&menu_string1, 0x00, sizeof(menu_string1));
	memset(&menu_string2, 0x00, sizeof(menu_string2));
	memset(&menu_string3, 0x00, sizeof(menu_string3));
	memset(&menu_string3, 0x00, sizeof(menu_string4));

//	sprintf(menu_string, "Save \"CamNo %02d\" Image...", m_pOwner->m_camarray[m_nCamNo].m_nCamDisp+1);
	_stprintf(menu_string1, _T("Save \"CamNo %02d\" Image..."), m_nChannel+1);
	_stprintf(menu_string2, TEXT("Print \"CamNo %02d\" Image..."), m_nChannel+1);
	_stprintf(menu_string3, TEXT("Reset \"CamNo %02d\" Channel"), m_nChannel+1);

	if(m_pOwner->m_nWatermark)
		_stprintf(menu_string4, TEXT("Hide Watermark"));
	else
		_stprintf(menu_string4, TEXT("Show Watermark"));
	
	UINT new_id1 = menu.GetSubMenu(0)->GetMenuItemID(0);
	menu.GetSubMenu(0)->ModifyMenu(ID_MENU_SAVEAS, MF_BYCOMMAND|MF_STRING, new_id1, (LPCTSTR)(LPSTR)menu_string1);

	UINT new_id2 = menu.GetSubMenu(0)->GetMenuItemID(3);
	menu.GetSubMenu(0)->ModifyMenu(ID_MENU_PRINT_IMAGE, MF_BYCOMMAND|MF_STRING, new_id2, (LPCTSTR)(LPSTR)menu_string2);

	UINT new_id3 = menu.GetSubMenu(0)->GetMenuItemID(5);
	menu.GetSubMenu(0)->ModifyMenu(ID_MENU_RESET, MF_BYCOMMAND|MF_STRING, new_id3, (LPCTSTR)(LPSTR)menu_string3);

	UINT new_id4 = menu.GetSubMenu(0)->GetMenuItemID(8);
	menu.GetSubMenu(0)->ModifyMenu(ID_MENU_SHOW_WATERMARK, MF_BYCOMMAND|MF_STRING, new_id4, (LPCTSTR)(LPSTR)menu_string4);

	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, x, y, this);

	CStatic::OnRButtonDown(nFlags, point);
}


void COneCamera::OnMenuResetChannel()
{
	m_pOwner->m_Imagearray[m_nCamNo].Destroy();
	m_pOwner->m_Imagearray[m_nCamNo].Copy(*m_pOwner->temp_image);
	m_pOwner->m_camarray[m_nCamNo].m_bLoss = TRUE;
	m_pOwner->m_camarray[m_nCamNo].m_strDateTime.Empty();
	m_pOwner->m_camarray[m_nCamNo].Invalidate();
}

void COneCamera::OnMenuPrintImage() 
{
//	m_pOwner->m_nSelectCamera = m_nChannel;
	m_pOwner->m_nSelectCamera = m_nCamNo;

	//CDlgPrint	dlgPrint;
	
//	dlgPrint.DoModal();		
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
		m_pOwner->m_Imagearray[i].Destroy();
		m_pOwner->m_Imagearray[i].Copy(*m_pOwner->temp_image);
		m_pOwner->m_camarray[i].m_bLoss = TRUE;
		m_pOwner->m_camarray[i].m_strDateTime.Empty();
		m_pOwner->m_camarray[i].Invalidate();
	}
}

void COneCamera::OnMenuSaveas() 
{
	
	WCHAR		file_name[16];
	static WCHAR BASED_CODE OpenFilter[] = _T("JPG Files (*.jpg)|*.jpg; *.jpg||");

	swprintf(file_name, _T("image_%02d.jpg"), m_pOwner->m_camarray[m_nCamNo].m_nCamDisp+1);

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

		if(m_pOwner->m_Imagearray[m_nCamNo].GetWidth() > 600)
		{
			nWidth	= 720;
			//nHeight = theApp.m_nVideoFormat == NTSC ? 480 : 576;
		}
		else
		{
			nWidth	= 320;
		//	nHeight = theApp.m_nVideoFormat == NTSC ? 240 : 288;
		}

		m_pOwner->m_Imagearray[m_nCamNo].Resample(nWidth, nHeight, 1, iDst);
		iDst->Save(strFilePath, CXIMAGE_FORMAT_JPG);
		_SAFE_DELETE(iDst);
#else
		m_pOwner->m_Imagearray[m_nCamNo].Save(strFilePath, CXIMAGE_FORMAT_JPG);
#endif
	}
}

void COneCamera::OnMenuSaveall() 
{
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
}

void COneCamera::ResetAVI()
{
	CString strFileName;

	m_bSaveToAVI = FALSE;
//	strFileName	 = m_AVI->GetFileName();

	//if(m_AVI)
//	{
//		m_AVI->CloseAviFile();

//		_SAFE_DELETE(m_AVI);

//		m_nAviIndex++;
		
	//	CString strExt;
//		strExt.Format("%04d.avi", m_nAviIndex);

	//	m_AVI = new TAVI(strFileName + strExt, strFileName);
		//m_AVI->SetFccHandler(m_dwFcc);
		//m_AVI->SetFrameRate(m_dwRate);
//	}

	//m_bSaveToAVI = TRUE;
}
