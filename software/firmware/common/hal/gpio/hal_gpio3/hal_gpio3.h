/*
 * hal_gpio3.h
 *
 *  Created on: Dec 10, 2015
 *      Author: jack
 */

#ifndef HAL_GPIO_HAL_GPIO3_HAL_GPIO3_H_
#define HAL_GPIO_HAL_GPIO3_HAL_GPIO3_H_


#include <hal/gpio/hal_gpio.h>

namespace HAL
{
	// this gpio pin was implemented as an output pin
	// reading the pin state and registering interrupts will
	// have no effect because they are not implemented.
	// the purpose of this pin is to control the peak reset pin
	class HAL_GPIO3 : public HAL_GPIO_PIN
	{
	public:
		HAL_GPIO3();

		// Initialize hardware
		virtual int Initialize();

		// Set output state
		virtual void SetPinState(PIN_STATE);

		// Read input state
		virtual PIN_STATE ReadPinState();

		// return instance of HAL_GPIO1
		static HAL_GPIO_PIN* GetInstance();
	protected:

		// is initialized
		bool mInitialized;

		// instance of HAL_GPIO1
		static HAL_GPIO3 mInstance;
	};
}


#endif /* HAL_GPIO_HAL_GPIO3_HAL_GPIO3_H_ */
