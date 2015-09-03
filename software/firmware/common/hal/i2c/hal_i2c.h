// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// hal_i2c.h
// Abstract definition of an i2c interface. The interface can be passed in
// to drivers in order to provide a generic way of accessing i2c interfaces.
// This makes it easier to port and maintain existing code.
// ex: (drivers don't need to be rewritten in order to change i2c interface)
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#ifndef __HAL_I2C_H
#define __HAL_I2C_H

#include "oal/oal_common.h"

#include <stdint.h>

namespace HAL
{
	/*
	 * HAL_I2C:
	 * Abstract class that defines an i2c interface.
	 * Can be passed in to drivers allowing drivers to be
	 * portable to different hardware.
	 *
	 */
	class HAL_I2C : public OAL::ThreadSafe
	{
	public:

		// MODE:
		// Master/Slave
		typedef enum
		{
			SLAVE = 0,
			MASTER = 1
		} MODE;

		// Setup default values
		HAL_I2C();

		// Called when class goes out of scope
		virtual ~HAL_I2C();

		// Set the mode that the I2C interface uses
		void SetMode(MODE);

		// Set the datarate of the I2C interface
		void SetDataRate(unsigned int);

		// Set the timeout of a blocking I2C call.
		// If an error occurs and the bus locks up,
		// it will wait {timeout} ms to catch the error.
		void SetTimeout(unsigned int);

		// Initialize the hardware with the given parameters
		virtual int Initialize() = 0;

		// Transmit data over the I2C bus
		virtual int TransmitData(
				uint8_t address,
				const uint8_t* data,
				uint32_t length) = 0;

		// Receive data over the I2C bus
		virtual int ReceiveData(
				uint8_t address,
				uint8_t* data,
				uint32_t length) = 0;

	protected:

		// Mode of the i2c interface
		MODE mMode;

		// Datarate in (bits/s)
		unsigned int mDataRate;

		// Timeout of interface in ms
		unsigned int mTimeout;
	};
}

#endif
