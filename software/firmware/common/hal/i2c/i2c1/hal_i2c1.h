// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// hal_i2c1.h
// Implementation of HAL_I2C. Used by the LidarLiteV2 Driver to communicate
// with the LidarLiteV2 device.
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#ifndef __HAL_I2C1_H
#define __HAL_I2C1_H

#include "hal/i2c/hal_i2c.h"

namespace HAL
{
	/*
	 * HAL_I2C1:
	 * An implementation of HAL_I2C interface.
	 */
	class HAL_I2C1 : public HAL_I2C
	{
	public:

		// Setup default configuration
		HAL_I2C1();

		// Block the calling thread
		int BlockCallingThread();

		// Unblock the calling thread
		int UnblockCallingThread();

		// Initialize the hardware with the given parameters
		virtual int Initialize();

		// Transmit data over the I2C bus
		virtual int TransmitData(
				uint8_t address,
				const uint8_t* data,
				uint32_t length);

		// Receive data over the I2C bus
		virtual int ReceiveData(
				uint8_t address,
				uint8_t* data,
				uint32_t length);

		// Return an instance of I2C1
		static HAL_I2C1* GetInstance();

	protected:

		// Used to block and unblock calling threads
		OAL::Semaphore mSemaphore;

		// An instance of HAL_I2C1
		static HAL_I2C1 mInstance;
	};
}

#endif
