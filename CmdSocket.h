#pragma once
#include "SocketThreadHandler.h"
#include <afxmt.h>



class CCmdSocket :public CSocketThreadHandler
{
	
public:
	CCmdSocket(void);
	virtual ~CCmdSocket(void);

	// SocketClient Interface handler
	//virtual void OnThreadBegin(CSocketHandle* pSH);
	virtual void OnThreadExit(CSocketHandle* pSH);
	virtual void OnDataReceived(CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr);
	//virtual void OnConnectionDropped(CSocketHandle* pSH);
	//virtual void OnConnectionError(CSocketHandle* pSH, DWORD dwError);

	bool ConnectToServer(CString severAddr);
	int LoginServer(CString userName,CString pwd);

	//************************************
	// Method:    AskKeyt 申请密钥
	// Access:    public 
	// Returns:   int
	// Qualifier:
	//************************************
	int AskKeyt();

protected:

private:
	
	CString m_UserName;//用户名
	CString m_Password;
	CString serverPort;//服务器port 22616
	


	CEvent eventKeyt;		//申请key


};
