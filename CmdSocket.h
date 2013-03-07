#pragma once
#include "socketclientimpl.h"

class CCmdSocket 
{
	typedef SocketClientImpl<ISocketClientHandler> CSocketClient;
	friend CSocketClient;
public:
	CCmdSocket(void);
	virtual ~CCmdSocket(void);

	// SocketClient Interface handler
	//virtual void OnThreadBegin(CSocketHandle* pSH);
	//virtual void OnThreadExit(CSocketHandle* pSH);
	virtual void OnDataReceived(CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr);
	//virtual void OnConnectionDropped(CSocketHandle* pSH);
	//virtual void OnConnectionError(CSocketHandle* pSH, DWORD dwError);

};
