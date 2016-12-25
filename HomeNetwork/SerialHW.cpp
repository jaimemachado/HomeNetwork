#include <NeoHWSerial.h>
#include "SerialHW.h"


SerialHW::SerialHW()
{
	baudRate = -1;
	fifo = new FIFO(20);
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

int SerialHW::sendBytes(const char* bytesToSend, uint8_t size)
{
	int ret = 0;
	for(int x=0; x < size; x++)
	{
		if(!fifo->pushByte(bytesToSend[x]))
		{
			break;
		}
		ret++;
	}
	return ret;
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

bool SerialHW::sendByte(uint8_t data)
{
	return fifo->pushByte(data);
}
