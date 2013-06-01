// OneCamera.cpp : implementation file
//

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "OneCamera.h"
#include "WebCamPannel.h"
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
	m_pOwner	 = NULL;
	m_nCamNo = 0;
	m_cameraID = 0;
	menu.LoadMenu(IDR_MENU_CAMSELECT);
	m_MediaSocket = NULL;
	m_bFull			= FALSE;
	m_bDrawable = FALSE;
	m_AVIPlayer = new CMyAVPlayer;
	m_brBkgnd.CreateSolidBrush(clBlack);
	ctrlDlg = NULL;
}

COneCamera::~COneCamera()
{
	//LOG_DEBUG("COneCamera ["<< m_nCamNo<<"] distructor!");
	if (m_MediaSocket != NULL)
	{
		m_MediaSocket->CloseConnect();
		delete m_MediaSocket;
	}

	if (m_AVIPlayer != NULL)
	{
		if (IsPlaying())
		{
			m_AVIPlayer->CloseStream();
		}
		
		delete m_AVIPlayer;
	}

}


BEGIN_MESSAGE_MAP(COneCamera, CStatic)
	//{{AFX_MSG_MAP(COneCamera)
//	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENU_RESET, OnMenuResetChannel)
	ON_COMMAND(ID_MENU_RESET_ALL, OnMenuResetAll)
//	ON_WM_ERASEBKGND()
	
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_MENU_PLAY, &COneCamera::OnMenuPlay)
	ON_COMMAND(ID_MENU_SLOW, &COneCamera::OnMenuSlow)
	ON_COMMAND(ID_MENU_FAST, &COneCamera::OnMenuFast)

	ON_WM_CREATE()
	//ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_MENU_FULLSCREEN, &COneCamera::OnMenuFullscreen)
	ON_COMMAND(ID_MENU_AllFullScreen, &COneCamera::OnMenuAllfullscreen)
//	ON_WM_CTLCOLOR()
//	ON_WM_CTLCOLOR_REFLECT()
//	ON_WM_ENABLE()
	ON_WM_LBUTTONUP()
//	ON_WM_ERASEBKGND()
ON_WM_PAINT()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COneCamera message handlers

void COneCamera::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( m_pOwner->m_nActiveCamera != m_nCamNo )
	{
		m_pOwner->SetActiveCamera(m_nCamNo);
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
	if(m_pOwner->m_nActiveCamera == m_nCamNo)
	{
		if( m_pOwner->m_FnDivision == DIV_TOGGLED)
		{
			m_pOwner->SetPlayDivision(m_pOwner->m_nToggledDivision);
		}
		else
		{
			if(m_pOwner->m_FnDivision == DIV_CH1 || m_pOwner->m_FnDivision==DIV_PLAYER)
				return;
			m_pOwner->m_nToggledDivision = m_pOwner->m_FnDivision;
			m_pOwner->SetPlayDivision(DIV_TOGGLED);
		}
	}
	CStatic::OnLButtonDblClk(nFlags, point);
}

void COneCamera::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if( m_pOwner->m_nActiveCamera != m_nCamNo )
	{
		m_pOwner->SetActiveCamera(m_nCamNo);
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
		TRACE("Right click is not playing, so gray!\n");
	}
	else
	{
		menu.GetSubMenu(0)->EnableMenuItem(ID_MENU_RESET,MF_BYCOMMAND|MF_ENABLED  );  
		TRACE("Right click is playing\n");
	}
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, x, y, this);
	CStatic::OnRButtonDown(nFlags, point);
}


void COneCamera::OnMenuResetChannel()
{
	theApp.g_pMainWnd->SendMessage(WM_RTVIDEOSTOP,m_cameraID);
	m_pOwner->m_camarray[m_nCamNo].Invalidate();
}

void COneCamera::OnMenuResetAll()
{
	for(int i = 0; i < CAM_MAX; i++)
	{
		m_pOwner->m_camarray[i].Invalidate();
	}
}


void COneCamera::OnMenuPlay()
{
	m_AVIPlayer->PlayFile(this->GetSafeHwnd());
}

void COneCamera::OnMenuSlow()
{
	// TODO: 慢速播放
}

void COneCamera::OnMenuFast()
{
	// TODO: 快速播放
}

void COneCamera::SetOwner( CWebCamPannel* pOwner )
{
	m_pOwner = pOwner;
	this->m_AVIPlayer->SetPlayWnd(this->GetSafeHwnd());
	
}


BOOL COneCamera::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//  直接返回以保持设置的指针
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

	 this->Invalidate();
}

int COneCamera::StartPlay()
{
	int videoID = m_cameraID >> 8;
	int channelNo = m_cameraID & 0xFF;
	//int ret = theApp.g_cmdSocket->SendRealTimeDataMsg(videoID,channelNo,0,0);
	int ret = theApp.g_cmd->StartView(m_cameraID);
	if (ret != KE_SUCCESS)
	{
		this->StopRTPlay();
		//m_cameraID = 0;
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

void COneCamera::OnMenuFullscreen()
{

	m_pOwner->ShowFullScreen();
}

void COneCamera::SwapVideo( COneCamera * camera )
{
		int tmpID = camera->m_cameraID;
		camera->m_cameraID = this->m_cameraID;
		this->m_cameraID = tmpID;
		//交换AVPlayer *
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

void COneCamera::OnLButtonUp(UINT nFlags, CPoint point)
{
	//  鼠标抬起，交换画面
	 	if( m_pOwner->m_nActiveCamera != m_nCamNo )
	 	{
	  		COneCamera * tmpCam = &m_pOwner->m_camarray[m_pOwner->m_nActiveCamera];
	 		if (tmpCam->m_bDrag)
	 		{
	 			SwapVideo(tmpCam);
	 			m_pOwner->SetActiveCamera(m_nCamNo);
	 			tmpCam->Invalidate();
	 			this->Invalidate();
	 		}
	 	}
	 	::SetCursor(::LoadCursor(NULL,IDC_ARROW));
	CStatic::OnLButtonUp(nFlags, point);
}


void COneCamera::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//  画背景色

	
 	if (m_AVIPlayer->IsPlaying() )//如果正在播放视频,则不刷新
	{
		return;
	}
	if(m_pOwner != NULL)
	{
		 	CRect rect;
		 	GetClientRect(rect);
	 		dc.FillRect(rect, &m_brBkgnd);		
	}
	
}

void COneCamera::OnMouseMove(UINT nFlags, CPoint point)
{
	// 在此添加消息处理程序代码和/或调用默认值
	if (m_pOwner->m_FnDivision == DIV_PLAYER)
	{
		CRect rect;
		GetClientRect(rect);
		if (ctrlDlg == NULL)
		{
			ctrlDlg = new CPlayerCtrlDlg;
			ctrlDlg->Create(IDD_DIALOG_Player,this);
		}
		CRect ctrlRect;
		ctrlDlg->GetClientRect(ctrlRect);
		int startY = rect.bottom-ctrlRect.Height() ;
		if (point.y > startY)
		{
			ctrlDlg->ShowWindow(SW_SHOW);
			ctrlDlg->MoveWindow(rect.top,startY,rect.Width(),ctrlRect.Height());
		}
		else
		{
			ctrlDlg->ShowWindow(SW_HIDE);
		}
	}
	
	CStatic::OnMouseMove(nFlags, point);
}
