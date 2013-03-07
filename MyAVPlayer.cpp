#include "StdAfx.h"
#include "MyAVPlayer.h"

#pragma comment(lib, "AVPlay.lib")

CMyAVPlayer::CMyAVPlayer(void)
{
	m_lPlayHandle = 0;
	m_lPause      = 1;
	AV_Initial(0);

}

CMyAVPlayer::~CMyAVPlayer(void)
{
	AV_Free();
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
