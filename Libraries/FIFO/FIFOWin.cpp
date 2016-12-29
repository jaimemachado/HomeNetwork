#include "FIFOWin.h"

bool FIFOWin::pushByte(uint8_t data)
{
	queue_.push(data);
	return true;
}

bool FIFOWin::popByte(uint8_t& data)
{
	if (queue_.empty())
	{
		return false;
	}
	data = queue_.front();
	queue_.pop();
	return true;
}

bool FIFOWin::peakByte(uint8_t& data)
{
	if (queue_.empty())
	{
		return false;
	}
	data = queue_.front();
	return true;
}

uint8_t FIFOWin::getErrors()
{
	return 0;
}

void FIFOWin::clearFIFO()
{
	std::queue<uint8_t> empty;
	std::swap(queue_, empty);
}
