/*
 * fast_pulse1.h
 *
 *  Created on: Dec 5, 2015
 *      Author: jack
 *
 */

// tested and works. Generates a 40ns pulse
// jack ritchey

#ifndef HAL_FAST_PULSE_FAST_PULSE1_FAST_PULSE1_H_
#define HAL_FAST_PULSE_FAST_PULSE1_FAST_PULSE1_H_

#include <hal/fast_pulse/fast_pulse.h>

namespace HAL
{
	// high speed pulse for firing the pulse laser
	class FastPulse1 : public FastPulse
	{
	public:

		FastPulse1();
		// initialize the hardware
		virtual int32_t Initialize();
		// send pulse
		virtual int32_t Pulse();
		// get instance of FastPulse1
		static FastPulse* GetInstance();
	protected:
		// is initialized
		bool mInitialized;
		// instance of FastPulse1
		static FastPulse1 mInstance;
	};
}



#endif /* HAL_FAST_PULSE_FAST_PULSE1_FAST_PULSE1_H_ */
