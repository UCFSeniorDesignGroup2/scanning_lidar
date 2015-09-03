// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// oal_mutex.h
// Used to create sections of code that are mutually exclusive.
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#ifndef __OAL_MUTEX_H
#define __OAL_MUTEX_H

namespace OAL
{
	// handle for managing a mutex
	typedef void* oal_mutex_handle;

	 // a simple mutex for synchronizing tasks
	class Mutex
	{
	public:

		Mutex();

		~Mutex();

		// Lock the mutex
		void Lock();

		// Unlock the mutex
		void Unlock();

	private:

		// mutex handle
		oal_mutex_handle mMutexHandle;
	};
}

#endif
