// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// hal_usart1.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#include "hal_usart1.h"

using namespace HAL;

HAL_USART1 HAL_USART1::mInstance;

/*
 * Constructor:
 *
 * Initializes a semaphore to provide blocking calls.
 */
HAL_USART1::HAL_USART1() :
		mSemaphore(1,0)
{

}

/*
 * Destructor:
 *
 * No memory to be freed.
 */
HAL_USART1::~HAL_USART1()
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
void HAL_USART1::BlockCallingThread()
{
	mSemaphore.Take();
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
void HAL_USART1::UnblockCallingThread()
{
	mSemaphore.Give();
}

/*
 * Initialize:
 * Parameters:
 * (none).
 *
 * Initializes USART hardware interface.
 *
 * Returns:
 * -1 if failure. 0 if success.
 */
int HAL_USART1::Initialize()
{
	return 0;
}

/*
 * TransmitData:
 * Parameters:
 * 1. (const uint8_t*) data, 2. (uint32_t) length
 *
 * Sends data over USART interface.
 *
 * Returns:
 * -1 if failure. 0 if successful.
 */
int HAL_USART1::TransmitData(const uint8_t* data, uint32_t length)
{
	return 0;
}

/*
 * ReveiceData:
 * Parameters:
 * 1. (uint8_t*) data, 2. (uint32_t) length
 *
 * Checks the dma circular buffer to see if there is data available.  If there is,
 * then the data is copied from the buffer. If there is no data available, than the
 * functions blocks until data is received or a timeout occurs.
 *
 * Returns:
 * -1 if failure. $BYTES_READ if success.
 */
int HAL_USART1::ReceiveData(uint8_t* data, uint32_t length)
{
	return 0;
}

/*
 * GetInstance:
 * Parameters:
 * (none).
 *
 * Retrieves an instance of USART1 interface so drivers may access it.
 *
 * Returns:
 * (HAL_USART1*) instance
 */
HAL_USART1* HAL_USART1::GetInstance()
{
	return &mInstance;
}
