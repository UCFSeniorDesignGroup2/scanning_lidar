/*
 * range_finder.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: jack
 */

#include "range_finder.h"

#include <string.h>

using namespace Drivers;

#define LIDARLITEV2_ADDR_W 0x62
#define LIDARLITEV2_ADDR_R 0x62

RangeFinder::RangeFinder()
{
	mI2CInterface = 0;
}

void RangeFinder::SetI2CInterface(HAL::I2C_MASTER* i2c)
{
	mI2CInterface = i2c;
}

int32_t RangeFinder::GetRangeInMiliMeters()
{
	uint8_t data = 0x04;
	WriteRegister(0x00, &data, 1);
	uint8_t rx_buffer[2];
	ReadRegister(0x8f, rx_buffer, 2);
	int32_t distance = (rx_buffer[0] << 8) + rx_buffer[1];
	return distance;
}

int32_t RangeFinder::WriteRegister(uint8_t start_reg_addr, const uint8_t* data, uint32_t length)
{
	uint8_t txdata[length+1];
	txdata[0] = start_reg_addr;
	memcpy(txdata+1, data, length);
	int32_t status = mI2CInterface->WriteData(LIDARLITEV2_ADDR_W, txdata, length+1);
	return status;
}

int32_t RangeFinder::ReadRegister(uint8_t start_reg_addr, uint8_t* data, uint32_t length)
{

	int32_t status = mI2CInterface->WriteData(LIDARLITEV2_ADDR_W, &start_reg_addr, 1);
	if(status < 0)
	{
		return status;
	}
	status = mI2CInterface->ReadData(LIDARLITEV2_ADDR_R, data, length);

	return status;
}
