#pragma once
#include <inttypes.h>

typedef void(*retFunction)(uint8_t byte);
class ISerial
{
	public:

		virtual void run();
		virtual ~ISerial() {};
		virtual bool initSerial() = 0;
		virtual void setBaudRate(int baudRate) = 0;
		virtual void setCallBackFunction(retFunction interruptFunction) = 0;
		virtual int sendBytes(const char* bytesToSend, uint8_t size) = 0;
		virtual bool sendByte(uint8_t data) = 0;
};