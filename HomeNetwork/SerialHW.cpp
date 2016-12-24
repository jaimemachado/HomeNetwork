#include <NeoHWSerial.h>
#include "SerialHW.h"

SerialHW::SerialHW()
{
	baudRate = -1;
}

SerialHW::~SerialHW()
{
	NeoSerial.detachInterrupt();
	NeoSerial.end();
	delete fifo;
}

void SerialHW::setBaudRate(int baudRate)
{
	this->baudRate = baudRate;
}

void SerialHW::setCallBackFunction(retFunction interruptFunction)
{
	NeoSerial.attachInterrupt(interruptFunction);
}

void SerialHW::sendBytes(const char* bytesToSend, uint8_t size)
{
	for(int x=0; x < size; x++)
	{
		fifo->pushByte(bytesToSend[x]);
	}
}

bool SerialHW::initSerial()
{
	NeoSerial.begin(baudRate);
	while (!NeoSerial) {}
}

void SerialHW::run()
{
	uint8_t data;
	if(NeoSerial.availableForWrite() > 0)
	{
		if (fifo->popByte(data))
		{
			NeoSerial.write(data);
		}
	}
}
