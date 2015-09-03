// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// oal_mutex.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#include "oal_mutex.h"
#include "oal_common.h"

extern "C"
{
	#include <FreeRTOS.h>
	#include <semphr.h>
}

using namespace OAL;


/*
 * Constructor:
 *
 * Allocates a mutex on the FreeRTOS heap
 */
Mutex::Mutex()
{
	mMutexHandle = xSemaphoreCreateMutex();
}

/*
 * Destructor:
 *
 * Frees the mutex from the FreeRTOS heap
 */
Mutex::~Mutex()
{
	vSemaphoreDelete(mMutexHandle);
}

/*
 * Lock:
 * Parameters:
 * (none).
 *
 * Locks the mutex. Any other code that hits this point must block until
 * the mutex is unlocked.
 *
 * Returns:
 * (void)
 */
void Mutex::Lock()
{
	portBASE_TYPE taskWoken = pdFALSE;
	if(inHandlerMode())
	{
		if(xSemaphoreTakeFromISR(mMutexHandle, &taskWoken) == pdTRUE)
		{
			portEND_SWITCHING_ISR(taskWoken);
		}
	}
	else
	{
		xSemaphoreTake(mMutexHandle, portMAX_DELAY);
	}
}

/*
 * Unlock:
 * Parameters:
 * (none).
 *
 * Makes a section of code accessible again.
 *
 * Returns:
 * (void).
 */
void Mutex::Unlock()
{
	portBASE_TYPE taskWoken = pdFALSE;
	if(inHandlerMode())
	{
		if(xSemaphoreGiveFromISR(mMutexHandle, &taskWoken) == pdTRUE)
		{
			portEND_SWITCHING_ISR(taskWoken);
		}
	}
	else
	{
		xSemaphoreGive(mMutexHandle);
	}
}
