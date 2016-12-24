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
		void setBaudRate(int baudRate) override;
		void setCallBackFunction(retFunction interruptFunction) override;
		void sendBytes(const char* bytesToSend, uint8_t size) override;
		bool initSerial() override;
		void run() override;
};
