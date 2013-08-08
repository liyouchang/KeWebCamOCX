// PlayerCtrlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KeWebCamOCX.h"
#include "PlayerCtrlDlg.h"
#include "Common.h"

// CPlayerCtrlDlg �Ի���

IMPLEMENT_DYNAMIC(CPlayerCtrlDlg, CDialog)

CPlayerCtrlDlg::CPlayerCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerCtrlDlg::IDD, pParent)
{

}

CPlayerCtrlDlg::~CPlayerCtrlDlg()
{
}

void CPlayerCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_btnPlay);
	DDX_Control(pDX, IDC_BUTTON_FAST, m_btnFast);
	DDX_Control(pDX, IDC_BUTTON_SLOW, m_btnSlow);
	DDX_Control(pDX, IDC_BUTTON_PAUSE, m_btnPause);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);
	DDX_Control(pDX, IDC_BUTTON_OpenFile, m_btnOpen);
	DDX_Control(pDX, IDC_SLIDER_MP, m_playpos);
}


BEGIN_MESSAGE_MAP(CPlayerCtrlDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CPlayerCtrlDlg::OnBnClickedButtonPlay)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_MP, &CPlayerCtrlDlg::OnNMReleasedcaptureSliderPos)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_FAST, &CPlayerCtrlDlg::OnBnClickedButtonFast)
	ON_BN_CLICKED(IDC_BUTTON_SLOW, &CPlayerCtrlDlg::OnBnClickedButtonSlow)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CPlayerCtrlDlg::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CPlayerCtrlDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_OpenFile, &CPlayerCtrlDlg::OnBnClickedButtonOpenfile)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MP, &CPlayerCtrlDlg::OnNMCustomdrawSliderPos)
	ON_MESSAGE(WM_PLAYOVER,FilePlayOver)
//	ON_STN_CLICKED(IDC_SlowPlay, &CPlayerCtrlDlg::OnStnClickedSlowplay)
END_MESSAGE_MAP()


// CPlayerCtrlDlg ��Ϣ�������

void CPlayerCtrlDlg::OnBnClickedButtonPlay()
{
	
	COnePlayer * pCamera = (COnePlayer *)this->GetParent();
	pCamera->m_AVIPlayer->NormalPlay();
	PlayStart();
}

void CPlayerCtrlDlg::OnCancel()
{
	
	//ȫ��ʱesc�˳�
	theApp.g_PlayWnd->GetParent()->SendMessage(WM_CLOSE);
//	CDialog::OnCancel();
}

BOOL CPlayerCtrlDlg::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialog::OnChildNotify(message, wParam, lParam, pLResult);
}

void CPlayerCtrlDlg::OnNMReleasedcaptureSliderPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	COnePlayer * pCamera = (COnePlayer *)this->GetParent();
	int pos = m_playpos.GetPos();
	pCamera->m_AVIPlayer->SetPlayPos(pos);
	*pResult = 0;
}

BOOL CPlayerCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

//  	m_playpos.SetSkin(IDB_SLIDE, IDB_BLUE_NORM, IDB_BLUE_NORMSEL, BG_STARTATICK | BG_CENTER_ONTICK | BG_STRETCH_HOR);
  	

	COnePlayer * pCamera  = (COnePlayer *)this->GetParent();
	CMyAVPlayer * pPlayer = pCamera->m_AVIPlayer;
	pPlayer->SetCtrlDlg(this);

	m_playpos.SetPageSize(5);
	m_playpos.SetRange(0,100);
// 	CRect rect;
// 	GetClientRect(rect);

	m_btnOpen.SetBitmaps(IDB_OPEN1,clBlack);
	m_btnOpen.SetTooltipText(IDS_STRING_BtnOpenFile);
	m_btnPlay.SetBitmaps(IDB_PLAY1,clBlack);
	m_btnPlay.SetTooltipText(IDS_STRING_BtnPlay);
	m_btnFast.SetBitmaps(IDB_FastPlay1,clBlack);
	m_btnPause.SetBitmaps(IDB_PAUSE1,clBlack);
	m_btnStop.SetBitmaps(IDB_Stop1,clBlack);
	m_btnSlow.SetBitmaps(IDB_SlowPlay1,clBlack);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPlayerCtrlDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetClientRect(rect);
	CRect infoRect;
	GetDlgItem(IDC_STATIC_SPEED)->GetWindowRect(infoRect);
	this->ScreenToClient(infoRect);
	infoRect.MoveToX(rect.Width()-infoRect.Width());
	GetDlgItem(IDC_STATIC_SPEED)->MoveWindow(infoRect);


	CRect slideRect;
	m_playpos.GetClientRect(slideRect);
	int width = rect.Width();
	m_playpos.MoveWindow(rect.top,rect.left,rect.Width(),slideRect.Height());
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

}

void CPlayerCtrlDlg::OnBnClickedButtonFast()
{
 	COnePlayer * pCamera = (COnePlayer *)this->GetParent();
	int speed = pCamera->m_AVIPlayer->GetSpeed();
	if (speed < 4)
	{
		speed++;
	}
	pCamera->m_AVIPlayer->SetSpeed(speed);
 	SetPlayState(speed);
}

void CPlayerCtrlDlg::OnBnClickedButtonSlow()
{
	COnePlayer * pCamera = (COnePlayer *)this->GetParent();
	int speed = pCamera->m_AVIPlayer->GetSpeed();
	if (speed > -4)
	{
		speed--;
	}
	pCamera->m_AVIPlayer->SetSpeed(speed);
	SetPlayState(speed);
}

