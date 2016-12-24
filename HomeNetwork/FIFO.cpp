

#include <stdlib.h>
#include "FIFO.h"
 

FIFO::FIFO(uint8_t size)
{
	size_ = size;
	buffer_ = (uint8_t*)malloc(size_);
	bufferINIndex_ = 0;
	bufferINIndex_ = 0;
	length_ = 0;
}


bool FIFO::peakByte(uint8_t& data)
{
	if (length_ == 0)
	{
		return false;
	}
	data = buffer_[bufferOUTIndex_];

	return true;
}
bool FIFO::pushByte(uint8_t data)
{
	if (length_ + 1 > size_)
	{
		error_ |= FIFO_BUFFEROVERFLOW_ERROR;
		return false;
	}
	length_++;
	buffer_[bufferINIndex_] = data;

	bufferINIndex_ = bufferINIndex_ + 1 >= size_ ? 0 : bufferINIndex_ + 1;

	return true;
}

bool FIFO::popByte(uint8_t & data)
{
	if (length_ == 0)
	{
		return false;
	}
	data = buffer_[bufferOUTIndex_];

	length_--;
	bufferOUTIndex_ = bufferOUTIndex_ + 1 >= size_ ? 0 : bufferOUTIndex_ + 1;
	return true;
}

uint8_t FIFO::getErrors()
{
	return error_;
}

void FIFO::clearFIFO()
{
	bufferOUTIndex_ = 0;
	bufferINIndex_ = 0;
	length_ = 0;
}

FIFO::~FIFO()
{
	free(buffer_);
}
