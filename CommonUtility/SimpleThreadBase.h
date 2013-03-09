#pragma once

#include <process.h> 
class SimpleThreadBase
{
public:
	SimpleThreadBase(void);
	virtual ~SimpleThreadBase(void);
	bool isRunning() const { return (flags & fRUNNING) != 0; }
	virtual void Run() =0 ;
	void Join ();
	virtual void Stop(DWORD dwTimeout = INFINITE);
	virtual void Start();
protected:
	static unsigned int __stdcall ThreadProc(void* arg);
	HANDLE _thread;
	enum Flags
	{
		fRUNNING  = 0x01,
		fJOINED   = 0x02
	};
	unsigned flags;
	volatile 	bool toStop;
	unsigned thread_id;
};
