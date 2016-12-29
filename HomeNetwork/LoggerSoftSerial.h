// Logger.h

#ifndef _LOGGER_h
#define _LOGGER_h
#include <stdio.h>


#ifndef WIN32_

#define logPrintf0(logger, a) sprintf(logger->logBuffer, a); logger->log(logger->logBuffer);

#define logPrintf1(logger, a, b) sprintf(logger->logBuffer, a, b); logger->log(logger->logBuffer);

#define logPrintf2(logger, a, b, c) sprintf(logger->logBuffer, a, b, c); logger->log(logger->logBuffer);

#else

#define logPrintf0(logger, a) printf(a);

#define logPrintf1(logger, a, b) printf(a, b); 

#define logPrintf2(logger, a, b, c) printf(a, b, c); 

#endif



class LoggerSoftSerial
{
	static LoggerSoftSerial* instance;
	

	LoggerSoftSerial();

public:
	static char logBuffer[80];
	static LoggerSoftSerial* getInstance();

	void log(const char* text);



};


#endif

