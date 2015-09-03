// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// oal_thread.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#include "oal_thread.h"
#include "oal_common.h"

extern "C"
{
	#include <FreeRTOS.h>
	#include <task.h>
}

using namespace OAL;


/*
 * Constructor:
 * Parameters:
 * (none).
 *
 * Sets up default stacksize and priority of a task and creates a semaphore.
 */
Thread::Thread() :
	mSemaphore(1,0)
{
	mStackSize = configMINIMAL_STACK_SIZE;
	mPriority = tskIDLE_PRIORITY + 1;
	mRunning = false;
	mThreadHandle = NULL;
}

/*
 * Destructor:
 *
 * Kills the task.
 */
Thread::~Thread()
{
	Kill();
}

/*
 * SetStackSize:
 * Parameters:
 * 1. (unsigned int) stack_size
 *
 * Configures the thread's stack size.
 * (only valid if called before the task is started).
 *
 * Returns:
 * (void)
 */
void Thread::SetStackSize(unsigned int stack_size)
{
	mStackSize = stack_size;
}

/*
 * Start:
 * Parameters:
 * (none).
 *
 * Starts the thread.
 *
 * Returns:
 * (void)
 */
void Thread::Start()
{
	mRunning = true;
	xTaskCreate(Launcher, "", mStackSize, this, mPriority, &mThreadHandle);
}

/*
 * Stop:
 * Parameters:
 * (none).
 *
 * Sets the mRunning flag to false in order to notify the thread
 * that it needs to finish up.
 *
 * Returns:
 * (void)
 */
void Thread::Stop()
{
	mMutex.Lock();
	mRunning = false;
	mMutex.Unlock();
}

/*
 * Kill:
 * Parameters:
 * (none).
 *
 * Notify a thread to stop then delete it.
 *
 * Return:
 * (void)
 */
void Thread::Kill()
{
	Stop();
	vTaskDelete(mThreadHandle);
}

/*
 * Join:
 * Parameters:
 * (none).
 *
 * Notify a thread to stop then wait for it to finish and finally delete it.
 *
 * Returns:
 * (void).
 */
void Thread::Join()
{
	Stop();
	mSemaphore.Take();
	Kill();
}

/*
 * IsRunning:
 * Parameters:
 * (none).
 *
 * Checks if the thread is currently running.
 *
 * Returns:
 * (bool) is_running t/f
 */
bool Thread::IsRunning()
{
	bool running = false;
	mMutex.Lock();
	running = mRunning;
	mMutex.Unlock();
	return running;
}

/*
 * Resume:
 * Parameters:
 * (none).
 *
 * If a thread has been suspended this method will set it back to the ready state.
 *
 * Returns:
 * (void)
 */
void Thread::Resume()
{
	vTaskResume(mThreadHandle);
}

/*
 * __unblock_join:
 * Parameters:
 * (none).
 *
 * Utility function that is used to unblock a thread that has called join.
 * (called right before a thread finishes).
 *
 * Returns:
 * (void)
 */
void Thread::__unblock_join()
{
	mSemaphore.Give();
}

/*
 * Launch:
 * Parameters:
 * 1. (void (*)(void*) thread, 2. (unsigned int) stacksize, 3. (unsigned int) priority
 *
 * A convenient static method for creating thread that does not require making a class.
 *
 * Returns:
 * (oal_thread_handle)
 */
oal_thread_handle Thread::Launch(
		void (*code)(void*),
		unsigned int stacksize,
		unsigned int priority)
{
	oal_thread_handle handle;
	xTaskCreate(code, "", stacksize, NULL, priority, &handle);
	return handle;
}

/*
 * Delete:
 * Parameters:
 * 1. (oal_thread_handle) thread
 *
 * A method for deleting a statically created thread
 *
 * Returns:
 * (void)
 */
void Thread::Delete(oal_thread_handle thread)
{
	vTaskDelete(thread);
}

/*
 * Sleep:
 * Parameters:
 * 1. (Unsigned int) ticks
 *
 * Suspends a thread for $ticks(ms)
 *
 * Returns:
 * (void)
 */
void Thread::Sleep(unsigned int ticks)
{
	vTaskDelay(ticks);
}

/*
 * Launcher:
 * Parameters:
 * 1. (void*)thread class
 *
 * Used in the process of creating a thread
 *
 * Returns:
 * (void)
 */
void Thread::Launcher(void* arg)
{
	((Thread*)arg)->Run();
	((Thread*)arg)->__unblock_join();
}

/*
 * StartScheduler:
 * Parameters:
 * (none).
 *
 * Starts the freeRTOS scheduler
 *
 * Returns:
 * (void)
 */
void Thread::StartScheduler()
{
	vTaskStartScheduler();
}

/*
 * GetTicks:
 * Parameters:
 * (none).
 *
 * Gets number of ticks since the schedule was started
 *
 * Returns:
 * (unsigned int) ticks
 */
unsigned int Thread::GetTicks()
{
	return xTaskGetTickCount();
}

extern "C"
{
	/*
	 * vApplicationStackOverflowHook:
	 * Parameters:
	 * (none).
	 *
	 * Error handler that is called then a stackoverflow occurs.
	 *
	 * Returns:
	 * (void)
	 */
	void vApplicationStackOverflowHook()
	{
		while(1)
		{

		}
	}

	/*
	 * vApplicationMallocFailedHook:
	 * Parameters:
	 * (none).
	 *
	 * Error handler that is called then a malloc failure occurs.
	 *
	 * Returns:
	 * (void)
	 */
	void vApplicationMallocFailedHook()
	{
		while(1)
		{

		}
	}
}
