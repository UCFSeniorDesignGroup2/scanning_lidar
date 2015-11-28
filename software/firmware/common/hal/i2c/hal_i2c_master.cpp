#include "hal_i2c_master.h"


using namespace HAL;

I2C_MASTER::~I2C_MASTER()
{

}


void I2C_MASTER::TakeControl()
{
	mMutex.Lock();
}

void I2C_MASTER::ReleaseControl()
{
	mMutex.Unlock();
}
