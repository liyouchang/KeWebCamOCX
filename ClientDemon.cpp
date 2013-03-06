#include "StdAfx.h"
#include "ClientDemon.h"

CClientDemon::CClientDemon(void)
{
		m_bIsPlay			= FALSE;
}

CClientDemon::~CClientDemon(void)
{
}

void CClientDemon::Play()
{
	m_bIsPlay = TRUE;
}

BOOL CClientDemon::IsPlay()
{
	return m_bIsPlay;
}
