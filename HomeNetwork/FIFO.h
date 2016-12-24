#pragma once
#include <inttypes.h>


#define FIFO_BUFFEROVERFLOW_ERROR 0x01

class FIFO {
protected:
	uint8_t  error_;
	uint8_t  size_;
	uint8_t* buffer_;

	uint8_t bufferINIndex_;
	uint8_t bufferOUTIndex_;
	uint8_t length_;

public:
	FIFO(uint8_t size);
	bool pushByte(uint8_t data);
	bool popByte(uint8_t& data);
	bool peakByte(uint8_t& data);
	uint8_t getErrors();
	void clearFIFO();
	virtual ~FIFO();
};