void CPlayerCtrlDlg::OnBnClickedButtonPause()
{
	COnePlayer * pCamera = (COnePlayer *)this->GetParent();
	pCamera->m_AVIPlayer->PausePlay();
	SetPlayState(-5);
	//SetWindowEnable(1);
}

void CPlayerCtrlDlg::OnBnClickedButtonStop()
{
	COnePlayer * pCamera = (COnePlayer *)this->GetParent();
	pCamera->m_AVIPlayer->CloseFile();
	pCamera->Invalidate();
	if (pCamera->m_cameraID != 0)
	{
		CMediaSocket * media = theApp.g_cmd->GetMediaSocket(pCamera->m_cameraID);
		media->CloseConnect();
	}
	
	PlayStop();

}

void CPlayerCtrlDlg::SetPlayState( int id )
{
	CString strState = _T("�����ٶ�");
	switch(id)
	{
	case 0:strState = _T("�����ٶ�");break;
	case 1:strState = _T("2����");break;
	case 2:strState = _T("4����");break;
	case 3:strState = _T("8����");break;
	case 4:strState = _T("16����");break;
	case -1:strState = _T("1/2����");break;
	case -2:strState = _T("1/4����");break;
	case -3:strState = _T("1/8����");break;
	case -4:strState = _T("1/16����");break;
	case -5:strState = _T("��ͣ");break;
	}
	GetDlgItem(IDC_STATIC_SPEED)->SetWindowText(strState);
}

void CPlayerCtrlDlg::GetPlayPos()
{
	COnePlayer * pCamera = (COnePlayer *)this->GetParent();
	int pos;
	int ret = pCamera->m_AVIPlayer->GetPlayPos(pos);
	if (ret ==0)
	{
		m_playpos.SetPos(pos);
		//TRACE1("Play pos %d\r\n",pos);
	}
}

void CPlayerCtrlDlg::OnBnClickedButtonOpenfile()
{
	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.h264)|*.h264|All Files (*.*)|*.*||"), NULL);
	CString strFile;
	COnePlayer * pCamera = (COnePlayer *)this->GetParent();
	if (dlgFile.DoModal() == IDOK)
	{
		strFile = dlgFile.GetPathName();
		//MessageBox(strFile);
		std::string strFileName = tstd::tstr_to_str(strFile.GetString());
		pCamera->m_AVIPlayer->PlayFile(strFileName.c_str(),0,this->GetSafeHwnd());
		//OnBnClickedButtonPlay();
		//SetWindowEnable(0);
	}
}

void CPlayerCtrlDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_iTimeHandle)
	{
		//GetDownloadRate();
		GetPlayPos();

		COnePlayer * pCamera = (COnePlayer *)this->GetParent();
		if (pCamera->m_cameraID != 0)
		{
			CMediaSocket * media = theApp.g_cmd->GetMediaSocket(pCamera->m_cameraID);
			CString tmpStr;
			int playNo = media->playedFile+1;
			int downloadNo = media->downloaded;
			int totalNo = media->totalFile;
			int rate =media->m_Recorder->GetDownloadPercent();
			tmpStr.Format(_T("�ļ�����:%d,���ڲ���:%d,�������:%d,���ؽ���%d%%"),totalNo,playNo,downloadNo,rate);
			SetDlgItemText(IDC_STATIC_Status,tmpStr);

		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CPlayerCtrlDlg::OnNMCustomdrawSliderPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CPlayerCtrlDlg::SetWindowEnable( int id )
{
	switch (id)	
	{
	case 0:
		{//��������
			GetDlgItem(IDC_BUTTON_FAST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_SLOW)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
			break;
		}
	case 1:
		{//��ͣ
			GetDlgItem(IDC_BUTTON_FAST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SLOW)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);

			break;
		}
	case 2:
		{//ֹͣ
			GetDlgItem(IDC_BUTTON_FAST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SLOW)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
			break;
		}
	}
}

LRESULT CPlayerCtrlDlg::FilePlayOver( WPARAM wParam, LPARAM lParam )
{
	COnePlayer * pCamera = (COnePlayer *)this->GetParent();
	pCamera->m_AVIPlayer->CloseFile();
	PlayStop();
	if (pCamera->m_cameraID != 0)
	{
		CMediaSocket * media = theApp.g_cmd->GetMediaSocket(pCamera->m_cameraID);
		media->playedFile++;
		media->PlayFileInfoList.pop_front();
		if(!media->PlayFileInfoList.empty())//������һ����Ƶ
		{
			PlayFileInfo playInfo = media->PlayFileInfoList.front();
			pCamera->m_AVIPlayer->PlayFile(playInfo.fileName.c_str(),playInfo.GetFileSizeKB());
		}
		else
		{
			media->CloseConnect();
			pCamera->Invalidate();
		}
	}
	
	
	return 0;
}

void CPlayerCtrlDlg::PlayStart()
{
	m_iTimeHandle = 1;
	SetTimer(m_iTimeHandle,1000,NULL);
	SetPlayState(0);
	//SetWindowEnable(0);
}

void CPlayerCtrlDlg::PlayStop()
{
	//SetWindowEnable(2);
	KillTimer(m_iTimeHandle);
	m_playpos.SetPos(0);
}

//void CPlayerCtrlDlg::OnStnClickedSlowplay()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	GetDlgItem(IDC_SlowPlay)->EnableWindow(FALSE);
//}
