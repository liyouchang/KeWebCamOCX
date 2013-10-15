#include "StdAfx.h"
#include "MyAVPlayer.h"
#include "CommonUtility/CommonFunctions.h"
#include "CommonUtility/LogModule.h"
#include "Common.h"
#pragma comment(lib, "AVPlay.lib")

CMyAVPlayer::CMyAVPlayer(void)
{
	StreamID++;
	if (StreamID>35)
	{
		StreamID=1;
	}
	m_lPlayHandle = StreamID;
	m_lPause      = 1;
	m_hPlayWnd = NULL;
	int m_videoID = 0;
	int m_channelNo =0 ;
	m_PlayStatus = PLAYSTATUS_Free;
	m_iPlaySpeed = 0;
	//AV_Initial(0);
	m_CtrlDlg = NULL;
}

CMyAVPlayer::~CMyAVPlayer(void)
{
	//AV_Free();
}

//************************************
// Method:    PlayFile
// FullName:  CMyAVPlayer::PlayFile
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const char * fileName
// Parameter: int fileSize   文件大小，KB
// Parameter: HWND endMsgWnd
//************************************
int CMyAVPlayer::PlayFile(const char * fileName,int fileSize,HWND endMsgWnd)
{
	m_lPlayHandle = 65;  //回放句柄范围  65~96
	long lRet = AV_OpenFile_Ex(m_lPlayHandle,(char *)fileName,fileSize);
	if (lRet == 0)
	{
		if (endMsgWnd == NULL )
		{
			endMsgWnd = m_CtrlDlg->GetSafeHwnd();
		//	m_CtrlDlg->OnBnClickedButtonPlay();
		}
		AV_SetFileEndMsgWnd(m_lPlayHandle, (long)endMsgWnd, WM_PLAYOVER);
		lRet = AV_Play(m_lPlayHandle,m_hPlayWnd);
		if (lRet != 0)
		{
			TRACE(_T("播放失败"));
			return lRet;
		}
		//AV_SetSpeed(m_lPlayHandle,m_iPlaySpeed);
		this->NormalPlay();
		m_PlayStatus = PLAYSTATUS_PlayStream;
		//m_CtrlDlg->PlayStart();
	}else{
		TRACE(TEXT("打开文件失败"));
	}
	return lRet;
}

int CMyAVPlayer::OpenStream()
{
	int iRet = AV_OpenStream_Ex(m_lPlayHandle);
	if (iRet != 0)
	{
		trace("播放器打开失败 iRet=%d\n",iRet);
		m_PlayStatus = PLAYSTATUS_Free;
		return iRet;
	}
	iRet = AV_Play(m_lPlayHandle,m_hPlayWnd);
	if (iRet != 0 )
	{
		LOG_ERROR("播放器播放失败 iRet="<<iRet);
		m_PlayStatus = PLAYSTATUS_Free;
		return iRet;
	}
	m_PlayStatus = PLAYSTATUS_PlayStream;
	//AV_SetPlayVideoInfo(m_lPlayHandle,m_videoID,m_channelNo);
	return iRet;
}

void CMyAVPlayer::SetPlayWnd( HWND hWnd )
{
		m_hPlayWnd = hWnd;
		if (IsPlaying())
		{
			int ret = AV_ReInitViewHandle(m_lPlayHandle,m_hPlayWnd);
			if (ret != 0)
			{
				LOG_WARN("AV_ReInitViewHandle error!");
			}
		}
		
}

int CMyAVPlayer::InputStream( const BYTE * data, int dataLen )
{
	//CSingleLock lock(&m_cs, TRUE);
	AV_InputData(m_lPlayHandle,(long *)(data) , dataLen);
	return 0;
}

bool CMyAVPlayer::IsPlaying()
{
	//CSingleLock lock(&m_cs, TRUE);
	return (m_PlayStatus == PLAYSTATUS_PlayStream);
}

int CMyAVPlayer::CloseStream()
{
	//CSingleLock lock(&m_cs, TRUE);
	int ret = AV_CloseStream_Ex(m_lPlayHandle);
	m_PlayStatus = PLAYSTATUS_Free;
	return ret;
}

int CMyAVPlayer::CapPic( LPCTSTR pFileName )
{
	std::string fileName = tstr_to_str(pFileName);
	return AV_CapPic_Ex(m_lPlayHandle,fileName.c_str());
	//return 0;
}

int CMyAVPlayer::InitPlayer()
{
	if (isInitail == 0) 
		isInitail = 1;
	else
		return 0;
	int ret = AV_Initial(0);
	if (ret != 0)
	{
		LOG_WARN("AV_Initial Error!");
	}
	return ret;
}

