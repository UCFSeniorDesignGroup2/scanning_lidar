#ifndef __SYNC_PULSE_H
#define __SYNC_PULSE_H

#include <stdint.h>

#include <hal/external_clock/external_clock.h>

namespace HAL
{
	// this is a special output pin that is syncronized with
	// a timer so that it can only change at the exact
	// moment the timer resets.(aka. on the external clock edge)
	class SyncPulse
	{
	public:
		SyncPulse();
		virtual ~SyncPulse() {}
		// the clock to syncronize with
		void SetExternalClock(ExternalClock* ext_clock);

		// initialize hardware
		virtual int32_t Initialize() = 0;

		// set the state of the pin. the pin will change
		// when it gets an interrupt from the clock
		virtual int32_t SetState(uint8_t state) = 0;

	protected:

		ExternalClock* mExtClock;
	};
}

#endif
