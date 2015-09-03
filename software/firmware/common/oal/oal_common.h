// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// oal_common.h
// Provides access to the entire Operating System Abstraction Layer
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------


#ifndef __OAL_COMMON_H
#define __OAL_COMMON_H

namespace OAL
{
	// Checks if the program is in isr handler mode
	int inHandlerMode();
}

#include "oal_thread.h"
#include "oal_mutex.h"
#include "oal_semaphore.h"
#include "oal_queue.h"
#include "oal_thread_safe.h"

#endif
