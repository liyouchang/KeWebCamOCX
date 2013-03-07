#pragma once

#include "AVPlay.h"

class CMyAVPlayer
{
public:
	CMyAVPlayer(void);
	virtual ~CMyAVPlayer(void);


	void PlayFile(HWND hWnd);
private:
	long m_lPlayHandle;
	long  m_lPause;
};
