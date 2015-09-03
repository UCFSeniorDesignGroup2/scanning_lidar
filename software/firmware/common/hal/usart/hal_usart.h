// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// hal_usart.h
// Abstract description of a USART interface.
// This class can be passed in to a driver that uses
// USART allowing drivers to be portable.
//
// Depending on the application, there may be flow control,
// parity ect.. These details will be left to the implementation.
// The only parameter that can be set is the baudrate. The reason
// is that many devices allow multiple baudrate settings
// after initialization.  For this reason, some drivers may need to
// be able to change the baudrate on the fly.
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#ifndef __HAL_UASRT_H
#define __HAL_UASRT_H

#include "oal/oal_common.h"

#include <stdint.h>

namespace HAL
{
	/*
	 * HAL_USART:
	 * Abstract definition of a USART interface
	 */
	class HAL_USART : public OAL::ThreadSafe
	{
	public:

		// Setup default values for USART interface
		HAL_USART();

		// Called when structure goes out of scope
		virtual ~HAL_USART();

		// Set the baudrate of the interface
		void SetBaudrate(unsigned int);

		// Initialize the hardware interface
		virtual int Initialize() = 0;

		// Transmit data
		virtual int TransmitData(const uint8_t* data, uint32_t length) = 0;

		// Read data from the rx buffer
		virtual int ReceiveData(uint8_t* data, uint32_t length) = 0;

	protected:

		// The baudrate of the interface
		unsigned int mBaudrate;

		// A lock for claiming control of the interface
		OAL::Mutex mLock;
	};
}

#endif
