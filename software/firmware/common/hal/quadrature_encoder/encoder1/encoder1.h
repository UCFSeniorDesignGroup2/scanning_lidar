// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// encoder1.h
// Implementation of a quadrature encoder.
//
// Author: jack
//   Date: Sep 3, 2015
// ---------------------------------------------------------------------------


#ifndef HAL_QUADRATURE_ENCODER_ENCODER1_ENCODER1_H_
#define HAL_QUADRATURE_ENCODER_ENCODER1_ENCODER1_H_

#include "hal/quadrature_encoder/quadrature_encoder.h"

namespace HAL
{
	/*
	 * Implementation of QuadratureEncoder
	 */
	class Encoder1 : public QuadratureEncoder
	{
	public:

		Encoder1();

		virtual ~Encoder1();

		// Initialize the hardware encoder
		virtual int Initialize();

		// When the motor is in a know position call this function to set the count to zero
		virtual void ZeroTicks();

		// Get the tick position of the motor shaft
		virtual unsigned int GetTicks();

		// Get direction the motor is rotating
		virtual DIRECTION GetDirection();

		// Get an instance of encoder1
		static Encoder1* GetInstance();

	protected:

		static Encoder1 mInstance;
	};
}


#endif /* HAL_QUADRATURE_ENCODER_ENCODER1_ENCODER1_H_ */
