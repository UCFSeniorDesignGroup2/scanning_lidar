// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// oal_thread_safe.h
// Used to give a class thread safe functionality.
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#ifndef OAL_OAL_THREAD_SAFE_H_
#define OAL_OAL_THREAD_SAFE_H_

#include "oal_common.h"

namespace OAL
{
	// Defines abstract threadsafe class
	class ThreadSafe
	{
	public:

		// Let a thread take control of the structure
		void TakeControl();
		// Let a thread give up control of the structure
		void ReleaseControl();

	protected:
		Mutex mMutex;
	};
}

#endif
