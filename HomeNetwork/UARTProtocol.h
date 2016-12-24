#pragma once
#include <inttypes.h>

#include "ISerial.h"
#include "FIFO.h"
#define UARTPROTOCOL_INBUFFERSIZE 20


//Error
#define UARTPROTOCOL_INBUFFEROVERFLOW_ERROR 0x01


enum UARTProtocolBaudRate {
	BAUD4800 = 4800,
	BAUD9600 = 9600,
	BAUD19200 = 19200,
	BAUD38400 =38400
};


class UARTProtocol {
private:
	UARTProtocolBaudRate baudrate_;
	uint8_t inBuffer_[UARTPROTOCOL_INBUFFERSIZE];
	uint8_t inBufferIndexWrite_;
	uint8_t inBufferIndexRead_;
	char flagsError_;

	ISerial* serial_;

	bool byteAvailable();
	bool getByte(uint8_t& data);
	FIFO* bufferRX_;

public:
	~UARTProtocol();
	void init();
	void run();
	UARTProtocol(UARTProtocolBaudRate baudRate);

	void SendData(const char* string);

	void pushByteInRXBuffer(uint8_t byte);
};

static UARTProtocol* uartInstance;

static void uartInterruptFunc(uint8_t byte);