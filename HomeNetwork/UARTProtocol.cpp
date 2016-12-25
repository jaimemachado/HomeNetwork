#include "UARTProtocol.h"

#include "SerialHW.h"
#include <arduino.h>

#include <util/crc16.h>




static void uartInterruptFunc(uint8_t byte) 
{
	if(uartInstance != 0)
	{
		uartInstance->pushByteInRXBuffer(byte);
	}
}

UARTProtocol::UARTProtocol(UARTProtocolBaudRate baudRate) 
{

	state = WAIT_SYNCBYTE;
	flagsError_ = 0;
	baudrate_ = baudRate;
	bufferRX_ = new FIFO(20);
	serial_ = new SerialHW();
	logger_ = LoggerSoftSerial::getInstance();

	logger_->log("Teste");
	logPrintf0(logger_, "Change from state");

	countLog = 0;
}


bool UARTProtocol::byteAvailable()
{
	uint8_t data;
	if (bufferRX_->peakByte(data))
	{
		return true;
	}
	return false;
}

bool UARTProtocol::getByte(uint8_t& data)
{
	bool ret = byteAvailable();
	if (ret) {
		ret = bufferRX_->popByte(data);
	}
	return ret;
}

void UARTProtocol::changeToState(const UARTProtocolStages setState)
{
	logPrintf2(logger_, "Change from state: %d -> %d", (int)state, (int)setState);
	state = setState;
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
	
	if(countLog < 10)
	{
		logger_->log("Teste3");
		countLog++;
	}

	while (byteAvailable()) {
		uint8_t data;
	    getByte(data);

		switch (state)
		{
			case WAIT_SYNCBYTE: 
				if(data == SYNC_BYTE)
				{
					changeToState(WAIT_SIZE);
				}
				break;
			case WAIT_SIZE: 
				dataSize = data;
				changeToState(WAIT_NSIZE);
				break;
			case WAIT_NSIZE: 
				if(dataSize == !data)
				{
					changeToState(WAIT_COMMAND);
				}else
				{
					dataSize = 0;
					changeToState(WAIT_SYNCBYTE);
				}
				
				break;
			case WAIT_COMMAND: 
				break;
			case WAIT_DATA: 
				break;
			case WAIT_CRC: 
				break;
			default: break;
		}


		// logPrintf(logger_, "Byte Received %c", data);
		//logger_->log("Byte Received");
	}
}

void UARTProtocol::SendData(const char* string, uint8_t size) 
{
	int bytesSent = serial_->sendBytes(string, size);
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

