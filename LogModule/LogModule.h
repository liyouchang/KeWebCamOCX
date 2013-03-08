//**********************************************************************************************************************
//  Copyright (C), 2011-2012
//**********************************************************************************************************************

#pragma once
#ifndef _NOLOG
#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>
#include <iomanip>
#include <log4cplus/layout.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/ndc.h>
#include <log4cplus/helpers/loglog.h>
using namespace log4cplus;
#else
#define Logger int
#endif


using namespace std;

#define  Max_File_Name 100
#define  Max_File_Path 256

enum LogLevel
{
	LogLevel_DISABLE = 0,
	LogLevel_DEBUG  = 1,
	LogLevel_INFO,
	LogLevel_WARN,
	LogLevel_ERROR
	
};


class LogModule
{
public:
	LogModule(void);
	~LogModule(void);
	void SetLogLevel(int level);
	Logger GetLogger();
	static LogModule & Instance();
	static tstring GetCurrentPath();
protected:
	Logger _singleLogger;
	int logLevel;
	static LogModule  _instance;
};

#ifndef _NOLOG
#define  LOG_DEBUG(logEvent) \
	LOG4CPLUS_DEBUG(LogModule::Instance().GetLogger() , logEvent)
#define  LOG_INFO(logEvent) \
	LOG4CPLUS_INFO(LogModule::Instance().GetLogger() , logEvent)
#define  LOG_WARN(logEvent) \
	LOG4CPLUS_WARN(LogModule::Instance().GetLogger() , logEvent)
#define  LOG_ERROR(logEvent) \
	LOG4CPLUS_ERROR(LogModule::Instance().GetLogger() , logEvent)
#else
#define  LOG_DEBUG(logEvent)
#define  LOG_INFO(logEvent)
#define  LOG_WARN(logEvent)
#define  LOG_ERROR(logEvent)
#endif
