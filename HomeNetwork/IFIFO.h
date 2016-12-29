#pragma once

class IFIFO
{
public:
	virtual bool pushByte(uint8_t data) = 0;
	virtual bool popByte(uint8_t& data) = 0;
	virtual bool peakByte(uint8_t& data) = 0;
	virtual uint8_t getErrors() = 0;
	virtual void clearFIFO() = 0;
	virtual ~IFIFO(){};
};
