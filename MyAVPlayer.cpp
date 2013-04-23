#include "StdAfx.h"
#include "MyAVPlayer.h"
#include "CommonUtility/CommonFunctions.h"
#pragma comment(lib, "AVPlay.lib")

CMyAVPlayer::CMyAVPlayer(void)
{
	StreamID++;
	m_lPlayHandle = StreamID;
	m_lPause      = 1;
	m_hPlayWnd = NULL;
	int m_videoID = 0;
	int m_channelNo =0 ;
	
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
		WideCharToMultiByte(CP_ACP, 0, strFileName, -1, fileName, sizeof(fileName), NULL, NULL );
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
		m_PlayStatus = STA_QUESTDATA_FREE;
		return iRet;
	}
	iRet = AV_Play(m_lPlayHandle,m_hPlayWnd);
	if (iRet != 0 )
	{
		//TRACE("播放器播放失败 videoID=%d,channelNo=%d,iRet=%d\n",videoID,channelNo,iRet);
		m_PlayStatus = STA_QUESTDATA_FREE;
		return iRet;
	}
	m_PlayStatus = STA_PLAY;
	//AV_SetPlayVideoInfo(m_lPlayHandle,m_videoID,m_channelNo);
	return iRet;
}

void CMyAVPlayer::SetPlayWnd( HWND hWnd ,bool toInit)
{
		m_hPlayWnd = hWnd;
		AV_ReInitViewHandle(m_lPlayHandle,m_hPlayWnd);
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
	return (m_PlayStatus == STA_PLAY);
}

int CMyAVPlayer::CloseStream()
{
	//CSingleLock lock(&m_cs, TRUE);
	int ret = AV_CloseStream_Ex(m_lPlayHandle);
	m_PlayStatus = STA_QUESTDATA_FREE;

	return ret;
}

int CMyAVPlayer::CapPic( LPCTSTR pFileName )
{
	std::string fileName = tstr_to_str(pFileName);
	return AV_CapPic_Ex(m_lPlayHandle,fileName.c_str());
}

int CMyAVPlayer::InitPlayer()
{
	return AV_Initial(0);
}

int CMyAVPlayer::StreamID = 0;
