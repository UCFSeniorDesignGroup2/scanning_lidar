// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// hal_gpio.h
// Defines a abstract definition of a gpio pin. How the pin functions will be
// determined by its implementation of the virtual methods. The interface provides
// a method for setting the pins output state if it is an output, and reading its
// state if it is an input.  In addition to reading the state of the pin when it is
// an input, callbacks may also be specified to be called when the pin changes.
// It is not reccomended for the implementation to call these callbacks directely from
// a exti interrupt.  Instead a thread should be created that is notified by an exti
// interrupt.  This thread should then call the callbacks. Details such as internal
// pull-up resistors and if the pin is open-drain ect.. will be determined by the implementation.
//
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#ifndef HAL_GPIO_HAL_GPIO_H_
#define HAL_GPIO_HAL_GPIO_H_

#include "oal/oal_common.h"

namespace HAL
{

	/*
	 * HAL_GPIO_PIN:
	 * Abstract definition of a gpio pin.
	 */
	class HAL_GPIO_PIN : public OAL::ThreadSafe
	{
	public:

		// State of a pin
		typedef enum
		{
			HIGH,
			LOW
		} PIN_STATE;

		HAL_GPIO_PIN();
		virtual ~HAL_GPIO_PIN();

		// Initialize hardware
		virtual int Initialize() = 0;

		// Set output state
		virtual void SetPinState(PIN_STATE) = 0;

		// Read input state
		virtual PIN_STATE ReadPinState() = 0;

		// Set callback to handle rising edge
		void OnRisingEdge(void (*cb)());

		// Set callback to handle falling edge
		void OnFallingEdge(void (*cb)());

	protected:

		// callback to handle rising edge
		void (*mOnRising)();

		// callback to handle falling edge
		void (*mOnFalling)();
	};
}

#endif
