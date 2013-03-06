#pragma once

class CClientDemon
{
public:
	CClientDemon(void);
	virtual ~CClientDemon(void);
	BOOL	m_bIsPlay;
	void	Play();
		BOOL	IsPlay();
};
