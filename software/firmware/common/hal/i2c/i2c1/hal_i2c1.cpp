// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// hal_i2c1.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#include "hal_i2c1.h"

using namespace HAL;

// Instance of HAL_I2C1
HAL_I2C1 HAL_I2C1::mInstance;

/*
 * Constructor:
 * Parameters:
 * (none).
 *
 * Initializes a semaphore with a max count of 1 starting at 0.
 * The semaphore is used to make blocking calls.
 */
HAL_I2C1::HAL_I2C1() :
		mSemaphore(1,0)
{

}

/*
 * BlockCallingThread:
 * Parameters:
 * (none).
 *
 * Blocks the calling thread until an external event unblocks it.
 *
 * Returns:
 * (int) status.
 */
int HAL_I2C1::BlockCallingThread()
{
	return mSemaphore.TakeTimeout(mTimeout);
}

/*
 * UnblockCallingThread:
 * Parameters:
 * (none).
 *
 * Unblocks a thread from an external event.
 *
 * Returns:
 * (int) status.
 */
int HAL_I2C1::UnblockCallingThread()
{
	mSemaphore.Give();
	return 0;
}

/*
 * Initialize:
 * Parameters:
 * (none).
 *
 * Initializes the interface.
 *
 * Returns:
 * -1 if failure. 0 if success.
 */
int HAL_I2C1::Initialize()
{
	/*
	 * Todo
	 */
	return 0;
}

/*
 * TransmitData:
 * Parameters:
 * 1. (uint8_t) address, 2. (const uint8_t*) data, 3, (uint32_t length
 *
 * Transmits data over i2c.
 *
 * Returns:
 * -1 if failure. 0 if success.
 */
int HAL_I2C1::TransmitData(
		uint8_t address,
		const uint8_t* data,
		uint32_t length)
{
	/*
	 * Todo
	 */
	return 0;
}

/*
 * ReceiveData:
 * Parameters:
 * 1. (uint8_t) address, 2. (uint8_t*) data, 3, (uint32_t length
 *
 * Reads data over i2c.
 *
 * Returns:
 * -1 if failure. 0 if success.
 */
int HAL_I2C1::ReceiveData(
		uint8_t address,
		uint8_t* data,
		uint32_t length)
{
	/*
	 * Todo
	 */
	return 0;
}

/*
 * GetInstance:
 * Parameters:
 * (none).
 *
 * Retrieves an instance of I2C1 so drivers may use the interface.
 *
 * Returns:
 * (HAL_I2C1*) instance
 */
HAL_I2C1* HAL_I2C1::GetInstance()
{
	return &mInstance;
}
