// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// hal_usart2.h
// Implementation of a USART interface.
// Used to establish communication between the Base Module and an external device.
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#ifndef __HAL_USART1_H
#define __HAL_USART1_H

#include "../hal_usart.h"


namespace HAL
{
	/*
	 * HAL_USART2:
	 * An implementation of a HAL_USART interface.
	 */
	class HAL_USART2
	{
	public:

		// Setup data structure
		HAL_USART2();

		// Called when class is out of scope
		virtual ~HAL_USART2();

		// Block calling thread
		void BlockCallingThread();

		// Unblock calling thread
		void UnblockCallingThread();

		// Initialize the hardware interface
		virtual int Initialize();

		// Transmit data
		virtual int TransmitData(const uint8_t* data, uint32_t length);

		// Read data from the rx buffer
		virtual int ReceiveData(uint8_t* data, uint32_t length);

		// Get an instance of HAL_USART2
		static HAL_USART2* GetInstance();

	protected:

		// Used for blocking calling thread
		OAL::Semaphore mSemaphore;

		// Instance of HAL_USART2
		static HAL_USART2 mInstance;
	};
}

#endif
