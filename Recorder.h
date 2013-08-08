#pragma once
#include "commonutility\simplethreadbase.h"
#include "CommonUtility/TypedFifo.h"
#include "CommonUtility/tstdlibs.h"
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
	bool MakeRecordFileName(int cameraID);
	size_t GetFileSize(){return fileSize;}
	std::string GetFileName(){return fileName;}
	void SetTotalFileSize(int fileSize);
	int GetTotalFileSize(){return this->totalFileSize;}
	bool FreshToPlay();
	static CString GetRecordFilePath();
	int GetDownloadPercent();
protected:	
	std::string fileName;
	CByteFifo m_buf;
	size_t fileSize;
	size_t totalFileSize;
	bool bFresh;
};
