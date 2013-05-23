#pragma once
#include "../CommonUtility/SimpleThreadBase.h"
#include "../SocketThreadHandler.h"
#include <list>
#include <vector>
using namespace std;

class ResendMsg
{
public:
	ResendMsg(vector<BYTE> data,int id)
	{
		time = 0;
		lastSendTime = GetTickCount();
		this->data = data;
		this->eachResendTime = 3000;
		this->msgID = id;
	}
	~ResendMsg(){}
	bool operator==(const int id){return this->msgID == id;}
	int msgID;
	vector<BYTE> data;
	int time;//重发次数
	long lastSendTime;//上次重发时间
	int eachResendTime;
};

template <class T> class PredicateID   : public std::unary_function<T, bool> 
{
public:
	PredicateID(int id){m_ID = id;}
	bool operator( ) ( T& val ) 
	{
		return ( val == m_ID );
	}
	int m_ID;
};



class CResendThread :
	public SimpleThreadBase
{
public:
	CResendThread(CSocketThreadHandler * socketHandle);
	virtual ~CResendThread(void);
	virtual void Run();
	void AddResendMsg(int id,vector<BYTE> data);
	void RemoveResendMsg(int id);
	int GetResendTime(int id);
protected:
	CCriticalSection m_cs; // sinchronicity
	CSocketThreadHandler * m_socketHandle;
	std::list<ResendMsg> m_resendMsgList;

};
