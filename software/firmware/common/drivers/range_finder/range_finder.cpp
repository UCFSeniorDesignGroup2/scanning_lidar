/*
 * range_finder.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: jack
 */

#include "range_finder.h"
#include "range_finder_conf.h"

#include <stdlib.h>


#include <string.h>

using namespace Drivers;

#define HARDWARE_AVERAGING


RangeFinder::RangeFinder() :
		mSemaphore(1,0)
{
	mExternalClock = 0;
	mEnablePin = 0;
	mResetPin = 0;
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

void RangeFinder::SetPeakResetPin(HAL::HAL_GPIO_PIN* gpio)
{
	mResetPin = gpio;
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
	if(mExternalClock == 0 || mEnablePin == 0 || mIntBPin == 0 || mResetPin == 0 ||
			mStartPin == 0 || mSPIInterface == 0 || mThreshold == 0 || mLaserPulse == 0)
	{
		return -1;
	}

	mEnablePin->SetPinState(HAL::HAL_GPIO_PIN::LOW);
	OAL::Thread::Sleep(100); // 10ms
	mEnablePin->SetPinState(HAL::HAL_GPIO_PIN::HIGH);
	OAL::Thread::Sleep(100); // 10ms

	mInitialized = true;
	return 0;
}

void RangeFinder::ConfigAveraging(uint32_t avg)
{

}

void RangeFinder::ConfigTofMode(uint8_t mode)
{

}

int32_t RangeFinder::CalcTOF()
{

	uint8_t CONFIG1_REG = 0x01;
	WriteRegister(0x00, &CONFIG1_REG, 1);


#ifndef HARDWARE_AVERAGING
	mStartPin->SetState(1); // start the tdc counter
	mStartPin->SetState(0); // reset the start pin
#else
//	mResetPin->SetPinState(HAL::HAL_GPIO_PIN::HIGH);
//	OAL::Thread::Sleep(100);
//	mResetPin->SetPinState(HAL::HAL_GPIO_PIN::LOW);
	uint8_t CONFIG2_REG = (3<<3); // set to 8 averaging cycles
	WriteRegister(0x01, &CONFIG2_REG, 1);
	for(int i = 0; i < 8; i++)
	{
		mStartPin->SetState(1); // start the tdc counter
		mStartPin->SetState(0); // reset the start pin
		OAL::Thread::Sleep(1);
	}
#endif

	if(mSemaphore.TakeTimeout(100) < 0) // wait for interrupt from intb pin
	{
		return -1;
	}

	uint8_t INT_STATUS_REG = 0x00;
	ReadRegister(0x02, &INT_STATUS_REG, 1);  // read the int status reg

	int32_t status = 0; // return status

	// check clock overflow bit
	if((INT_STATUS_REG & 0x02) == 0x02) // overflow detected
	{
		status = -1; // pulse not received
	}
	else if((INT_STATUS_REG & 0x01) == 0x01) // new measurement detected
	{

		uint8_t TIME1[3];
		ReadRegister(0x10, TIME1, 3); // read the TIME1 register
		uint8_t CALIBRATION1[3];
		ReadRegister(0x1b, CALIBRATION1, 3);
		uint8_t CALIBRATION2[3];
		ReadRegister(0x1c, CALIBRATION2, 3);

		uint32_t TIME1_REG = (TIME1[0] << 16) | (TIME1[1] << 8) | (TIME1[2] << 0);
		uint32_t CALIBRATION1_REG = (CALIBRATION1[0] << 16) | (CALIBRATION1[1] << 8) | (CALIBRATION1[0] << 0);
		uint32_t CALIBRATION2_REG = (CALIBRATION2[0] << 16) | (CALIBRATION2[1] << 8) | (CALIBRATION2[0] << 0);

//		uint64_t calCount = (CALIBRATION2_REG - CALIBRATION1_REG)/9; // calCount for 10 calibration periods
//		uint64_t normLSBINV = calCount * 16; // 16MHz
//		uint64_t tof = TIME1_REG * ((uint64_t)1000/normLSBINV); // tof in ps

		uint64_t tmp1 = (uint64_t)((uint64_t)TIME1_REG * (uint64_t)1000000*9);
		uint64_t tmp2 = (16*(CALIBRATION2_REG - CALIBRATION1_REG));
		uint64_t tof = tmp1/tmp2;

		INT_STATUS_REG = 0x0F;
		WriteRegister(0x02, &INT_STATUS_REG, 1); // clear status reg by writing 1's

#ifndef HARDWARE_AVERAGING
		status = (int32_t)tof;
#else
		status = (int32_t)tof / 8;
#endif
	}


#ifndef HARDWARE_AVERAGING
	OAL::Thread::Sleep(1);
#endif

	return status;
}

int32_t RangeFinder::GetRangeInMiliMeters(int32_t& section)
{

	int32_t tof = 0;
	int32_t count = 0;
	for(int i = 0; i < 1; i++)
	{
		int32_t tmp = CalcTOF();

		if(tmp > 0)
		{
			count++;
			tof += tmp;
		}
	}
	if(count == 0 || tof < 0)
	{
		return -1;
	}
	tof /= count;

	int32_t tof_closest = -1;
	for(int i = 1; i < LUT_TABLE_SIZE; i++)
	{
		if(tof < lut_table[i].tof && tof > lut_table[i-1].tof)
		{
			tof_closest = i;
		}
	}

	if(tof_closest == -1)
	{
		return -1;
	}

	int32_t tof_diff = abs(lut_table[tof_closest].tof - lut_table[tof_closest - 1].tof);
	int32_t dist_diff = abs(lut_table[tof_closest].distance - lut_table[tof_closest-1].distance);

	int32_t mes_tof_diff = abs(tof - lut_table[tof_closest-1].tof);
	float percent = (float)mes_tof_diff / (float)tof_diff;
	int32_t distance = lut_table[tof_closest-1].distance + dist_diff * percent;

	section = tof_closest;
	//	return distance*3.28084/1000*12;
//	return distance*3.28084/1000*12; // convert mm to inches
	return distance;
}

void RangeFinder::HandleIntBISR()
{
	mSemaphore.Give();
}


int32_t RangeFinder::WriteRegister(uint8_t addr, const uint8_t* data, uint32_t length)
{
	uint8_t tx_data[(const uint32_t)length+1];
	uint8_t rx_data[(const uint32_t)length+1];

	// set write bit
	tx_data[0] = (addr | 0x40);

	memcpy(tx_data+1, data, length);
	memset(rx_data, 0, length+1);
	return mSPIInterface->TransferData(tx_data,rx_data,length+1);

}

int32_t RangeFinder::ReadRegister(uint8_t addr, uint8_t* data, uint32_t length)
{
	uint8_t tx_data[(const uint32_t)length+1];
	uint8_t rx_data[(const uint32_t)length+1];

	memset(tx_data, 0, length+1);
	memset(rx_data, 0, length+1);
	tx_data[0] = addr;

	int32_t ret = mSPIInterface->TransferData(tx_data, rx_data, length+1);

	memcpy(data, rx_data+1, length);
	return ret;
}
