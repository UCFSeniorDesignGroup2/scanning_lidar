// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// oal_queue.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------


#include "oal_queue.h"
#include "oal_common.h"

extern "C"
{
	#include <FreeRTOS.h>
	#include <queue.h>
}

using namespace OAL;

/*
 * Constructor:
 *
 * Creates a queue on the freeRTOS heap and sets some values
 */
QueueBase::QueueBase(unsigned int length, unsigned int item_size)
{
	mQueueHandle = xQueueCreate(length, item_size);
	mLength = length;
	mItemSize = item_size;
}

/*
 * Destructor:
 *
 * Frees queue from the freeRTOS heap
 */
QueueBase::~QueueBase()
{
	vQueueDelete(mQueueHandle);
}

/*
 * Enqueue:
 * Parameters:
 * 1. (const void*) data.
 *
 * Places a data item into the queue.
 *
 * Returns:
 * (int) status
 */
int QueueBase::Enqueue(const void* data)
{
	int ret;
	portBASE_TYPE taskWoken = pdFALSE;
	if(inHandlerMode())
	{
		ret = xQueueSendFromISR(mQueueHandle, data, &taskWoken);
	}
	else
	{
		ret = xQueueSend(mQueueHandle, data, 0);
	}
	portEND_SWITCHING_ISR(taskWoken);
	return ret;
}

/*
 * Dequeue:
 * Parameters:
 * 1. (void*) data
 *
 * Gets data from the queue.
 *
 * Returns:
 * (int) status
 */
int QueueBase::Dequeue(void* data)
{
	int ret;
	portBASE_TYPE taskWoken = pdFALSE;
	if(inHandlerMode())
	{
		ret = xQueueReceiveFromISR(mQueueHandle, data, &taskWoken);
	}
	else
	{
		ret = xQueueReceive(mQueueHandle, data, portMAX_DELAY);
	}

	portEND_SWITCHING_ISR(taskWoken);

	return ret;
}

/*
 * GetSize:
 * Parameters:
 * (none).
 *
 * Gets the number of items in the queue
 *
 * Returns:
 * (unsigned int) size
 */
unsigned int QueueBase::GetSize()
{
	if(inHandlerMode())
	{
		return uxQueueMessagesWaitingFromISR(mQueueHandle);
	}
	else
	{
		return uxQueueMessagesWaiting(mQueueHandle);
	}
}

/*
 * Clear:
 * Parameters:
 * (none).
 *
 * Resets the queue size to zero.
 * (This method frees the queue and reallocates it so use it sparingly).
 *
 * Returns:
 * (void)
 */
void QueueBase::Clear()
{
	vQueueDelete(mQueueHandle);
	mQueueHandle = xQueueCreate(mLength, mItemSize);
}
