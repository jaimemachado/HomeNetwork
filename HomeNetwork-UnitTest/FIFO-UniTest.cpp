#include "gtest/gtest.h"

#include "../HomeNetwork/FIFO.h"

class FifoTest : public ::testing::Test{
protected:
	static FIFO* shared_resource_;
	// Per-test-case set-up.
	// Called before the first test in this test case.
	// Can be omitted if not needed.
	static void SetUpTestCase() {
		shared_resource_ = new FIFO(10);
	}

	// Per-test-case tear-down.
	// Called after the last test in this test case.
	// Can be omitted if not needed.
	static void TearDownTestCase() {
		delete shared_resource_;
		shared_resource_ = NULL;
	}

	// You can define per-test set-up and tear-down logic as usual.
	virtual void SetUp()
	{
		shared_resource_->clearFIFO();
	}
	

	// Some expensive resource shared by all tests.
	
};

FIFO* FifoTest::shared_resource_ = NULL;

TEST_F(FifoTest, testPushByte)
{
	ASSERT_TRUE(shared_resource_->pushByte(0));
	ASSERT_TRUE(shared_resource_->pushByte(1));
	ASSERT_TRUE(shared_resource_->pushByte(2));
	ASSERT_TRUE(shared_resource_->pushByte(3));
	ASSERT_TRUE(shared_resource_->pushByte(4));
	ASSERT_TRUE(shared_resource_->pushByte(5));
	ASSERT_TRUE(shared_resource_->pushByte(6));
	ASSERT_TRUE(shared_resource_->pushByte(7));
	ASSERT_TRUE(shared_resource_->pushByte(8));
	ASSERT_TRUE(shared_resource_->pushByte(9));

	ASSERT_EQ(0, shared_resource_->getErrors());
}

TEST_F(FifoTest, testPeakByteEmptyFIFO)
{
	uint8_t data;
	ASSERT_FALSE(shared_resource_->peakByte(data));
	ASSERT_FALSE(shared_resource_->peakByte(data));
	ASSERT_FALSE(shared_resource_->peakByte(data));
	ASSERT_FALSE(shared_resource_->peakByte(data));

	ASSERT_EQ(0, shared_resource_->getErrors());
}

TEST_F(FifoTest, testPeakByte)
{
	uint8_t excpected = 20;
	uint8_t actualPeak;
	uint8_t actualPeak2;
	uint8_t actualpop;
	ASSERT_TRUE(shared_resource_->pushByte(excpected));
	ASSERT_TRUE(shared_resource_->peakByte(actualPeak));
	ASSERT_TRUE(shared_resource_->popByte(actualpop));
	ASSERT_FALSE(shared_resource_->peakByte(actualPeak2));

	ASSERT_EQ(excpected, actualPeak);
	ASSERT_EQ(excpected, actualpop);

	ASSERT_EQ(0, shared_resource_->getErrors());
}

TEST_F(FifoTest, testPopByteEmptyFIFO)
{
	uint8_t data;
	ASSERT_FALSE(shared_resource_->popByte(data));
	ASSERT_FALSE(shared_resource_->popByte(data));
	ASSERT_FALSE(shared_resource_->popByte(data));
	ASSERT_FALSE(shared_resource_->popByte(data));

	ASSERT_EQ(0, shared_resource_->getErrors());
}

TEST_F(FifoTest, testPushPop1Byte)
{
	uint8_t expectedData = 5;
	uint8_t actualData = 0;
	ASSERT_TRUE(shared_resource_->pushByte(expectedData));
	ASSERT_TRUE(shared_resource_->popByte(actualData));

	ASSERT_EQ(expectedData, actualData);

	ASSERT_EQ(0, shared_resource_->getErrors());
}

TEST_F(FifoTest, testPushPop10Bytes)
{
	uint8_t expectedData[20] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
	uint8_t actualData[20];
	int expectedIndex = 0;
	int actualIndex = 0;

	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 0
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 1
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 2
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 3
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 4
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 5
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 6
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 7
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 8
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 9
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 0
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 1
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 2
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 3
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 4
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 5
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 6
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 7
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 10
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 11
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 12
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 13
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 14
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 15
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 8
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 9
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 10
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 11
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 12
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 13
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 16
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 17
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 18
	ASSERT_TRUE(shared_resource_->pushByte(expectedData[expectedIndex++])); //in 19
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 14
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 15
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 16
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 17
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 18
	ASSERT_TRUE(shared_resource_->popByte(actualData[actualIndex++])); // out 19

    for(int x=0; x<20; x++)
    {
		ASSERT_EQ(expectedData[x], actualData[x]);
    }
	

	ASSERT_EQ(0, shared_resource_->getErrors());
}