int CMyAVPlayer::FreePlayer()
{
	if (isInitail == 0)
	{
		return 0;
	}
	isInitail = 0;
	return AV_Free();
}

int CMyAVPlayer::OpenSound()
{
	return AV_SoundPlay(m_lPlayHandle);
}

int CMyAVPlayer::CloseSound()
{
	return AV_StopSound(m_lPlayHandle);
}

int CMyAVPlayer::GetAudioData( char * pBuf )
{
	return AV_GetAudioData(pBuf);
}

int CMyAVPlayer::StartTalk()
{
	AV_InitAudioTalk();
	return AV_TalkOpen();
}

int CMyAVPlayer::StopTalk()
{
	return AV_TalkClose();
}

int CMyAVPlayer::SetSpeed( int speed )
{
	if (speed >=-4 || speed <=4)
	{
		this->m_iPlaySpeed = speed;
		return AV_SetSpeed(m_lPlayHandle,m_iPlaySpeed);
	}
	return -1;
}

int CMyAVPlayer::ChgSpeed( int chgSpeed )
{
	m_iPlaySpeed += chgSpeed;
	if (m_iPlaySpeed>4)
	{
		m_iPlaySpeed = 4;
	}
	if (m_iPlaySpeed < -4)
	{
		m_iPlaySpeed = -4;
	}
	return AV_SetSpeed(m_lPlayHandle,m_iPlaySpeed);
}

int CMyAVPlayer::SetPlayPos( int pos )
{
	return AV_SetPlayPos(m_lPlayHandle, pos);
}

int CMyAVPlayer::CloseFile()
{
	m_PlayStatus = PLAYSTATUS_Free;
	return AV_CloseFile_Ex(m_lPlayHandle);
}

int CMyAVPlayer::PausePlay()
{
	return AV_Pause(m_lPlayHandle,0);
}

int CMyAVPlayer::GetPlayPos( int &pos )
{
	return AV_GetPlayPos(m_lPlayHandle,(long*)&pos);
}

int CMyAVPlayer::NormalPlay()
{
	m_iPlaySpeed = 0;
	return AV_BackPlayNormal(m_lPlayHandle);
}

void CMyAVPlayer::SetCtrlDlg( CPlayerCtrlDlg * dlg )
{
	this->m_CtrlDlg = dlg;
}

int CMyAVPlayer::SetFileSize( long fileSize )
{
	return AV_SetFileSize(m_lPlayHandle,fileSize);
}

float CMyAVPlayer::GetPlayFluxRate()
{
	return AV_GetPlayFluxRate(m_lPlayHandle);
}

int CMyAVPlayer::GetPlayFrameRate()
{
	return AV_GetPlayFrameRate(m_lPlayHandle);
}

//************************************
// Method:    GetEncodeFormat
// FullName:  CMyAVPlayer::GetEncodeFormat
// Access:    public 
// Returns:   int
// 0: strName := 'D1';
// 1: strName := 'QCIF';
// 2: strName := 'CIF';
// 3: strName := 'HD1';
// 4: strName := 'QVGA';
// 5: strName := 'UXGA';
// 6: strName := '1080P';
// 7: strName := 'VGA';
// 8: strName := 'QQVGA';
// 9: strName := 'D2';
// 10: strName := 'SVGA';
// 11: strName := 'XGA';
// 12: strName := '720P';
// 13: strName := 'WXGA';
// 14: strName := '960P';
// Qualifier:
//************************************
int CMyAVPlayer::GetEncodeFormat()
{
	//const char * encodeName[] = {"D1","QCIF","CIF","HD1","QVGA","UXGA","1080P","VGA","QQVGA","D2","SVGA","XGA","720P","WXGA","960P"};
	int code =  AV_GetEncodeFormat(m_lPlayHandle);
	return code;
}

CString CMyAVPlayer::GetEncodeFormatName()
{
	const TCHAR * encodeName[] = {_T("D1"),_T("QCIF"),_T("CIF"),_T("HD1"),_T("QVGA"),_T("UXGA"),
		_T("1080P"),_T("VGA"),_T("QQVGA"),_T("D2"),_T("SVGA"),_T("XGA"),_T("720P"),_T("WXGA"),_T("960P")};
	int code =  AV_GetEncodeFormat(m_lPlayHandle);
	if (code < 0)
	{
		return _T("");
	}
	
	return encodeName[code];
	
}

// int CMyAVPlayer::SetFileEndMsgWnd( HWND fileWnd )
// {
// 	return AV_SetFileEndMsgWnd(m_lPlayHandle, (long)fileWnd, WM_PLAYOVER);
// }

int CMyAVPlayer::isInitail = 0;

int CMyAVPlayer::StreamID = 0;
