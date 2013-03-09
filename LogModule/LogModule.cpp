//**********************************************************************************************************************
//  Copyright (C), 2011
//**********************************************************************************************************************

#include "stdafx.h"
#include "LogModule.h"
#include "inifile.h"



log4cplus::tstring GetCurrentPath()
{
	tstring retPath= _T("./");
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

void InitLogModule()
{

	log4cplus::tstring iniFile =GetCurrentPath() + _T("log.ini");
	CIniFile ini;

	bool retValue = ini.Load(iniFile);
	if(!retValue)
	{
		TRACE(_T("Load config file error"));
		return;
	}
#ifndef _NOLOG
	log4cplus::tstring logFileName = ini.GetKeyValue(_T("log"),_T("logFileName"),_T("log.log"));
	logFileName = GetCurrentPath() + logFileName;
	log4cplus::tstring sLogLevel = ini.GetKeyValue(_T("log"),_T("logLevel"),_T("OFF_LOG_LEVEL"));


	log4cplus::LogLevel level = getLogLevelManager().fromString(sLogLevel);
	Logger::getRoot().setLogLevel(level);

	//int maxBackupIndex = 2;
	//long maxFileSize = 10*1024*1024;
	if(level  != NOT_SET_LOG_LEVEL)
	{
		SharedAppenderPtr append_1(new RollingFileAppender(logFileName));
		//SharedAppenderPtr append_1(new FileAppender(logFileName));
		append_1->setName(LOG4CPLUS_TEXT("myAppender"));
		log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%m/%d/%y %H:%M:%S,%Q} [%t] %-5p - %m [%l]%n");
		append_1->setLayout( std::auto_ptr<Layout>(new PatternLayout(pattern)));
		Logger::getRoot().addAppender(append_1);
	}

#endif
}



