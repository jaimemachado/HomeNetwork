#pragma once
#include <stdint.h>
#include <queue>

#include "../../HomeNetwork/IFIFO.h"


class FIFOWin : public IFIFO
{
	std::queue<uint8_t> queue_;
public:

	bool pushByte(uint8_t data) override;
	bool popByte(uint8_t& data) override;
	bool peakByte(uint8_t& data) override;
	uint8_t getErrors() override;
	void clearFIFO() override;
};

