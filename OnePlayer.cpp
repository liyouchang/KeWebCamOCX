// OnePlayer.cpp : 实现文件
//

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "OnePlayer.h"


// COnePlayer 对话框

IMPLEMENT_DYNAMIC(COnePlayer, CDialog)

COnePlayer::COnePlayer(CWnd* pParent /*=NULL*/)
	: CDialog(COnePlayer::IDD, pParent)
{
	m_pOwner	 = NULL;
	m_nCamNo = 0;
	m_cameraID = 0;
	m_bFull			= FALSE;
	m_bDrawable = FALSE;
	m_AVIPlayer = new CMyAVPlayer;
	m_bDrag = FALSE;
	ctrlDlg = NULL;
	showCtrlDlg = false;
	menu.LoadMenu(IDR_MENU_CAMSELECT);

	m_brBkgnd.CreateSolidBrush(clBlack);
}

COnePlayer::~COnePlayer()
{

	if (m_cameraID != 0)
	{
		CMediaSocket::DelVideoSvrMedia(m_cameraID>>8);
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

void COnePlayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMERA, m_camera);
}


BEGIN_MESSAGE_MAP(COnePlayer, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_MENU_AllFullScreen, &COnePlayer::OnMenuAllfullscreen)
	ON_COMMAND(ID_MENU_RESET, &COnePlayer::OnMenuReset)
	ON_WM_CTLCOLOR()
	//ON_STN_CLICKED(IDC_TEXT_RIGHT, &COnePlayer::OnStnClickedTextRight)
END_MESSAGE_MAP()

bool COnePlayer::IsPlaying()
{
	return this->m_AVIPlayer->IsPlaying();
}


// COnePlayer 消息处理程序

void COnePlayer::OnLButtonDown(UINT nFlags, CPoint point)
{
	//  在此添加消息处理程序代码和/或调用默认值
	if( m_pOwner->m_nActiveCamera != m_nCamNo )
	{
	 		m_pOwner->SetActiveCamera(m_nCamNo);
	}
	CDialog::OnLButtonDown(nFlags, point);
	m_bDrag = FALSE;
	Sleep(200);
	if (m_pOwner->PeekLButtonUpMsg())
	{
		return;
	}
	m_bDrag = TRUE;
	::SetCursor(::LoadCursor(NULL,IDC_SIZEALL));
}

void COnePlayer::SetOwner( CWebCamPannel* pOwner )
{
	this->m_pOwner = pOwner;
	this->m_camera.SetOwner(this);
	this->m_AVIPlayer->SetPlayWnd(this->m_camera.GetSafeHwnd());
}

void COnePlayer::OnPaint()
{
	CPaintDC dc(this); // device context for painting


	CRect rect;
	GetClientRect(rect);

	if (m_pOwner->m_FnDivision == DIV_PLAYER)
	{
		if (ctrlDlg == NULL)
		{
			ctrlDlg = new CPlayerCtrlDlg;
			ctrlDlg->Create(IDD_DIALOG_Player,this);
		}
		if (!showCtrlDlg)
		{
			this->m_camera.MoveWindow(rect);
		}
		else
		{
			CRect ctrlRect;
			ctrlDlg->GetClientRect(ctrlRect);
			this->m_camera.MoveWindow(rect.top,rect.left,rect.Width(),rect.Height()-ctrlRect.Height());
		}
	}
	else
	{
		this->m_camera.MoveWindow(rect);
	}
	
	

}

void COnePlayer::ExchangeAVIPlayer( CMyAVPlayer *otherPlayer )
{
	this->m_AVIPlayer = otherPlayer;
	m_AVIPlayer->SetPlayWnd(this->GetSafeHwnd());
}

void COnePlayer::SwapVideo( COnePlayer * otherPlayer )
{
	int tmpID = otherPlayer->m_cameraID;
	otherPlayer->m_cameraID = this->m_cameraID;
	this->m_cameraID = tmpID;
	//交换AVPlayer *
	CMyAVPlayer * pOtherAVI =  otherPlayer->m_AVIPlayer;
	otherPlayer->ExchangeAVIPlayer(m_AVIPlayer);
	this->ExchangeAVIPlayer(pOtherAVI);
}

void COnePlayer::StopRTPlay( bool reUse /*= false*/ )
{
	//停止发送数据
	CMediaSocket * pMedia = CMediaSocket::GetVideoSvrMedia(m_cameraID>>8);
	pMedia->ReqestMediaData(m_cameraID,CMediaSocket::Media_None);
	pMedia->CloseConnect();
	//停止播放
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

void COnePlayer::SetActive()
{
	this->m_pOwner->SetActiveCamera(this->m_nCamNo);
}

int COnePlayer::StartPlay()
{
	int videoID = m_cameraID >> 8;
	int channelNo = m_cameraID & 0xFF;
	int ret = theApp.g_cmd->StartView(m_cameraID);
	if (ret != KE_SUCCESS)
	{
		this->StopRTPlay();
	}
	return ret;
}

void COnePlayer::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//  鼠标抬起，交换画面
 	if( m_pOwner->m_nActiveCamera != m_nCamNo )
 	{
  		COnePlayer * tmpCam = &m_pOwner->m_camarray[m_pOwner->m_nActiveCamera];
 		if (tmpCam->m_bDrag)
 		{
 			SwapVideo(tmpCam);
 			m_pOwner->SetActiveCamera(m_nCamNo);
 			tmpCam->Invalidate();
 			this->Invalidate();
 		}
 	}
	::SetCursor(::LoadCursor(NULL,IDC_ARROW));
	CDialog::OnLButtonUp(nFlags, point);
}

void COnePlayer::OnRButtonDown(UINT nFlags, CPoint point)
{
	// 在此添加消息处理程序代码和/或调用默认值
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
	CDialog::OnRButtonDown(nFlags, point);
}

void COnePlayer::OnMouseMove(UINT nFlags, CPoint point)
{
	// 在此添加消息处理程序代码和/或调用默认值
	if (m_pOwner->m_FnDivision == DIV_PLAYER && ctrlDlg)
	{
		CRect rect;
		GetClientRect(rect);
		CRect ctrlRect;
		ctrlDlg->GetClientRect(ctrlRect);
		int startY = rect.bottom-ctrlRect.Height() ;
		if (point.y > startY)
		{
			ctrlDlg->ShowWindow(SW_SHOW);
			ctrlDlg->MoveWindow(rect.top,startY,rect.Width(),ctrlRect.Height());
			showCtrlDlg = true;
		}
		else
		{
			ctrlDlg->ShowWindow(SW_HIDE);
			showCtrlDlg = false;
		}
	}
	else
	{
		showCtrlDlg = false;
	}
	CDialog::OnMouseMove(nFlags, point);
}

void COnePlayer::OnMenuAllfullscreen()
{
	m_pOwner->ShowFullScreen();
}

void COnePlayer::OnCancel()
{
	GetParent()->SendMessage(WM_CLOSE);
	
	CDialog::OnCancel();
}

void COnePlayer::OnMenuReset()
{
	theApp.g_pMainWnd->SendMessage(WM_RTVIDEOSTOP,m_cameraID);
	//	m_pOwner->m_camarray[m_nCamNo].Invalidate();
	this->Invalidate();
}

HBRUSH COnePlayer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	hbr = (HBRUSH)m_brBkgnd.GetSafeHandle();
	return hbr;
}

bool COnePlayer::IsActive()
{
	return (m_pOwner->m_nActiveCamera == this->m_nCamNo);
}
