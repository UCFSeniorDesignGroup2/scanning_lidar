// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// hal_usart.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#include "hal_usart.h"

using namespace HAL;

/*
 * Constructor:
 * Parameters:
 * (none).
 *
 * Sets the default baudrate.
 */
HAL_USART::HAL_USART()
{
	mBaudrate = 9600;
}

/*
 * Destructor:
 *
 * No memory to free.
 */
HAL_USART::~HAL_USART()
{

}

/*
 * SetBaudrate:
 * Parameters:
 * 1. (unsigned int) baudrate
 *
 * Sets the baudrate of the interface. If the baudrate of the interface is
 * changed it must be re-initiailzed.
 *
 * Returns:
 * (void)
 */
void HAL_USART::SetBaudrate(unsigned int baudrate)
{
	mBaudrate = baudrate;
}
