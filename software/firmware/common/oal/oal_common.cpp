// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// oal_common.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#include "oal_common.h"

extern "C"
{
	#include <stm32f4xx.h>
}

using namespace OAL;

/*
 * inHandlerMode:
 * Parameters:
 * (none).
 *
 * Checks if the program is in isr handler mode.
 *
 * Returns:
 * 1 if in handler mode. 0 otherwise.
 */
int OAL::inHandlerMode (void)
{
  return __get_IPSR() != 0;
}
