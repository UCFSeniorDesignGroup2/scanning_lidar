// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// oal_queue.h
// Queues are used to pass data from one thread to another in a thread-safe way.
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#ifndef __OAL_QUEUE_H
#define __OAL_QUEUE_H

namespace OAL
{
	// handle for managing a queue
	typedef void* oal_queue_handle;

	/*
	 * Base class for implementing a queue. Used to pass data to threads from ISRs and
	 * to send data from one thread to another.
	 */
	class QueueBase
	{
	public:

		// Setup queue
		QueueBase(unsigned int length, unsigned int item_size);

		// Free queue
		~QueueBase();

		// Put data in a queue
		int Enqueue(const void* data);

		// Take data out of a queue
		int Dequeue(void* data);

		// Get the size of a queue
		unsigned int GetSize();

		// Clear the queue
		void Clear();

	private:

		// Dimensions of the queue
		unsigned int mLength, mItemSize;

		// Handle for the queue
		oal_queue_handle mQueueHandle;
	};

	// Template class for convenience
	template <class T> class Queue : public QueueBase
	{
	public:
		Queue(unsigned int length) :
			QueueBase(length,sizeof(T))
		{

		}

		int Enqueue(const T* data)
		{
			return QueueBase::Enqueue((const void*)data);
		}

		int Dequeue(T* data)
		{
			return QueueBase::Dequeue((const void*)data);
		}
	};

}

#endif
