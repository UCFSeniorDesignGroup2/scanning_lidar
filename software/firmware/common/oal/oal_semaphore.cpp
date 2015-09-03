// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// oal_semaphore.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#include "oal_semaphore.h"
#include "oal_common.h"

extern "C"
{
	#include <FreeRTOS.h>
	#include <semphr.h>
}

using namespace OAL;


/*
 * Constructor:
 * Parameters:
 * (none).
 *
 * Sets up state variables and allocates the semaphore on the freeRTOS heap.
 *
 */
Semaphore::Semaphore(unsigned int max_count, unsigned int start_count)
{
	mMaxCount = max_count;
	mStartCount = start_count;
	mCount = mStartCount;
	mSemaphoreHandle = xQueueCreateCountingSemaphore(mMaxCount, mStartCount);
}

/*
 * Destructor:
 *
 * Frees semaphore from the freeRTOS heap
 */
Semaphore::~Semaphore()
{
	vSemaphoreDelete(mSemaphoreHandle);
}

/*
 * Take:
 * Parameters:
 * (none).
 *
 * Decrements semaphore count and blocks when it reaches zero.
 *
 * Returns:
 * (void)
 */
void Semaphore::Take()
{
	portBASE_TYPE taskWoken = pdFALSE;
	if(inHandlerMode())
	{
		if(xSemaphoreTakeFromISR(mSemaphoreHandle, &taskWoken) != pdTRUE)
		{
			return;
		}
		portEND_SWITCHING_ISR(taskWoken);
	}
	else
	{
		xSemaphoreTake(mSemaphoreHandle, portMAX_DELAY);
	}
	if(mCount > 0)
	{
		mCount--;
	}
}

/*
 * TakeTimeout:
 * Parameters:
 * 1. (unsigned int) timeout
 *
 * Decrements the count until it reaches 0. If called when the
 * count is 0 the thread will block until Give is called or until the
 * timeout has occured.
 *
 * Returns:
 * -1 if timeout occurs. 0 if it is unblocked by a give()
 */
int Semaphore::TakeTimeout(unsigned int timeout)
{
	// no timeout in isr
	if(inHandlerMode())
	{
		return -1;
	}

	if(xSemaphoreTake(mSemaphoreHandle, timeout) == pdTRUE)
	{
		return 0;
	}
	else
	{
		return -1;
	}

}

/*
 * Give:
 * Parameters:
 * (none).
 *
 * Increments the counter until it reaches max_count. A thread is blocking
 * on a semaphore take() than this method will unblock the thread.
 *
 * Returns:
 * (void)
 */
void Semaphore::Give()
{
	portBASE_TYPE taskWoken = pdFALSE;
	if(inHandlerMode())
	{
		if(xSemaphoreGiveFromISR(mSemaphoreHandle, &taskWoken) == pdTRUE)
		{
			portEND_SWITCHING_ISR(taskWoken);
		}

	}
	else
	{
		xSemaphoreGive(mSemaphoreHandle);
	}
	if(mCount < mMaxCount)
	{
		mCount++;
	}
}

/*
 * Reset:
 * Parameters:
 * (none).
 *
 * Resets the semaphore.
 *
 * Returns:
 * (void)
 */
void Semaphore::Reset()
{
	vSemaphoreDelete(mSemaphoreHandle);
	mSemaphoreHandle = xSemaphoreCreateCounting(mMaxCount, mStartCount);
}

/*
 * GetCount:
 * Parameters:
 * (none).
 *
 * Gets the current count of the semaphore.
 *
 * Returns:
 * (unsigned int) current count
 */
unsigned int Semaphore::GetCount()
{
	return mCount;
}
