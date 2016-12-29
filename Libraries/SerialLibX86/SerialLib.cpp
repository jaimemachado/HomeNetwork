
#include "SerialLib.h"

SerialLib::SerialLib()
{
	serialPtr = nullptr;
}

SerialLib::~SerialLib()
{
	
	if (serialPtr != nullptr)
	{
		serialPtr->close();
		delete serialPtr;
	}
}

void SerialLib::run()
{
}

bool SerialLib::initSerial(const char* port, int baudrate)
{
	serialPtr = new serial::Serial(port, baudrate, serial::Timeout::simpleTimeout(1000));

	return serialPtr->isOpen();
}

void SerialLib::setCallBackFunction(retFunction interruptFunction)
{
}

int SerialLib::sendBytes(const unsigned char* bytesToSend, uint8_t size)
{
	return serialPtr->write(bytesToSend, size);
}

bool SerialLib::sendByte(uint8_t data)
{
	return false;
}
