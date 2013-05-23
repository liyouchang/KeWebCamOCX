#include "StdAfx.h"
#include "MyAVPlayer.h"
#include "CommonUtility/CommonFunctions.h"
#include "CommonUtility/LogModule.h"
#pragma comment(lib, "AVPlay.lib")

CMyAVPlayer::CMyAVPlayer(void)
{
	StreamID++;
	m_lPlayHandle = StreamID;
	m_lPause      = 1;
	m_hPlayWnd = NULL;
	int m_videoID = 0;
	int m_channelNo =0 ;
	m_PlayStatus = PLAYSTATUS_Free;
	//AV_Initial(0);

}

CMyAVPlayer::~CMyAVPlayer(void)
{
	//AV_Free();
}

void CMyAVPlayer::PlayFile(HWND hWnd)
{
	if (m_lPlayHandle>0)
	{
		AV_CloseFile_Ex(m_lPlayHandle);
		m_lPlayHandle = 0;
	}
	m_lPlayHandle = 65;  //回放句柄范围  65~96
	CFileDialog fDlg(TRUE);
	if (fDlg.DoModal() == IDOK)
	{
		CString strFileName = fDlg.GetPathName();
		char fileName[MAX_PATH];
		//WideCharToMultiByte(CP_ACP, 0, strFileName, -1, fileName, sizeof(fileName), NULL, NULL );
		TRACE(fileName);
		long lRet = AV_OpenFile_Ex(m_lPlayHandle,fileName);
		if (lRet == 0)
		{
			lRet = AV_Play(m_lPlayHandle,hWnd);
			if (lRet != 0)
			{
				AfxMessageBox(_T("播放失败"));
			}
		}else{
			AfxMessageBox(TEXT("打开文件失败"));
		}
	}
}

int CMyAVPlayer::OpenStream()
{
	int iRet = AV_OpenStream_Ex(m_lPlayHandle);
	if (iRet != 0)
	{
		TRACE("播放器打开失败 iRet=%d\n",iRet);
		m_PlayStatus = PLAYSTATUS_Free;
		return iRet;
	}
	iRet = AV_Play(m_lPlayHandle,m_hPlayWnd);
	if (iRet != 0 )
	{
		LOG_ERROR("播放器播放失败 iRet=",iRet);
		m_PlayStatus = PLAYSTATUS_Free;
		return iRet;
	}
	m_PlayStatus = PLAYSTATUS_Play;
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
	return (m_PlayStatus == PLAYSTATUS_Play);
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

int CMyAVPlayer::isInitail = 0;

int CMyAVPlayer::StreamID = 0;
