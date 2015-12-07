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
	mExternalClock = 0;
	mEnablePin = 0;
	mIntBPin = 0;
	mStartPin = 0;
	mSPIInterface = 0;
	mThreshold = 0;
	mLaserPulse = 0;

	mInitialized = false;
}

void RangeFinder::SetTDCExternalClock(HAL::ExternalClock* ext_clk)
{
	mExternalClock = ext_clk;
}

void RangeFinder::SetTDCEnablePin(HAL::HAL_GPIO_PIN* gpio)
{
	mEnablePin = gpio;
}

void RangeFinder::SetTDCIntBPin(HAL::HAL_GPIO_PIN* gpio)
{
	mIntBPin = gpio;
}

void RangeFinder::SetTDCStartPin(HAL::SyncPulse* sync_pin)
{
	mStartPin = sync_pin;
}

void RangeFinder::SetTDCSPIInterface(HAL::HAL_SPI* spi_interface)
{
	mSPIInterface = spi_interface;
}

void RangeFinder::SetThresholdPin(HAL::AnalogOutput* analog_out)
{
	mThreshold = analog_out;
}

void RangeFinder::SetLaserFirePin(HAL::FastPulse* fast_pulse)
{
	mLaserPulse = fast_pulse;
}

int32_t RangeFinder::Initialize()
{
	mEnablePin->SetPinState(HAL::HAL_GPIO_PIN::LOW);
	OAL::Thread::Sleep(100); // 10ms
	mEnablePin->SetPinState(HAL::HAL_GPIO_PIN::HIGH);
	OAL::Thread::Sleep(100); // 10ms

	if(mExternalClock == 0 || mEnablePin == 0 || mIntBPin == 0 ||
			mStartPin == 0 || mSPIInterface == 0 || mThreshold == 0 || mLaserPulse == 0)
	{
		return -1;
	}

	mInitialized = true;
	return 0;
}

void RangeFinder::ConfigAveraging(uint32_t avg)
{

}

void RangeFinder::ConfigTofMode(uint8_t mode)
{

}

int32_t RangeFinder::GetRangeInMiliMeters()
{
	return 100;
}

int32_t RangeFinder::WriteRegister(uint8_t addr, const uint8_t* data, uint32_t length)
{
	uint8_t tx_data[(const uint32_t)length+1];
	uint8_t rx_data[(const uint32_t)length+1];

	// set write bit
	if(length == 1) // auto increment off
	{
		tx_data[0] = (addr | 0x40);
	}
	else if(length > 1) // auto increment on
	{
		tx_data[0] = (addr | 0xC0);
	}

	memcpy(tx_data+1, data, length);
	memset(rx_data, 0, length+1);
	return mSPIInterface->TransferData(tx_data,rx_data,length+1);

}

int32_t RangeFinder::ReadRegister(uint8_t addr, uint8_t* data, uint32_t length)
{
	uint8_t tx_data[(const uint32_t)length+1];
	uint8_t rx_data[(const uint32_t)length+1];

	// set read bit
	if(length == 1) // auto increment off
	{
		tx_data[0] = (addr | 0x00);
	}
	else if(length > 1) // auto increment on
	{
		tx_data[0] = (addr | 0x80);
	}

	memset(tx_data, 0, length+1);
	memcpy(rx_data+1, data, length);

	return mSPIInterface->TransferData(tx_data, rx_data, length+1);
}
