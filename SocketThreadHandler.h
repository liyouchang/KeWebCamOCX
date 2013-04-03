#pragma once
#include "CommonUtility/SocketClientImpl.h"
#include "CommonUtility/TypedFifo.h"
#include "CommonUtility/SimpleThreadBase.h"
#include "CommonUtility/LogModule.h"
#include "Common.h"
#include "KeMsg.h"


const int AF_IPV4   = 0;
const int AF_IPV6   = 1;
const int SOCK_TCP  = SOCK_STREAM-1;
const int SOCK_UDP  = SOCK_DGRAM-1;


class CSocketThreadHandler :
	public ISocketClientHandler,public SimpleThreadBase
{
	typedef SocketClientImpl<ISocketClientHandler> CSocketClient;
	friend CSocketClient;
	typedef CTypedFifo<BYTE> CByteFifo;

public:
	CSocketThreadHandler(void);
	CSocketThreadHandler(const UINT nCnt);
	virtual ~CSocketThreadHandler(void);

	virtual bool Init(const UINT nCnt );
	DWORD Write(const LPBYTE lpBuffer,DWORD dwCount);

	bool IsConnect();
	void CloseConnect();
	// SocketClient Interface handler
	virtual void OnThreadBegin(CSocketHandle* pSH);
	virtual void OnThreadExit(CSocketHandle* pSH);
	virtual void OnDataReceived(CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr);
	virtual void OnConnectionDropped(CSocketHandle* pSH){};
	virtual void OnConnectionError(CSocketHandle* pSH, DWORD dwError){};
	static void GetAddress(const SockAddrIn& addrIn, CString& rString) ;
protected:
	
	virtual bool CheckMessage(const BYTE* data, DWORD dwCount);
	virtual void HandleMessage(const BYTE* msgData){};
	//std::vector<BYTE> msgRecv;
	//std::vector<BYTE> msgSend;
	CSocketClient m_SocketClient;
	SockAddrIn m_loacalAddr;
	CByteFifo m_recvBuf;
	int m_recvBufSize;

	CString m_serverPort;//服务器port 22616
	CString m_serverIP;
	int m_nSockType;
	//数据处理函数
	virtual void Run();
};


//add by lht for print log
std::ostream& operator<<(std::ostream& output, const SockAddrIn& obj);
std::wostream& operator<<(std::wostream& output,const  SockAddrIn& obj);