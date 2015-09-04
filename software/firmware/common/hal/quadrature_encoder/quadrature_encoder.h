// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// quadrature_encoder.h
// An interface for reading encoder position of the motor.
//
// Author: jack
//   Date: Sep 3, 2015
// ---------------------------------------------------------------------------


#ifndef HAL_QUADRATURE_ENCODER_QUADRATURE_ENCODER_H_
#define HAL_QUADRATURE_ENCODER_QUADRATURE_ENCODER_H_

#include "oal/oal_common.h"

namespace HAL
{
	/*
	 * Abstract Definition of a quadrature encoder
	 */
	class QuadratureEncoder : public OAL::ThreadSafe
	{
	public:

		// Direction the motor is rotating
		typedef enum
		{
			CLOCKWISE = 0,
			COUNTERCLOCKWISE
		} DIRECTION;

		QuadratureEncoder();

		virtual ~QuadratureEncoder();

		// Initialize the hardware encoder
		virtual int Initialize() = 0;

		// When the motor is in a know position call this function to set the count to zero
		virtual void ZeroTicks() = 0;

		// Get the tick position of the motor shaft
		virtual unsigned int GetTicks() = 0;

		// Get the direction the motor is rotating
		virtual DIRECTION GetDirection() = 0;

		// Set the total number of ticks per revolution
		void SetupTicksPerRevolution(unsigned int ticks);

		// Get the total number of ticks per revolution
		unsigned int GetTicksPerRevolution();

	protected:

		// Number of counts in a revolution
		unsigned int mMaxTicks;

	};
}

#endif /* HAL_QUADRATURE_ENCODER_QUADRATURE_ENCODER_H_ */
