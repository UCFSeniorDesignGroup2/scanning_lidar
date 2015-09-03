// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// hal_gpio.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------


#include "hal_gpio.h"

#include <stddef.h>

using namespace HAL;

/*
 * Constructor:
 * Parameters:
 * (none).
 *
 * Sets callback functions to NULL
 */
HAL_GPIO_PIN::HAL_GPIO_PIN()
{
	mOnRising = NULL;
	mOnFalling = NULL;
}

/*
 * Destructor:
 *
 * No memory to deallocate.
 */
HAL_GPIO_PIN::~HAL_GPIO_PIN()
{

}

/*
 * OnRisingEdge:
 * Parameters:
 * 1. (void (*)()) callback
 *
 * Sets a callback to be called when an input pin changes
 * from low state to high state.
 *
 * Returns:
 * (void)
 */
void HAL_GPIO_PIN::OnRisingEdge(void (*cb)())
{
	mOnRising = cb;
}

/*
 * OnFallingEdge:
 * Parameters:
 * 1. (void (*)()) callback
 *
 * Sets a callback to be called when an input pin changes
 * from high state to low state.
 *
 * Returns:
 * (void)
 */
void HAL_GPIO_PIN::OnFallingEdge(void (*cb)())
{
	mOnFalling = cb;
}
