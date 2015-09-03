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
			COMMAND_CONTROL = 0,
			SYSTEM_STATUS,
			MAX_ACQ_CNT,
			CORRELATION_RECORD_CNT,
			ACQ_MODE_CONTROL,
			THRESHOLD_OFFSET,
			DELAY_REF_L,
			DELAY_REF_H,
			REF_PEAK,
			VELOCITY,
			DELAY_RET_L,
			DELAY_RET_H,
			SIG_PEAK,
			NOISE_FLOOR,
			SIGNAL_STRENGTH,
			DISTANCE_L,
			DISTANCE_H,
			THRESHOLD_VALUE,
			BURST_DELAY,
			DISTANCE_OFFSET,
			PREV_DISTANCE_L,
			PREV_DISTANCE_H,

			// correlation control external registers
			EX_COMMAND_CONTROL = 0x40,
			EX_HARDWARE_VERSION,
			EX_PREAMP_CONTROL,
			EX_TRANSMIT_POWER,
			EX_RANGE_GATE_L,
			EX_RANGE_GATE_H,
			EX_RANGE_PWM_OUT,
			EX_ACQ_STATUS,
			EX_MES_PREAMP_STATUS,
			EX_OUTPUT_PORT,
			EX_RANGE_CRIT,
			EX_2ND_PEAK,
			EX_SOFTWARE_VERSION,
			EX_RECORD_SIZE_SEL,
			EX_DATA_ACCESS_PORT,
			EX_ACQ_SETTINGS,
			EX_CORRELATION_SIG_L,
			EX_CORRELATION_SIG_H,
			EX_SIGNAL_PEAK,
			EX_CORRELATION_NOISE_FLOOR,
			EX_SIGNAL_STRENGTH,
			EX_RESET_CORRELATOR,
			EX_CORR_DATA_ACCESS_PORT,
			EX_CLK_SYNC,
			EX_MES_LASER_PWR,
			EX_MES_FINE_DELAY,
			EX_COURSE_DELAY_L,
			EX_COURSE_DELAY_H,
			EX_POS_ZERO_CROSSING,
			EX_NEG_ZERO_CROSSING,
			EX_PWR_CONTROL,
			EX_VEL_MEAS_WIN_SETTING

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
		void SetGPIOInterface(HAL::HAL_GPIO_PIN* gpio_pin);

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

		// Instance of GPIO pin
		HAL::HAL_GPIO_PIN* mGPIOInterface;

		// Configuration
		int (*mConfiguration)(LidarLiteV2*);

		// I2c address
		uint8_t mI2CAddress;
	};

}

#endif
