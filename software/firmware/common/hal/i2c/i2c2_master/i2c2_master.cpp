#include "i2c2_master.h"

#include <i2c/i2c_hal_conf/i2c_hal_conf.h>

using namespace HAL;

I2C2_MASTER I2C2_MASTER::mInstance;

int32_t I2C2_MASTER::Initialize()
{
	return I2C2_Initialize_Hardware();
}

int32_t I2C2_MASTER::WriteData(uint8_t addr, const uint8_t* data, uint32_t length)
{
	return I2C2_WriteData(addr, data, length);
}

int32_t I2C2_MASTER::ReadData(uint8_t addr, uint8_t* data, uint32_t length)
{
	return I2C2_ReadData(addr, data, length);
}

I2C_MASTER* I2C2_MASTER::GetInstance()
{
	return &mInstance;
}
