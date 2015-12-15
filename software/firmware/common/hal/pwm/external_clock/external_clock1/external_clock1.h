// tested and works correctly. Note(generates 16.8MHz clock instead of 16Mhz)
// jack ritchey

#ifndef __EXTERNAL_CLOCK1_H
#define __EXTERNAL_CLOCK1_H

#include <hal/pwm/external_clock/external_clock.h>

namespace HAL
{
	// hardware counter used to generate 16Mhz signal for tdc
	class ExternalClock1 : public ExternalClock
	{
	public:
		ExternalClock1();
		// startup counter
		virtual int32_t Initialize();
		// set the interrupt for when the counter resets
		virtual void SetInterrupt(void (*cb)());
		// get an instance of ExternalClock1
		static ExternalClock* GetInstance();
		// called from hardware timer interrupt
		void OnIsr();

	protected:

		// callback from interrupt
		void (*mCb)();
		// instance of ExternalClock1
		static ExternalClock1 mInstance;
	};
}

#endif
