#ifndef __EXTERNAL_CLOCK_H
#define __EXTERNAL_CLOCK_H

#include <stdint.h>

namespace HAL
{
	// Interface for generating a hardware counter
	class ExternalClock
	{
	public:
		virtual ~ExternalClock() {}
		// start up the hardware counter
		virtual int32_t Initialize() = 0;
		// register an interupt when the counter overflows and resets
		virtual void SetInterrupt(void (*cb)()) = 0;

	};
}

#endif
