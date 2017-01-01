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
	bufferRX_ = new FIFO<uint8_t>(20);
	serial_ = new SerialHW();
	logger_ = LoggerSoftSerial::getInstance();
	receivedCommand = new FIFO<CommandPackage*>(3);
#else
	bufferRX_ = new FIFOWin<uint8_t>();
	serial_ = new SerialLib();
	receivedCommand = new FIFOWin<CommandPackage*>();
#endif
	logPrintf0(logger_, "Change from state");

	countLog = 0;
}


bool UARTProtocol::dataAvailable()
{
	uint8_t data;
	if (bufferRX_->peakData(data))
	{
		return true;
	}
	return false;
}

bool UARTProtocol::getData(uint8_t& data)
{
	bool ret = dataAvailable();
	if (ret) {
		ret = bufferRX_->popData(data);
	}
	return ret;
}

void UARTProtocol::changeToState(const UARTProtocolStages setState)
{
	logPrintf3(logger_, "Change from state: %d -> %d - IgnoringData: %d", (int)state, (int)setState, ignorePackage_);
	state = setState;
}

UARTProtocol::~UARTProtocol()
{
	CommandPackage* tempcmd;
	while (receivedCommand->popData(tempcmd))
	{
		free(tempcmd->data);
		free(tempcmd);
	}
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
	while (dataAvailable()) {
		uint8_t data;
	    getData(data);

		switch (state)
		{
			case WAIT_SYNCBYTE: 
				if(data == SYNC_BYTE)
				{
					clearStateMachineVars(false);
					changeToState(WAIT_SIZE);
				}
				break;
			case WAIT_SIZE: 
				receiveCommandTemp = new CommandPackage();
				receiveCommandTemp->dataSize = data;
				logPrintf1(logger_, "Data Size: %d", receiveCommandTemp->dataSize);
				changeToState(WAIT_NSIZE);
				break;
			case WAIT_NSIZE: 
				if(twos_complement(receiveCommandTemp->dataSize) == data)
				{
					changeToState(WAIT_DESTADDR);
				}else
				{
					clearStateMachineVars(true);
					changeToState(WAIT_DESTADDR);
				}
				
				break;
			case WAIT_DESTADDR: 
				logPrintf2(logger_, "Pachakge Address:%d  Local Address:%d", localAdress_, data);
				if(data == localAdress_ && !ignorePackage_)
				{
					receiveCommandTemp->destID = data;
					if(receiveCommandTemp->dataSize > 0)
					{
						receiveCommandTemp->data = (unsigned char*)malloc((receiveCommandTemp->dataSize)*sizeof(unsigned char));
						logPrintf0(logger_, "Data Allocated");
					}
				}
				else
				{
					logPrintf0(logger_, "Ignoring Package diferent Dest Address");
					clearStateMachineVars(true);
				}
				changeToState(WAIT_ORIADDR);
				break;
			case WAIT_ORIADDR: 
				if(!ignorePackage_)
				{
					receiveCommandTemp->senderID = data;
				}
				changeToState(WAIT_COMMAND);
				break;
			case WAIT_COMMAND: 
				if (!ignorePackage_)
				{
					receiveCommandTemp->command = data;
				}
				if(receiveCommandTemp->dataSize > 0)
				{
					changeToState(WAIT_DATA);
				}else
				{
					changeToState(WAIT_CRC1);
				}
				
				break;
			case WAIT_DATA:
				receiveCommandTemp->data[dataSizeTemp] = data;
				logPrintf2(logger_, "Data[%d] : %d", dataSizeTemp, data);
				dataSizeTemp++;
				if(dataSizeTemp == receiveCommandTemp->dataSize)
				{
					changeToState(WAIT_CRC1);
				}
				break;
			case WAIT_CRC1: 
				if(!ignorePackage_)
				{
					crcPackage = CalculateCRC16(*receiveCommandTemp);
					uint8_t crc1 = ((crcPackage >> 8) & 0x00FF);
					logPrintf2(logger_, "CRC1:%d  Data:%d", crc1, data);
					if (crc1 != data)
					{
						clearStateMachineVars(true);
					}
				}
				changeToState(WAIT_CRC2);
				break;
			case WAIT_CRC2:
				uint8_t crc2;
				crc2 = data & 0x00FF;
				logPrintf2(logger_, "CRC2:%d  Data:%d", crc2, data);
				if (!ignorePackage_ && crc2 == data)
				{
					receivedCommand->pushData(receiveCommandTemp);
					receiveCommandTemp = nullptr;
					logPrintf0(logger_, "Package Available");
				}
				clearStateMachineVars(true);
				changeToState(WAIT_SYNCBYTE);
				break;
			default: 
				clearStateMachineVars(true);
				changeToState(WAIT_SYNCBYTE);
				break;

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
	protPackage.data = (unsigned char*)(malloc(protPackage.dataSize));

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

CommandPackage* UARTProtocol::getReceivedPackage()
{
	CommandPackage* ret = nullptr;
	if(receivedCommand->peakData(ret))
	{
		receivedCommand->popData(ret);
		return  ret;
	}
	return ret;
}

uint8_t UARTProtocol::twos_complement(uint8_t val)
{
	return -(unsigned int)val;
}

void UARTProtocol::clearStateMachineVars(bool ignorePackage)
{
	if(receiveCommandTemp != nullptr)
	{
		if(receiveCommandTemp->data != nullptr)
		{
			free(receiveCommandTemp->data);
		}
		free(receiveCommandTemp);
	}
	dataSizeTemp = 0;
	crcPackage = 0;
	receiveCommandTemp = nullptr;
	ignorePackage_ = ignorePackage;
}

void UARTProtocol::SendCommand(const uint8_t& command, const uint8_t& destAddr, const unsigned char* string, const uint8_t& size)
{
	CommandPackage cmdPackage;
	ProtocolPackage protPackage;
	cmdPackage.senderID = localAdress_;
	cmdPackage.destID = destAddr;
	cmdPackage.command = command;
	cmdPackage.dataSize = size;
	cmdPackage.data = (unsigned char*)(malloc(size * sizeof(unsigned char)));
	memcpy(cmdPackage.data, string, size);
	
	bool packageOK = CreatePackage(cmdPackage, protPackage);
	int bytesSent = serial_->sendBytes(protPackage.data, protPackage.dataSize);
}

void UARTProtocol::pushByteInRXBuffer(uint8_t byte)
{
	if(!bufferRX_->pushData(byte))
	{
		if(bufferRX_->getErrors() == FIFO_BUFFEROVERFLOW_ERROR)
		{
			flagsError_ |= UARTPROTOCOL_INBUFFEROVERFLOW_ERROR;
		}
	}
}

