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
	uint8_t senderID;
	uint8_t command;
	uint8_t* data;
	uint8_t dataSize;
};

class UARTProtocol {

	enum UARTProtocolStages
	{
		WAIT_SYNCBYTE = 0,
		WAIT_SIZE = 1,
		WAIT_NSIZE = 2,
		WAIT_COMMAND = 3,
		WAIT_DATA = 4,
		WAIT_CRC = 5

	};

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
	FIFO* bufferRX_;
	uint8_t countLog;
	void changeToState(const UARTProtocolStages setState);

	//Protocol Info
	uint8_t dataSize;


public:
	~UARTProtocol();
	void init();
	void run();
	UARTProtocol(UARTProtocolBaudRate baudRate);

	void SendData(const char* string, uint8_t size);

	void pushByteInRXBuffer(uint8_t byte);
};

static UARTProtocol* uartInstance;

static void uartInterruptFunc(uint8_t byte);