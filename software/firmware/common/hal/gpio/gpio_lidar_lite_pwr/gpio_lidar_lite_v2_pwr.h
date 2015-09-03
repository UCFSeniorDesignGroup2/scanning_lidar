// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// gpio_lidar_lite_pwr.h
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#ifndef HAL_GPIO_GPIO_LIDAR_LITE_PWR_GPIO_LIDAR_LITE_V2_PWR_H_
#define HAL_GPIO_GPIO_LIDAR_LITE_PWR_GPIO_LIDAR_LITE_V2_PWR_H_

#include "hal/gpio/hal_gpio.h"

namespace HAL
{
	/*
	 * GPIO_LidarLiteV2_PWR:
	 * An implementation of HAL_GPIO_PIN. Used by the
	 * LidarLiteV2 driver to enable the power on the
	 * LidarLiteV2 device.
	 * (This pin is configured as an output)
	 */
	class GPIO_LidarLiteV2_PWR : public HAL_GPIO_PIN
	{
	public:

		// Initialize hardware
		virtual int Initialize();

		// Set output state
		virtual void SetPinState(PIN_STATE);

		// Read input state
		virtual PIN_STATE ReadPinState();

		// Get instance of GPIO_LidarLiteV2_PWR
		static GPIO_LidarLiteV2_PWR* GetInstance();

	protected:

		// Instance of GPIO_LidarLiteV2_PWR
		static GPIO_LidarLiteV2_PWR mInstance;
	};
}

#endif
