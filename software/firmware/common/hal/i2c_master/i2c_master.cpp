#include "i2c_master.h"

using namespace HAL;

I2C_MASTER::I2C_MASTER() :
		mSemaphore(1,0)
{

}

void I2C_MASTER::BlockCallingThread()
{
	mSemaphore.Take();
}

void I2C_MASTER::UnblockCallingThread()
{
	mSemaphore.Give();
}
