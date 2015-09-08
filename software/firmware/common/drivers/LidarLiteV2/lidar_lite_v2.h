// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// lidar_lite_v2.h
// A driver for configuring and reading data off of the LidarLiteV2 device.
// The driver is designed to be highly configurable by using the HAL to access the
// I2C interface and GPIO pin for enabling the device.  In addition, there is also
// an optional configuration parameter that can be passed before initialization.
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#ifndef DRIVERS_LIDARLITEV2_LIDAR_LITE_V2_H_
#define DRIVERS_LIDARLITEV2_LIDAR_LITE_V2_H_

#include "oal/oal_common.h"

#include "hal/i2c/hal_i2c.h"
#include "hal/gpio/hal_gpio.h"

namespace Drivers
{
	/*
	 * LidarLiteV2:
	 * Driver for configuring and reading data off of
	 * a LidarLiteV2 device.
	 */
	class LidarLiteV2 : public OAL::ThreadSafe
	{
	public:


		// Register definitions for LidarLiteV2
		typedef enum
		{
			// uP internal control registers
			CONTROL_REG_0x00 = 0,
			CONTROL_REG_0x01,
			CONTROL_REG_0x02,
			CONTROL_REG_0x03,
			CONTROL_REG_0x04,
			CONTROL_REG_0x05,
			CONTROL_REG_0x06,
			CONTROL_REG_0x07,
			CONTROL_REG_0x08,
			CONTROL_REG_0x09,
			CONTROL_REG_0x0A,
			CONTROL_REG_0x0B,
			CONTROL_REG_0x0C,
			CONTROL_REG_0x0D,
			CONTROL_REG_0x0E,
			CONTROL_REG_0x0F,
			CONTROL_REG_0x10,
			CONTROL_REG_0x11,
			CONTROL_REG_0x12,
			CONTROL_REG_0x13,
			CONTROL_REG_0x14,
			CONTROL_REG_0x15,

			// correlation control external registers
			EX_CONTROL_REG_0x40 = 0x40,
			EX_CONTROL_REG_0x41,
			EX_CONTROL_REG_0x42,
			EX_CONTROL_REG_0x43,
			EX_CONTROL_REG_0x44,
			EX_CONTROL_REG_0x45,
			EX_CONTROL_REG_0x46,
			EX_CONTROL_REG_0x47,
			EX_CONTROL_REG_0x48,
			EX_CONTROL_REG_0x49,
			EX_CONTROL_REG_0x4A,
			EX_CONTROL_REG_0x4B,
			EX_CONTROL_REG_0x4C,
			EX_CONTROL_REG_0x4D,
			EX_CONTROL_REG_0x4E,
			EX_CONTROL_REG_0x4F,
			EX_CONTROL_REG_0x50,
			EX_CONTROL_REG_0x51,
			EX_CONTROL_REG_0x52,
			EX_CONTROL_REG_0x53,
			EX_CONTROL_REG_0x54,
			EX_CONTROL_REG_0x55,
			EX_CONTROL_REG_0x56,
			EX_CONTROL_REG_0x57,
			EX_CONTROL_REG_0x58,
			EX_CONTROL_REG_0x59,
			EX_CONTROL_REG_0x5A,
			EX_CONTROL_REG_0x5B,
			EX_CONTROL_REG_0x5C,
			EX_CONTROL_REG_0x5D,
			EX_CONTROL_REG_0x5E,
			EX_CONTROL_REG_0x5F

		} REGISTER;

		// Setup the default values for the device
		LidarLiteV2(uint8_t i2c_address=0xC4);

		// Called when the class goes out of scope
		~LidarLiteV2();

		// Initialize LidarLiteV2
		// Enables the device power and then runs the optional configuration code.
		int Initialize();

		// Set the i2c interface for the LidarLiteV2 driver.
		void SetI2CInterface(HAL::HAL_I2C* interface);

		// Set the gpio interface for powering the LidarLiteV2 device
		void SetPwrPinInterface(HAL::HAL_GPIO_PIN* gpio_pin);

		// Set the gpio interface for powering the LidarLiteV2 device
		void SetModePinInterface(HAL::HAL_GPIO_PIN* gpio_pin);

		// Set the configuration of the LidarLiteV2
		void SetConfiguration(int (*config)(LidarLiteV2*));

		// Set register on the device
		int SetRegister(
				REGISTER reg,
				const uint8_t* data ,
				uint32_t size);

		// Read register off the device
		int ReadRegister(
				REGISTER reg,
				uint8_t* data,
				uint32_t size);

	protected:

		// Instance of an I2C interface
		HAL::HAL_I2C* mI2CInterface;

		// Instance of PWR pin
		HAL::HAL_GPIO_PIN* mPwrPinInterface;

		// Instance of Mode pin
		HAL::HAL_GPIO_PIN* mModePinInterface;

		// Configuration
		int (*mConfiguration)(LidarLiteV2*);

		// I2c address
		uint8_t mI2CAddress;
	};

}

#endif
