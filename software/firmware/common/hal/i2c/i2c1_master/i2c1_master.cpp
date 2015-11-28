#include "i2c1_master.h"

#include <i2c/i2c_hal_conf/i2c_hal_conf.h>

using namespace HAL;


I2C1_MASTER I2C1_MASTER::mInstance;



int32_t I2C1_MASTER::Initialize()
{
	return I2C1_Initialize_Hardware();
}


int32_t I2C1_MASTER::WriteData(uint8_t addr, const uint8_t* data, uint32_t length)
{
	return I2C1_WriteData(addr, data, length);
}

int32_t I2C1_MASTER::ReadData(uint8_t addr, uint8_t* data, uint32_t length)
{
	return I2C1_ReadData(addr, data, length);
}

I2C_MASTER* I2C1_MASTER::GetInstance()
{
	return &mInstance;
}
