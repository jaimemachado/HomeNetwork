#pragma once


template <class TYPE>
class IFIFO
{
public:
	virtual bool pushData(TYPE data) = 0;
	virtual bool popData(TYPE& data) = 0;
	virtual bool peakData(TYPE& data) = 0;
	virtual uint8_t getErrors() = 0;
	virtual void clearFIFO() = 0;
	virtual ~IFIFO(){};
};
