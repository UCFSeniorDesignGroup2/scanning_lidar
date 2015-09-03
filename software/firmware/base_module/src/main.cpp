// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// main.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#include "diag/Trace.h"

// include OAL
#include "oal/oal_common.h"


void main_task(void* args)
{
	(void)args;

	while(1)
	{
		/*
		 * todo
		 */
	}
}


int main(int argc, char* argv[])
{
	(void)argc; (void)argv;

	// launch first thread
	OAL::Thread::Launch(main_task);

	// start scheduler
	OAL::Thread::StartScheduler();

	return 0;
}
