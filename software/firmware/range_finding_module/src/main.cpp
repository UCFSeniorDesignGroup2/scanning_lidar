// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// main.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#include "diag/Trace.h"

// include OAL
#include "oal/oal_common.h"

// include drivers
#include "drivers/LidarLiteV2/lidar_lite_v2.h"
#include "drivers/LidarLiteV2/lidar_lite_v2_conf.h"

// include hal for configuring drivers
#include "hal/gpio/gpio_lidar_lite_pwr/gpio_lidar_lite_v2_pwr.h"
#include "hal/i2c/i2c1/hal_i2c1.h"

// Instance of  LidarLiteV2
Drivers::LidarLiteV2 lidar;


void main_task(void* args)
{
	(void)args;

	// claim control of the lidar instance
	lidar.TakeControl();

	// set the i2c interface
	lidar.SetI2CInterface(HAL::HAL_I2C1::GetInstance());

	// set the gpio interface
	lidar.SetPwrPinInterface(HAL::GPIO_LidarLiteV2_PWR::GetInstance());

	 // set the configuration
	lidar.SetConfiguration(Drivers::LidarLiteConfig_High_RepRate);

	// initialize the lidar
	lidar.Initialize();

	// give up control of the lidar
	lidar.ReleaseControl();

	while(1)
	{
		/*
		 * todo
		 */
	}
}


int main(int argc, char* argv[])
{
	(void)argc; (void)argv;

	// launch first thread
	OAL::Thread::Launch(main_task);

	// start scheduler
	OAL::Thread::StartScheduler();

	return 0;
}
