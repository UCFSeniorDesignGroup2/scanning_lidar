// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// gpio_lidar_lite_v2_pwr.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#include <hal/gpio/gpio_lidar_lite_pwr/gpio_lidar_lite_v2_pwr.h>

using namespace HAL;

// Instance of GPIO_LidarLiteV2_PWR
GPIO_LidarLiteV2_PWR GPIO_LidarLiteV2_PWR::mInstance;

/*
 * Initialize:
 * Parameters:
 * (none).
 *
 * Initializes the gpio pin.
 *
 * Returns:
 * -1 if failure. 0 if success.
 */
int GPIO_LidarLiteV2_PWR::Initialize()
{
	return 0;
}

/*
 * SetPinState:
 * Parameters:
 * 1. (PIN_STATE) state
 *
 * Sets the output state of the gpio pin.
 *
 * Returns:
 * (void)
 */
void GPIO_LidarLiteV2_PWR::SetPinState(PIN_STATE state)
{

}

/*
 * ReadPinState:
 * Parameters:
 * (none).
 *
 * This method does nothing because the pin is an output not an input.
 *
 * Returns:
 * PIN_STATE
 */
HAL_GPIO_PIN::PIN_STATE GPIO_LidarLiteV2_PWR::ReadPinState()
{
	return LOW;
}

/*
 * GetInstance:
 * Parameters:
 * (none).
 *
 * Gets an instance of this pin so that it may provide access to
 * the pin.
 *
 * Returns:
 * (GPIO_LidarLiteV2_PWR*)instance
 */
GPIO_LidarLiteV2_PWR* GPIO_LidarLiteV2_PWR::GetInstance()
{
	return &mInstance;
}
