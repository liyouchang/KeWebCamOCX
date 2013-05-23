#include "StdAfx.h"
#include "Recorder.h"
#include "CommonUtility/LogModule.h"

CRecorder::CRecorder(void)
{
}

CRecorder::~CRecorder(void)
{
}

bool CRecorder::Initialize()
{
	const int BufSize = 0x10000;
	BOOL ret =  m_buf.Init(BufSize);
	if (ret == FALSE)
	{
		return false;
	}
	return  true;
}

void CRecorder::Run()
{
	const char * RECORDHEAD						= "KAERH.264";				//本地录像文件头
	const int onceWriteSize = 32*1024;
	 FILE * phFileHandle = fopen(fileName.c_str() ,"wb");//录像的文件句柄
	 if (phFileHandle == NULL)
	 {
		 LOG_ERROR("Open record file error!");
		 return;
	 }
	 fwrite(RECORDHEAD,1,9,phFileHandle);
	int state,len,iRet;
	int  iBigLen = 0;
	
	std::vector<BYTE> pBigBuffer;
	while(!toStop)
	{
		iBigLen = m_buf.GetCount();
		if (iBigLen  < onceWriteSize )
		{
			Sleep(50);
			continue;
		}		
		pBigBuffer.resize(iBigLen,0);
		int readLen = m_buf.Read(&pBigBuffer[0],iBigLen);
		if (readLen != iBigLen)
		{
			LOG_ERROR("Read record buffer error");
			continue;
		}
		iRet = fwrite(&pBigBuffer[0],iBigLen,1,phFileHandle);	
	}

	iBigLen = m_buf.GetCount();
	if (iBigLen  > 0 )
	{
		pBigBuffer.resize(iBigLen,0);
		int readLen = m_buf.Read(&pBigBuffer[0],iBigLen);
		iRet = fwrite(&pBigBuffer[0],iBigLen,1,phFileHandle);
	}		

	fclose(phFileHandle);
	
}

void CRecorder::SetFileName( const char * fileName )
{
	this->fileName = fileName;
}

void CRecorder::InputBuf( const BYTE * buf, int len )
{
	m_buf.Write(buf,len);
}
