/*
 * fast_pulse.h
 *
 *  Created on: Dec 5, 2015
 *      Author: jack
 */

#ifndef HAL_FAST_PULSE_FAST_PULSE_H_
#define HAL_FAST_PULSE_FAST_PULSE_H_

#include <stdint.h>

namespace HAL
{
	// special interface for sending high speed pulses
	class FastPulse
	{
	public:
		virtual ~FastPulse() {}
		// initialize the hardware
		virtual int32_t Initialize() = 0;
		// send the high speed pulse
		virtual int32_t Pulse() = 0;
	};

}


#endif /* HAL_FAST_PULSE_FAST_PULSE_H_ */
