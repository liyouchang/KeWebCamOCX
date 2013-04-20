#pragma once

#include "MyAVPlayer.h"

class CSocketThreadHandler;

class CMsgCallback
{
public:
	CMsgCallback(void);
	virtual ~CMsgCallback(void);
	virtual void StopRTVideo();
	virtual CMyAVPlayer * GetRealPlayer(int cameraID);
	virtual CSocketThreadHandler * GetMediaSocket();
};
