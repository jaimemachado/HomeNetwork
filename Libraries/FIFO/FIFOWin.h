#pragma once
#include <stdint.h>
#include <queue>

#include "../../HomeNetwork/IFIFO.h"

template <class TYPE>
class FIFOWin : public IFIFO<TYPE>
{
	std::queue<TYPE> queue_;
public:

	bool pushData(TYPE data) override
	{
		queue_.push(data);
		return true;

	}
	bool popData(TYPE& data) override
	{
		if (queue_.empty())
		{
			return false;
		}
		data = queue_.front();
		queue_.pop();
		return true;
	}
	bool peakData(TYPE& data) override
	{
		if (queue_.empty())
		{
			return false;
		}
		data = queue_.front();
		return true;
	}
	uint8_t getErrors() override
	{
		return 0;
	}
	void clearFIFO() override
	{
		std::queue<TYPE> empty;
		std::swap(queue_, empty);
	}
};

