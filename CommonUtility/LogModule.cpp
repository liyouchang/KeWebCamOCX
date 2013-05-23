//**********************************************************************************************************************
//  Copyright (C), 2011
//**********************************************************************************************************************

#include "stdafx.h"
#include "LogModule.h"
#include "inifile.h"


#ifndef _NOLOG
log4cplus::tstring GetCurrentPath()
{
	log4cplus::tstring retPath= _T("./");
#ifdef WIN32
	TCHAR filePath[MAX_PATH];
	HMODULE hModule= NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,(PCTSTR)GetCurrentPath,&hModule);
	GetModuleFileName(hModule, filePath, MAX_PATH);
	//	PathRemoveFileSpec(filePath);
	retPath = filePath;
	int len = retPath.rfind(_T("\\"));
	retPath = retPath.substr(0,len+1);
#endif

	return retPath;
}
#endif

void InitLogModule()
{
	static int isInitail = 0;
	if (isInitail == 0) 
		isInitail = 1;
	else
		return;
	//to support chinnese
	std::locale::global(std::locale(""));

#ifndef _NOLOG
	log4cplus::tstring iniFile =GetCurrentPath() + _T("log.ini");
	//iniFile =_T("E:\\workspace\\KeWebCamOCX\\bin\\log.ini");
	CIniFile ini;

	bool retValue = ini.Load(iniFile);
	if(!retValue)
	{
		TRACE(_T("Load config file error"));
		return;
	}

	log4cplus::tstring logFileName = ini.GetKeyValue(_T("log"),_T("logFileName"),_T("log.log"));
	logFileName = GetCurrentPath() + logFileName;
	//logFileName = _T("E:\\workspace\\KeWebCamOCX\\ÍøÂçÉãÏñÍ·²¥·Å²å¼þ\\logthat.log");
	log4cplus::tstring sLogLevel = ini.GetKeyValue(_T("log"),_T("logLevel"),_T("OFF_LOG_LEVEL"));


	log4cplus::LogLevel level = log4cplus::getLogLevelManager().fromString(sLogLevel);
	log4cplus::Logger::getRoot().setLogLevel(level);

	//int maxBackupIndex = 2;
	//long maxFileSize = 10*1024*1024;
	if(level  != log4cplus::NOT_SET_LOG_LEVEL)
	{
		log4cplus::SharedAppenderPtr append_1(new log4cplus::RollingFileAppender(logFileName));
		//SharedAppenderPtr append_1(new FileAppender(logFileName));
		append_1->setName(LOG4CPLUS_TEXT("myAppender"));
		log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%m/%d/%y %H:%M:%S,%Q} [%t] %-5p - %m [%l]%n");
		append_1->setLayout( std::auto_ptr<log4cplus::Layout>(new log4cplus::PatternLayout(pattern)));
		log4cplus::Logger::getRoot().addAppender(append_1);
	}

#endif
}



