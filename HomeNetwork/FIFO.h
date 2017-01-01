#pragma once
#include <inttypes.h>
#include "IFIFO.h"

#define FIFO_BUFFEROVERFLOW_ERROR 0x01

template <class TYPE>
class FIFO : public IFIFO<TYPE>{
protected:
	uint8_t  error_;
	uint8_t  size_;
	TYPE* buffer_;

	uint8_t bufferINIndex_;
	uint8_t bufferOUTIndex_;
	uint8_t length_;

public:
	FIFO(uint8_t size)
	{
		size_ = size;
		buffer_ = (TYPE*)malloc(size_);
		bufferINIndex_ = 0;
		bufferINIndex_ = 0;
		length_ = 0;

	}
	bool pushData(TYPE data) override
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
	};
	bool popData(TYPE& data) override
	{
		if (length_ == 0)
		{
			return false;
		}
		data = buffer_[bufferOUTIndex_];

		length_--;
		bufferOUTIndex_ = bufferOUTIndex_ + 1 >= size_ ? 0 : bufferOUTIndex_ + 1;
		return true;
	};
	bool peakData(TYPE& data) override
	{
		if (length_ == 0)
		{
			return false;
		}
		data = buffer_[bufferOUTIndex_];

		return true;
	};
	uint8_t getErrors() override
	{
		return error_;
	}
	void clearFIFO() override{
		bufferOUTIndex_ = 0;
		bufferINIndex_ = 0;
		length_ = 0;
	};
	virtual ~FIFO()
	{
		free(buffer_);
	};
};