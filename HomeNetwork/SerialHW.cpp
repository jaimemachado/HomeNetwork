#include <NeoHWSerial.h>
#include "SerialHW.h"


SerialHW::SerialHW()
{
	baudRate = -1;
	fifo = new FIFO<uint8_t>(20);
}

SerialHW::~SerialHW()
{
	NeoSerial.detachInterrupt();
	NeoSerial.end();
	delete fifo;
}

void SerialHW::setCallBackFunction(retFunction interruptFunction)
{
	NeoSerial.attachInterrupt(interruptFunction);
}

int SerialHW::sendBytes(const unsigned char* bytesToSend, uint8_t size)
{
	int ret = 0;
	for(int x=0; x < size; x++)
	{
		if(!fifo->pushData(bytesToSend[x]))
		{
			break;
		}
		ret++;
	}
	return ret;
}

bool SerialHW::initSerial(const char* port, int baudrate)
{
	baudRate = baudrate;
	NeoSerial.begin(baudRate);
	while (!NeoSerial) {}
}

void SerialHW::run()
{
	uint8_t data;
	if(NeoSerial.availableForWrite() > 0)
	{
		if (fifo->popData(data))
		{
			NeoSerial.write(data);
		}
	}
}

bool SerialHW::sendByte(unsigned char data)
{
	return fifo->pushData(data);
}
