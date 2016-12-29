#pragma once

#include "../../HomeNetwork/ISerial.h"
#include "serial/serial.h"

class SerialLib : public ISerial
{
	serial::Serial* serialPtr;

public:
	SerialLib();
	virtual ~SerialLib();

	void run() override;
	bool initSerial(const char* port, int baudrate) override;
	void setCallBackFunction(retFunction interruptFunction) override;
	int sendBytes(const unsigned char* bytesToSend, uint8_t size) override;
	bool sendByte(unsigned char data) override;
};
