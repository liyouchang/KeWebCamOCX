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
//using namespace log4cplus;
#else
#define Logger int
#endif



enum LogLevel
{
	LogLevel_DISABLE = 0,
	LogLevel_DEBUG  = 1,
	LogLevel_INFO,
	LogLevel_WARN,
	LogLevel_ERROR
	
};

void InitLogModule();



#ifndef _NOLOG
#define  LOG_DEBUG(logEvent) \
	LOG4CPLUS_DEBUG(log4cplus::Logger::getRoot() , logEvent<<"  <"__FUNCTION__<<">  ")
#define  LOG_INFO(logEvent) \
	LOG4CPLUS_INFO(log4cplus::Logger::getRoot() , logEvent<<"  <"__FUNCTION__<<">  ")
#define  LOG_WARN(logEvent) \
	LOG4CPLUS_WARN(log4cplus::Logger::getRoot() , logEvent<<"  <"__FUNCTION__<<">  ")
#define  LOG_ERROR(logEvent) \
	LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), logEvent<<"  <"__FUNCTION__<<">  ")
#else
#define  LOG_DEBUG(logEvent)
#define  LOG_INFO(logEvent)
#define  LOG_WARN(logEvent)
#define  LOG_ERROR(logEvent)
#endif
