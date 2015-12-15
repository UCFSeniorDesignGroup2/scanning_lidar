// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// oal_thread.h
// Provides method for starting the task scheduler and creating new tasks.
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#ifndef __OAL_THREAD_H
#define __OAL_THREAD_H

#include "oal_mutex.h"
#include "oal_semaphore.h"

namespace OAL
{
	// handle for managing threads
	typedef void* oal_thread_handle;


	// abstract thread class
	// override the virtual method Run or
	// call the static method Launch
	class Thread
	{
	public:

		Thread();

		virtual ~Thread();

		// Set stack size of the Thread
		void SetStackSize(unsigned int stack_size);
		// Starts the thread
		void Start();
		// Stops the thread
		void Stop();
		// Deletes thread from the scheduler
		void Kill();
		// Stops thread and waits for it to exit
		void Join();
		// Returns if running state of thread
		bool IsRunning();
		// Switch context to this thread
		void Resume();
		// override this function
		virtual void Run() = 0;
		// Unblocks threads waiting for Thread to join
		void __unblock_join();
		// Create a thread from a function
		static oal_thread_handle Launch(
				void (*code)(void*),
				unsigned int stacksize=1024,
				unsigned int priority=1);
		// delete a thread created statically
		static void Delete(oal_thread_handle thread);
		// put task for sleep for 'ticks ticks
		static void Sleep(unsigned int ticks);
		// ticks sence kernel started
		static unsigned int GetTicks();
		// start the RTOS kernel
		static void StartScheduler();

	private:

		// handle for a thread
		oal_thread_handle mThreadHandle;

		// flag to keep track of running state of thread
		bool mRunning;

		// stack size of the thread
		unsigned int mStackSize;
		// priority of the thread (default 1(lowest))
		unsigned int mPriority;

		// mutex for getting and setting mRunning synchronously
		Mutex mMutex;

		// semaphore for waiting for Thread to join()
		Semaphore mSemaphore;

		// static function used to create a thread
		static void Launcher(void* arg);
	};
}

#endif
