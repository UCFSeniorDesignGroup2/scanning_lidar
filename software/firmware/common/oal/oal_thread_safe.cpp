// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// oal_thread_safe.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#include "oal_thread_safe.h"

using namespace OAL;

/*
 * TakeControl:
 * Parameters:
 * (none).
 *
 * Locks the class
 *
 * Returns:
 * (void)
 */
void ThreadSafe::TakeControl()
{
	mMutex.Lock();
}

/*
 * ReleaseControl:
 * Parameters:
 * (none).
 *
 * Unlocks the class
 *
 * Returns:
 * (void)
 */
void ThreadSafe::ReleaseControl()
{
	mMutex.Unlock();
}
