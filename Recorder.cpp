#include "StdAfx.h"
#include "Recorder.h"
#include "CommonUtility/LogModule.h"
#include "CommonUtility/inifile.h"
#include "KeWebCamOCX.h"

CRecorder::CRecorder(void)
{
	totalFileSize = 0;
}

CRecorder::~CRecorder(void)
{
}

bool CRecorder::Initialize()
{
	const int BufSize = 1024*128;
	BOOL ret =  m_buf.Init(BufSize);
	if (ret == FALSE)
	{
		return false;
	}
	return  true;
}

void CRecorder::Run()
{
	const int onceWriteSize = 32*1024;
	 FILE * phFileHandle = fopen(fileName.c_str() ,"wb");//录像的文件句柄
	 if (phFileHandle == NULL)
	 {
		 LOG_ERROR("Open record file error!");
		 return;
	 }
	 this->fileSize = 0;
	 int iRet;
	m_buf.Drain(m_buf.GetCount());
//	const char * RECORDHEAD	= "KAERH.264";				//本地录像文件头
//	iRet = fwrite(RECORDHEAD,1,9,phFileHandle);
//	fileSize += iRet;

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
		iRet = fwrite(&pBigBuffer[0],1,iBigLen,phFileHandle);	
		this->fileSize += iRet;
	}	

	iBigLen = m_buf.GetCount();
	if (iBigLen  > 0 )
	{
		pBigBuffer.resize(iBigLen,0);
		int readLen = m_buf.Read(&pBigBuffer[0],iBigLen);
		iRet = fwrite(&pBigBuffer[0],iBigLen,1,phFileHandle);
		this->fileSize += iRet;
	}		

	fclose(phFileHandle);
	
}

void CRecorder::SetFileName( const char * fileName )
{
	this->fileName = fileName;
	this->fileSize = 0;
}

void CRecorder::InputBuf( const BYTE * buf, int len )
{
	m_buf.Write(buf,len);
}

//************************************
// Method:    MakeRecordFileName 根据cameraID和当前时间生成录像文件名称
// FullName:  CRecorder::MakeRecordFileName
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: int cameraID
//************************************
bool CRecorder::MakeRecordFileName( int cameraID )
{
	CString fullPath;
	CString fileName ;
	CString filePath;
	CString extName = _T("h264");
	SYSTEMTIME sys;

	GetLocalTime( &sys );
	fileName.Format(_T("%d_%04d%02d%02d%02d%02d%02d%03d.%s"), cameraID,
		sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,extName);

	CIniFile ini;
	bool retValue = ini.Load(theApp.g_pMainWnd->iniFile);
	if(!retValue)
	{
		TRACE(_T("Load WebClient.ini file error"));
		return false;
	}
	CString recordPath = ini.GetKeyValue(_T("LocalPath"),_T("RecordFilePath"),_T("c:\\Record\\")).c_str();

	if (recordPath.GetAt(recordPath.GetLength()-1) !=_T('\\'))
	{
		recordPath.AppendChar(_T('\\'));
	}
	filePath.Format(_T("%s%d\\"),recordPath,cameraID);

	if (!FolderExist(filePath))
	{
		CreateFolderEx(filePath);
		//if(!CreateFolderEx(filePath))
		//	return false;
	}
	fullPath = filePath + fileName;
	this->fileName = tstd::tstr_to_str(fullPath.GetString());
this->fileSize = 0;
	return true;
}

CString CRecorder::GetRecordFilePath()
{
	CIniFile ini;
	bool retValue = ini.Load(theApp.g_pMainWnd->iniFile);
	if(!retValue)
	{
		TRACE(_T("Load WebClient.ini file error"));
		return _T("");
	}
	CString recordPath = ini.GetKeyValue(_T("LocalPath"),_T("RecordFilePath"),_T("c:\\Record\\")).c_str();

	if (recordPath.GetAt(recordPath.GetLength()-1) !=_T('\\'))
	{
		recordPath.AppendChar(_T('\\'));
	}
	return recordPath;
}

bool CRecorder::FreshToPlay()
{
	if (bFresh && GetFileSize()> 100000)
	{
		bFresh = false;
		return true;
	}
	return false;
}

//************************************
// Method:    SetTotalFileSize 设置总文件大小，在录像开始时使用
// FullName:  CRecorder::SetTotalFileSize
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int fileSize
//************************************
void CRecorder::SetTotalFileSize( int fileSize )
{
	this->totalFileSize = fileSize*1000;
	this->bFresh = true;
}

int CRecorder::GetDownloadPercent()
{
	unsigned int currentfileSize = GetFileSize();
	unsigned int totalBytes = GetTotalFileSize();
	unsigned int percent = 0;
	if (currentfileSize<totalBytes)
	{
		float dTmp =(float) currentfileSize/(float)totalFileSize;
		percent = dTmp*100;
		
	}
	else
	{
		percent =  100; 
	}
	return percent;
}
