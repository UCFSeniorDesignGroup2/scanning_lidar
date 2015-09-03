// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// hal_i2c.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#include "hal_i2c.h"


using namespace HAL;

/*
 * Constructor:
 * Parameters:
 * (none).
 *
 * Setup default values for the interface.
 */
HAL_I2C::HAL_I2C()
{
	mDataRate = 400000;
	mMode = MASTER;
	mTimeout = 100;
}

/*
 * Destructor:
 *
 * No memory to be freed.
 */
HAL_I2C::~HAL_I2C()
{

}

/*
 * SetDataRate:
 * Parameters:
 * 1. (unsigned int) data_rate
 *
 * Sets the data rate of the interface in bits per second.
 * (note: if data rate is changed interface must be re-initialized).
 *
 * Returns:
 * (void)
 */
void HAL_I2C::SetDataRate(unsigned int data_rate)
{
	mDataRate = data_rate;
}

/*
 * SetMode:
 * Parameters:
 * 1. (MODE) mode
 *
 * Determines if the interface is a slave or master. (Default is master)
 * If slave mode is selected than the address parameter is not used in
 * data transfers.
 *
 * Returns:
 * (void)
 */
void HAL_I2C::SetMode(MODE mode)
{
	mMode = mode;
}

/*
 * SetTimeout:
 * Parameters:
 * 1. (unsigned int) timeout
 *
 * All data transfers are assumed to be blocking calls. In the event that
 * an interface locks up it is possible that the program will block indefinitely.
 * The 'preferred' behavior would be for it to return an error code after the interface
 * hasen't responded for a certain period of time. This period of time is called the timeout.
 *
 * Returns:
 * (void)
 */
void HAL_I2C::SetTimeout(unsigned int timeout)
{
	mTimeout = timeout;
}

