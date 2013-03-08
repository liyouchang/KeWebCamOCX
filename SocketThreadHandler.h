#pragma once
#include "CommonUtility/SocketClientImpl.h"
#include "CommonUtility/TypedFifo.h"
#include "CommonUtility/SimpleThreadBase.h"
#include "KeMsg.h"
#include "Common.h"


class CSocketThreadHandler :
	public ISocketClientHandler,public SimpleThreadBase
{
	typedef SocketClientImpl<ISocketClientHandler> CSocketClient;
	friend CSocketClient;
	typedef CTypedFifo<BYTE> CByptFifo;

public:
	CSocketThreadHandler(void);
	virtual ~CSocketThreadHandler(void);

	virtual BOOL Init(const UINT nCnt);


	// SocketClient Interface handler
	virtual void OnThreadBegin(CSocketHandle* pSH);
	virtual void OnThreadExit(CSocketHandle* pSH);
	virtual void OnDataReceived(CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr);
	virtual void OnConnectionDropped(CSocketHandle* pSH){};
	virtual void OnConnectionError(CSocketHandle* pSH, DWORD dwError){};
	static void GetAddress(const SockAddrIn& addrIn, CString& rString) ;
protected:
	
 
	virtual bool CheckMessage(const BYTE* data, DWORD dwCount);
	virtual int HandleMessage(){return 0;};
	std::vector<BYTE> msgRecv;
	std::vector<BYTE> msgSend;
	CSocketClient m_SocketClient;
	SockAddrIn m_loacalAddr;
	CByptFifo m_recvBuf;
	int m_recvBufSize;
	//数据处理函数
	virtual void Run();
};


//add by lht for print log
std::ostream& operator<<(std::ostream& output, SockAddrIn& obj);
std::wostream& operator<<(std::wostream& output, SockAddrIn& obj);