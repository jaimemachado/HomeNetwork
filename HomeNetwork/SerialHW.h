#pragma once
#include "ISerial.h"
#include "FIFO.h"

class SerialHW : public ISerial
{
		int baudRate;
		FIFO *fifo;
	public:
		SerialHW();
		virtual ~SerialHW();
		void setCallBackFunction(retFunction interruptFunction) override;
		int sendBytes(const unsigned char* bytesToSend, uint8_t size) override;
		bool initSerial(const char* port, int baudrate) override;
		void run() override;
		bool sendByte(unsigned char data) override;
	
};

