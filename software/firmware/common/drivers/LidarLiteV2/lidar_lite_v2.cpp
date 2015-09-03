// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// lidar_lite_v2.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#include "lidar_lite_v2.h"

#include "oal/oal_common.h"

#include <stddef.h>

using namespace Drivers;

/*
 * Constructor:
 * Parameters:
 * 1. (uint8_t)i2c_address
 *
 * Setup default values and set i2c address
 */
LidarLiteV2::LidarLiteV2(uint8_t i2c_address)
{
	mI2CAddress = i2c_address;
	mI2CInterface = NULL;
	mGPIOInterface = NULL;
	mConfiguration = NULL;
}

/*
 * Destructor:
 * Parameters:
 * (none).
 *
 * Doesn't do anything
 */
LidarLiteV2::~LidarLiteV2()
{

}

/*
 * Initialize:
 * Parameters:
 * (none).
 *
 * Powers on the LidarLiteV2 device, then runs an optional configuration routine.
 *
 * Returns:
 * -1 if failure. 0 if success.
 */
int LidarLiteV2::Initialize()
{
	if(mGPIOInterface != NULL)
	{
		// reset the device
		mGPIOInterface->TakeControl();
		mGPIOInterface->SetPinState(HAL::HAL_GPIO_PIN::PIN_STATE::LOW);
		OAL::Thread::Sleep(100);
		mGPIOInterface->SetPinState(HAL::HAL_GPIO_PIN::PIN_STATE::HIGH);
		mGPIOInterface->ReleaseControl();
	}
	else
	{
		return -1;
	}

	// configure setup registers
	if(mConfiguration != NULL)
	{
		return mConfiguration(this);
	}

	return 0;
}

/*
 * SetI2CInterface:
 * Parameters:
 * 1. (HAL_I2C*)i2c_interface
 *
 * Gives the LidarLiteV2 Driver access to an i2c interface
 * so that it can communicate with the device.
 *
 * Returns:
 * (void)
 */
void LidarLiteV2::SetI2CInterface(HAL::HAL_I2C* interface)
{
	mI2CInterface = interface;
}

/*
 * SetGPIOInterface:
 * Parameters:
 * 1. (HAL_GPIO_PIN*)gpio_interface
 *
 * Gives the LidarLiteV2 Driver access to a gpio interface so that
 * it can power on the device.
 *
 * Returns:
 * (void)
 */
void LidarLiteV2::SetGPIOInterface(HAL::HAL_GPIO_PIN* interface)
{
	mGPIOInterface = interface;
}

/*
 * SetConfiguration:
 * Parameters:
 * 1. (int (*)(LidarLiteV2*))configuration
 *
 * Provides a way to set a configuration routine for the LidarLiteV2 driver so that
 * it is easier to manage different configurations for the device.
 *
 * Returns:
 * (void)
 */
void LidarLiteV2::SetConfiguration(int (*config)(LidarLiteV2*))
{
	mConfiguration = config;
}

/*
 * SetRegister:
 * Parameters:
 * 1. (REGISTER) reg, 2. (const uint8_t*) data, 3. (uint32_t) reg_size
 *
 * Sets the value of a LidarLiteV2 register.
 *
 * Returns:
 * -1 if failure. 0 if success.
 */
int LidarLiteV2::SetRegister(
		REGISTER reg,
		const uint8_t* data,
		uint32_t size)
{
	if(mI2CInterface != NULL)
	{
		int ret = 0;
		mI2CInterface->TakeControl();
		mI2CInterface->TransmitData((mI2CAddress|1), (const uint8_t*)&reg, 1);
		ret = mI2CInterface->TransmitData((mI2CAddress|1), data, size);
		mI2CInterface->ReleaseControl();
		return ret; // data transfer success/failure
	}

	return -1; // no interface
}

/*
 * ReadRegister:
 * Parameters:
 * 1. (REGISTER) reg, 2. (uint8_t*) data, 3. (uint32_t) reg_size
 *
 * Reads a value from a LidarLiteV2 register.
 *
 * Returns:
 * -1 if failure. 0 if success.
 */
int LidarLiteV2::ReadRegister(
		REGISTER reg,
		uint8_t* data,
		uint32_t size)
{
	if(mI2CInterface != NULL)
	{
		int ret = -1;
		mI2CInterface->TakeControl();
		mI2CInterface->TransmitData((mI2CAddress|1), (const uint8_t*)&reg, 1);
		ret = mI2CInterface->ReceiveData((mI2CAddress&(~1)), data, size);
		mI2CInterface->ReleaseControl();
		return ret; // data transfer success/failure
	}

	return -1; // no interface
}
