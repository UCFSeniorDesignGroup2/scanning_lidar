// needs to be tested. Hardware needed to be fixed before testing.
// jack ritchey



#ifndef __HAL_GPIO2_H
#define __HAL_GPIO2_H

#include <hal/gpio/hal_gpio.h>


namespace HAL
{
	// this gpio pin is configured to respond to a falling edge on PE10
	// all other functionality of the pin has been masked.
	// SetPinState, ReadPinState, and OnRisingEdge are all implemented as stubs
	class HAL_GPIO2 : public HAL_GPIO_PIN
	{
	public:

		HAL_GPIO2();

		// Initialize hardware
		virtual int Initialize();

		// Set output state
		virtual void SetPinState(PIN_STATE);

		// Read input state
		virtual PIN_STATE ReadPinState();

		// used to handle interrupts
		void ProcessISR();

		// return reference to instance of HAL_GPIO2
		static HAL_GPIO_PIN* GetInstance();



	protected:

		// is initiailized?
		bool mInitialized;
		// instance of HAL_GPIO2
		static HAL_GPIO2 mInstance;
	};
}

#endif
