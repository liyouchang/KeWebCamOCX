#pragma once
#include "CommonUtility/SocketClientImpl.h"
#include "CommonUtility/TypedFifo.h"
#include "CommonUtility/SimpleThreadBase.h"
#include "CommonUtility/LogModule.h"
#include "Common.h"
#include "KeMsg.h"
#include <map>


const int AF_IPV4   = 0;
const int AF_IPV6   = 1;
const int SOCK_TCP  = SOCK_STREAM-1;
const int SOCK_UDP  = SOCK_DGRAM-1;

class  RecvMsgEvent
{
public:
	RecvMsgEvent();
	virtual ~RecvMsgEvent();
	void ResetMsgEvent(int msgType = 0);
	DWORD WaitMsgEvent(int timeout);
	void SetMsgEvent(){ pEvent->SetEvent();}
	int msgType;
	CEvent* pEvent ;		//申请key
	int respData;	//传递消息读取的数据,resp函数生成,使用者释放
	void * respPtr;//返回的数据
	int dataType;//
};


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
	
	virtual bool IsConnect();
	virtual void CloseConnect();
	// SocketClient Interface handler
	virtual void OnThreadBegin(CSocketHandle* pSH);
	virtual void OnThreadExit(CSocketHandle* pSH);
	virtual void OnDataReceived(CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr);
	virtual void OnConnectionDropped(CSocketHandle* pSH);
	virtual void OnConnectionError(CSocketHandle* pSH, DWORD dwError);

//recv data event. Noticy: Avoid wait the same message in deferent thread at the same time.
	virtual int WaitRecvMsg(int msgType , int timeout = MSG_WAIT_TIMEOUT,void ** respPtr = NULL);
	virtual void SetRecvMsg(int msgType,int respData,void * respPtr = NULL);
	virtual int GetRecvMsgData(int msgType ,void ** respPtr = NULL);
	std::map<int ,RecvMsgEvent > msgEventMap;
	//std::vector<RecvMsgEvent> msgEventMap; 
public:
	static void GetAddress(const SockAddrIn& addrIn, CString& rString) ;
	
	
protected:
	virtual bool GetMessageData();
	virtual bool CheckMessage(const BYTE* data, DWORD dwCount);
	virtual void HandleMessage(const BYTE* msgData){};
	std::vector<BYTE> m_MsgRecv;//读取消息临时缓冲区
	//std::vector<BYTE> msgSend;
	CSocketClient m_SocketClient;
	SockAddrIn m_loacalAddr;
	CByteFifo m_recvBuf;
	int m_recvBufSize;
	CString m_serverPort; //服务器 port 22616
	CString m_serverIP;
	int m_nSockType;
	//数据处理函数
	virtual void Run();
};


//add by lht for print log
std::ostream& operator<<(std::ostream& output, const SockAddrIn& obj);
std::wostream& operator<<(std::wostream& output,const  SockAddrIn& obj);