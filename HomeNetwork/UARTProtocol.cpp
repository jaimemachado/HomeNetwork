#include "UARTProtocol.h"



#ifndef WIN32_

#include <arduino.h>

#include <util/crc16.h>
#include "SerialHW.h"
#else

#include "../Libraries/SerialLibX86/SerialLib.h"
#include "../Libraries/FIFO/FIFOWin.h"
#endif




static void uartInterruptFunc(uint8_t byte) 
{
	if(uartInstance != 0)
	{
		uartInstance->pushByteInRXBuffer(byte);
	}
}

UARTProtocol::UARTProtocol(const char* port, UARTProtocolBaudRate baudRate)
{

	state = WAIT_SYNCBYTE;
	flagsError_ = 0;
	baudrate_ = baudRate;
	if(strlen(port) > 10)
	{
		logPrintf0(logger_, "Error port name too long - Using default COM1");
		strcpy(port_, "COM1");
	}else
	{
		strcpy(port_, port);
	}

	


#ifndef WIN32_
	bufferRX_ = new FIFO(20);
	serial_ = new SerialHW();
	logger_ = LoggerSoftSerial::getInstance();
#else
	bufferRX_ = new FIFOWin();
	serial_ = new SerialLib();

#endif
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

void UARTProtocol::init(uint8_t localAddress)
{
#ifndef WIN32_
	while (countLog < 10)
	{
		logger_->log("Teste3");
		countLog++;
	}
#endif
	uartInstance = this;
	serial_->setCallBackFunction(uartInterruptFunc);
	serial_->initSerial(port_, baudrate_);
	localAdress_ = localAddress;

	logPrintf1(logger_, "Init protocol on address: %d", localAddress);
}

void UARTProtocol::run()
{
	serial_->run();
	
	
	//Receive Machine
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
				if(twos_complement(dataSize) == data)
				{
					changeToState(WAIT_COMMAND);
				}else
				{
					dataSize = 0;
					changeToState(WAIT_SYNCBYTE);
				}
				
				break;
			case WAIT_DESTADDR: 
				break;
			case WAIT_ORIADDR: 
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

uint16_t UARTProtocol::CalculateCRC16(const CommandPackage& package)
{
	return 0x1234;
}

bool UARTProtocol::CreatePackage(const CommandPackage& command, ProtocolPackage& protPackage)
{

	protPackage.dataSize = command.dataSize + 8;
	protPackage.data = static_cast<unsigned char*>(malloc(protPackage.dataSize));

	protPackage.data[0] = SYNC_BYTE;                   //SyncByte
	protPackage.data[1] = command.dataSize;			  //Data Size
	protPackage.data[2] = twos_complement(command.dataSize);			  //Data !Size
	protPackage.data[3] = command.destID;              //Destination Address
	protPackage.data[4] = command.senderID;            //Origin Address
	protPackage.data[5] = command.command;             //Command
	memcpy(&protPackage.data[6], command.data, command.dataSize);
	uint8_t crcIndex = command.dataSize + 6;

	uint16_t crc16 = CalculateCRC16(command);

	uint8_t crc16_1 = (crc16 >> 8);
	uint8_t crc16_2 = (crc16 & 0x00FF);
	protPackage.data[crcIndex] = crc16_1;
	protPackage.data[crcIndex+1] = crc16_2;

	return true;
}

uint8_t UARTProtocol::twos_complement(uint8_t val)
{
	return -(unsigned int)val;
}


void UARTProtocol::SendCommand(const uint8_t& command, const uint8_t& destAddr, const unsigned char* string, const uint8_t& size)
{
	CommandPackage cmdPackage;
	ProtocolPackage protPackage;
	cmdPackage.senderID = localAdress_;
	cmdPackage.destID = destAddr;
	cmdPackage.command = command;
	cmdPackage.dataSize = size;
	cmdPackage.data = string;
	
	bool packageOK = CreatePackage(cmdPackage, protPackage);
	int bytesSent = serial_->sendBytes(protPackage.data, protPackage.dataSize);
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

