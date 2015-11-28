#include "i2c3_master.h"

using namespace HAL;

extern "C"
{
	#include <stm32f4xx.h>

	static int32_t HW_INIT();
	static int32_t HW_WRITE(uint8_t addr, const uint8_t* data, uint32_t length);
	static int32_t HW_READ(uint8_t addr, uint8_t* data, uint32_t length);
}

I2C3_MASTER I2C3_MASTER::mInstance;

I2C_MASTER* I2C3_MASTER::GetInstance()
{
	return &mInstance;
}

int32_t I2C3_MASTER::Initialize()
{
	return HW_INIT();
}

int32_t I2C3_MASTER::WriteData(uint8_t addr, const uint8_t* data, uint32_t length)
{
	return HW_WRITE(addr, data, length);
}

int32_t I2C3_MASTER::ReadData(uint8_t addr, uint8_t* data, uint32_t length)
{
	return HW_READ(addr, data, length);
}

extern "C"
{

	static I2C_HandleTypeDef I2CHandle;

	int32_t HW_INIT()
	{
		I2CHandle.Instance             = I2C3;
		I2CHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
		I2CHandle.Init.ClockSpeed      = 100000;
		I2CHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		I2CHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
		I2CHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		I2CHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
		I2CHandle.Init.OwnAddress1     = 0;
		I2CHandle.Init.OwnAddress2     = 0xFE;


		HAL_I2C_DeInit(&I2CHandle);
		if(HAL_I2C_Init(&I2CHandle) != HAL_OK)
		{
			return -1;
		}

		return 0;
	}

	int32_t HW_WRITE(uint8_t addr, const uint8_t* data, uint32_t length)
	{
		HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_DMA(&I2CHandle, (uint16_t)addr, (uint8_t*)data, (uint16_t)length);
		I2C3_MASTER::GetInstance()->BlockCallingThread();
		if(status == HAL_OK)
		{
			return 0;
		}
		return -1;
	}

	int32_t HW_READ(uint8_t addr, uint8_t* data, uint32_t length)
	{
		HAL_StatusTypeDef status = HAL_I2C_Master_Receive_DMA(&I2CHandle, (uint16_t)addr, (uint8_t*)data, (uint16_t)length);
		I2C3_MASTER::GetInstance()->BlockCallingThread();
		if(status == HAL_OK)
		{
			return 0;
		}
		return -1;
	}
}
