#pragma once

#ifndef WIN32
#include <inttypes.h>
#else
#include <stdint.h>

#endif

typedef void(*retFunction)(uint8_t byte);
class ISerial
{
	public:

		virtual void run() = 0;
		virtual ~ISerial() {};
		virtual bool initSerial(const char* port, int baudrate) = 0;
		virtual void setCallBackFunction(retFunction interruptFunction) = 0;
		virtual int sendBytes(const unsigned char* bytesToSend, uint8_t size) = 0;
		virtual bool sendByte(unsigned char data) = 0;
};