#pragma once
#include <inttypes.h>

#include "ISerial.h"
#include "FIFO.h"
#include "LoggerSoftSerial.h"
#define UARTPROTOCOL_INBUFFERSIZE 20


//PROTOCOL
#define SYNC_BYTE 0x55

//Error
#define UARTPROTOCOL_INBUFFEROVERFLOW_ERROR 0x01


enum UARTProtocolBaudRate {
	BAUD4800 = 4800,
	BAUD9600 = 9600,
	BAUD19200 = 19200,
	BAUD38400 =38400
};

struct ProtocolPackage
{
	unsigned char* data;
	uint8_t dataSize;
	ProtocolPackage()
	{
		data = nullptr;
		dataSize = 0;
	}
	~ProtocolPackage()
	{
		if(data != nullptr)
		{
			delete data;
		}
	}
};


struct CommandPackage
{
	uint8_t destID;
	uint8_t senderID;
	uint8_t command;
	const unsigned char* data;
	uint8_t dataSize;
};

class UARTProtocol {

	enum UARTProtocolStages
	{
		WAIT_SYNCBYTE = 0,
		WAIT_SIZE = 1,
		WAIT_NSIZE = 2,
		WAIT_DESTADDR = 3,
		WAIT_ORIADDR = 4,
		WAIT_COMMAND = 5,
		WAIT_DATA = 6,
		WAIT_CRC = 7

	};


	char port_[10];
	UARTProtocolStages state;
	UARTProtocolBaudRate baudrate_;
	uint8_t inBuffer_[UARTPROTOCOL_INBUFFERSIZE];
	uint8_t inBufferIndexWrite_;
	uint8_t inBufferIndexRead_;
	char flagsError_;

	ISerial* serial_;
	LoggerSoftSerial* logger_;

	bool byteAvailable();
	bool getByte(uint8_t& data);
	IFIFO* bufferRX_;
	uint8_t countLog;
	void changeToState(const UARTProtocolStages setState);

	//Protocol Info
	uint8_t dataSize;
	uint8_t localAdress_;


	uint16_t CalculateCRC16(const CommandPackage& package);

	bool CreatePackage(const CommandPackage& command, ProtocolPackage& protPackage);
	uint8_t twos_complement(uint8_t val);

public:
	~UARTProtocol();
	void init(uint8_t localAddress);
	void run();
	UARTProtocol(const char* port, UARTProtocolBaudRate baudRate);

	void SendCommand(const uint8_t& command, const uint8_t& destAddr, const unsigned char* string, const uint8_t& size);

	void pushByteInRXBuffer(uint8_t byte);
};

static UARTProtocol* uartInstance;

static void uartInterruptFunc(uint8_t byte);