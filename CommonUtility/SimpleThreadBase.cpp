#include "StdAfx.h"
#include "SimpleThreadBase.h"
#include <stdexcept>

SimpleThreadBase::SimpleThreadBase(void)
{
	flags  = 0;	
	_thread  = INVALID_HANDLE_VALUE;
	toStop = false;
}

SimpleThreadBase::~SimpleThreadBase(void)
{
	if (_thread != INVALID_HANDLE_VALUE)
		::CloseHandle (_thread);
}

unsigned int __stdcall SimpleThreadBase::ThreadProc( void* arg )
{
	if ( !arg )
		return -1;
	SimpleThreadBase * ptr = static_cast<SimpleThreadBase*>(arg);

	ptr->Run();
	ptr->flags &= ~SimpleThreadBase::fRUNNING;
	return 0;
}

void SimpleThreadBase::Start()
{
	flags |= fRUNNING;
	_thread = reinterpret_cast<HANDLE>(
		::_beginthreadex (0, 0, ThreadProc, this, 0, &thread_id));
	if (! _thread)
	{
		flags &= ~fRUNNING;
		throw std::runtime_error("Thread creation was not successful");
	}
}

void SimpleThreadBase::Join()
{
	::WaitForSingleObject (_thread, INFINITE);
	flags |= fJOINED;
}

void SimpleThreadBase::Stop( DWORD dwTimeout/* = INFINITE */)
{
	toStop = true;
	if ( _thread != NULL )
	{
		if ( WaitForSingleObject(_thread, dwTimeout) == WAIT_TIMEOUT ) {
			TerminateThread(_thread, 1);
		}
		CloseHandle(_thread);
		_thread = NULL;
	}
}
