// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// encoder1.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 3, 2015
// ---------------------------------------------------------------------------

#include "encoder1.h"

using namespace HAL;

Encoder1 Encoder1::mInstance;

/*
 * Constructor:
 * Parameters:
 * (none).
 *
 * Setup the encoder
 */
Encoder1::Encoder1()
{

}

/*
 * Destructor:
 *
 * Nothing to free
 */
Encoder1::~Encoder1()
{

}

/*
 * Initialize:
 * Parameters:
 * (none).
 *
 * Initializes the hardware and sets the max ticks per revolution
 *
 * Returns:
 * -1 if failure. 0 if success.
 */
int Encoder1::Initialize()
{
	return 0;
}

/*
 * ZeroTicks:
 * Parameters:
 * (none).
 *
 * Sets the current position to zero.
 * (used to calibrate the motor encoder)
 *
 * Returns:
 * (void)
 */
void Encoder1::ZeroTicks()
{

}

/*
 * GetTicks:
 * Parameters:
 * (none).
 *
 * Gets the current tick position of the motor
 *
 * Returns:
 * (unsigned int) tick_position
 */
unsigned int Encoder1::GetTicks()
{
	return 0;
}

/*
 * GetDirection:
 * Parameters:
 * (none).
 *
 * Gets the direction the motor is rotating
 *
 * Returns:
 * (void).
 */
QuadratureEncoder::DIRECTION Encoder1::GetDirection()
{
	return CLOCKWISE;
}

/*
 * GetInstance:
 * Parameters:
 * (none).
 *
 * Gets an instance of Encoder1
 *
 * Returns:
 * (Encoder1*) instance
 */
Encoder1* Encoder1::GetInstance()
{
	return &mInstance;
}
