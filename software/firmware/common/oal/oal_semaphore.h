// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// oal_semaphore.h
// Defines a semaphore that can be used for more advanced task synchronization.
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#ifndef __OAL_SEMAPHORE_H
#define __OAL_SEMAPHORE_H

namespace OAL
{
	typedef void* oal_semaphore_handle;

	// Simple semaphore for synchronizing tasks
	class Semaphore
	{
	public:

		Semaphore(
				unsigned int max_count,
				unsigned int start_count);

		~Semaphore();

		// Take semaphore
		void Take();

		// Take with timeout functionality
		int TakeTimeout(unsigned int timeout);

		// Give semaphore
		void Give();

		// Reset the semaphore
		void Reset();

		// Get the current count of the semaphore
		unsigned int GetCount();

	private:

		// Internal state variables
		unsigned int mMaxCount, mCount, mStartCount;

		// Handle to a semaphore
		oal_semaphore_handle mSemaphoreHandle;
	};
}

#endif
