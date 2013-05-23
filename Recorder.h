#pragma once
#include "commonutility\simplethreadbase.h"
#include "CommonUtility/TypedFifo.h"
#include <string>
#include <vector>
class CRecorder :
	public SimpleThreadBase
{
	typedef CTypedFifo<BYTE> CByteFifo;
public:
	CRecorder(void);
	virtual ~CRecorder(void);
	bool Initialize();
	virtual void Run();
	void SetFileName(const char * fileName);
	void InputBuf(const BYTE * buf, int len);
protected:	
	std::string fileName;
	CByteFifo m_buf;

};
