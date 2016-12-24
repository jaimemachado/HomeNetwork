#include "UARTProtocol.h"

#include "SerialHW.h"


static void uartInterruptFunc(uint8_t byte) 
{
	if(uartInstance != 0)
		uartInstance->pushByteInRXBuffer(byte);
}

UARTProtocol::UARTProtocol(UARTProtocolBaudRate baudRate) 
{
	flagsError_ = 0;
	baudrate_ = baudRate;
	bufferRX_ = new FIFO(20);
	serial_ = new SerialHW();
}


bool UARTProtocol::byteAvailable()
{
	if (serial_)
	{
		return true;
	}
	return false;
}

bool UARTProtocol::getByte(uint8_t& data)
{
	bool ret = byteAvailable();
	if (ret) {

	}
	return ret;
}

UARTProtocol::~UARTProtocol()
{
	delete serial_;
	delete bufferRX_;
}

void UARTProtocol::init()
{
	uartInstance = this;
	serial_->setCallBackFunction(uartInterruptFunc);
	serial_->setBaudRate(baudrate_);
	serial_->initSerial();
}

void UARTProtocol::run()
{
	serial_->run();

	while (byteAvailable()) {
		uint8_t data;
	    getByte(data);
	}
}

void UARTProtocol::SendData(const char* string) 
{
	
}

void UARTProtocol::pushByteInRXBuffer(uint8_t byte)
{
	if(!bufferRX_->pushByte(byte))
	{
		if(bufferRX_->getErrors() == FIFO_BUFFEROVERFLOW_ERROR)
		{
			flagsError_ |= UARTPROTOCOL_INBUFFEROVERFLOW_ERROR;
		}
	}
}

