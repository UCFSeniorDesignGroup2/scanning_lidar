// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// quadrature_encoder.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 3, 2015
// ---------------------------------------------------------------------------

#include "quadrature_encoder.h"

using namespace HAL;

/*
 * Constructor:
 * Parameters:
 * (none).
 *
 * Does nothing
 */
QuadratureEncoder::QuadratureEncoder()
{
	mMaxTicks = 0;
}

/*
 * Destructor:
 *
 * Nothing to free
 */
QuadratureEncoder::~QuadratureEncoder()
{

}

/*
 * SetupTicksPerRevolution:
 * Parameters:
 * 1. (unsigned int) ticks_per_revolution
 *
 * Sets the number of ticks in a single revolution.
 * (note: setup must occur before the encoder is initialized in order to be valid)
 *
 * Returns:
 * (void)
 */
void QuadratureEncoder::SetupTicksPerRevolution(unsigned int ticks)
{
	mMaxTicks = ticks;
}

/*
 * GetTicksPerRevolution:
 * Parameters:
 * (none).
 *
 * Gets the number of ticks the encoder increments per revolution
 *
 * Returns:
 * (unsigned int) ticks
 */
unsigned int QuadratureEncoder::GetTicksPerRevolution()
{
	return mMaxTicks;
}
