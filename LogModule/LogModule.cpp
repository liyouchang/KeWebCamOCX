//**********************************************************************************************************************
//  Copyright (C), 2011
//**********************************************************************************************************************

#include "stdafx.h"

#include "LogModule.h"
#include <string.h>
#include "inifile.h"

LogModule::LogModule(void)
{
	tstring iniFile = _T("log.ini");
    CIniFile ini;

	bool retValue = ini.Load(iniFile);
	if(!retValue)
	{
		TRACE(_T("Load config file error"));
		return;
	}
#ifndef _NOLOG
	tstring logFileName = ini.GetKeyValue(_T("log"),_T("logFileName"),_T("log.log"));
	tstring sLogLevel = ini.GetKeyValue(_T("log"),_T("logLevel"),_T("0"));
	logLevel = _ttoi (sLogLevel.c_str());
	int maxBackupIndex = 2;
	long maxFileSize = 10*1024*1024;
	if(logLevel != LogLevel_DISABLE)
	{
		SharedAppenderPtr append_1(
			new RollingFileAppender(logFileName,maxFileSize,maxBackupIndex));
		append_1->setName(LOG4CPLUS_TEXT("Lib_CheckPrinter"));
		 log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%m/%d/%y %H:%M:%S,%Q} [%t] %-5p - %m [%l]%n");
		append_1->setLayout( std::auto_ptr<Layout>(new PatternLayout(pattern)));
		Logger::getRoot().addAppender(append_1);
	}
	_singleLogger = Logger::getInstance(LOG4CPLUS_TEXT("Lib_CheckPrinter"));
	SetLogLevel(logLevel);
#endif
}

LogModule::~LogModule(void)
{
}

LogModule & LogModule::Instance()
{
	return _instance;
}


void LogModule::SetLogLevel( int level )
{
#ifndef _NOLOG
	switch (level)
	{
	case LogLevel_DEBUG:
		_singleLogger.setLogLevel(DEBUG_LOG_LEVEL);
		break;
	case LogLevel_INFO:
		_singleLogger.setLogLevel(INFO_LOG_LEVEL);
		break;
	case LogLevel_WARN:
		_singleLogger.setLogLevel(WARN_LOG_LEVEL);
		break;
	case LogLevel_ERROR:
		_singleLogger.setLogLevel(ERROR_LOG_LEVEL);
		break;
	case LogLevel_DISABLE:
		_singleLogger.setLogLevel(OFF_LOG_LEVEL);
		break;
	}
#endif
}

log4cplus::tstring LogModule::GetCurrentPath()
{
	tstring retPath= _T("./");
#ifdef WIN32
	TCHAR filePath[MAX_PATH];
	HMODULE hModule= NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,(PCTSTR)LogModule::GetCurrentPath().c_str(),&hModule);
	GetModuleFileName(hModule, filePath, MAX_PATH);
	//	PathRemoveFileSpec(filePath);
	retPath = filePath;
	int len = retPath.rfind(_T("\\"));
	retPath = retPath.substr(0,len+1);
#endif

	return retPath;
}

Logger LogModule::GetLogger()
{
	return _singleLogger;
}


LogModule LogModule::_instance;



