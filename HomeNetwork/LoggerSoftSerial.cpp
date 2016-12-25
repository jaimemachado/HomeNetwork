#include "LoggerSoftSerial.h"

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

char LoggerSoftSerial::logBuffer[80];
LoggerSoftSerial* LoggerSoftSerial::instance = nullptr;

LoggerSoftSerial::LoggerSoftSerial()
{
	mySerial.begin(4800);
}

LoggerSoftSerial* LoggerSoftSerial::getInstance()
{
	if(instance == nullptr)
	{
		instance = new LoggerSoftSerial();
		logPrintf0(instance,"Log initialized!!!");
	}
	return instance;
}

void LoggerSoftSerial::log(const char* text)
{
	mySerial.println(text);
}
