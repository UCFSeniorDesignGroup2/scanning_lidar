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

// include HAL
#include <hal/pwm/pwm.h>
#include <hal/pwm/pwm1/pwm1.h>
#include <hal/usb_cdc/hal_usb_cdc.h>
#include <hal/i2c_master/i2c_master.h>
#include <hal/i2c_master/i2c3_master/i2c3_master.h>

// include Drivers
#include <drivers/servo/servo.h>
#include <drivers/slip_interface/slip_interface.h>
#include <drivers/range_finder/range_finder.h>

// std libs
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// implementation of a Data pipe to be used with slip interface
// pipes data through USB
class USBDataPipe : public Drivers::DataPipe
{
public:
	USBDataPipe(HAL::USB_CDC* interface) {

		mInterface = interface;
	}

	virtual int WriteData(const uint8_t* data, uint32_t length);

protected:

	HAL::USB_CDC* mInterface;
};

int USBDataPipe::WriteData(const uint8_t* data, uint32_t length)
{
	return mInterface->WriteData(data, length);
}

void main_task(void* args)
{
	(void)args;



	// create USB data pipe
	HAL::USB_CDC* usb_instance = HAL::USB_CDC::GetInstance();
	usb_instance->Initialize();
	USBDataPipe data_pipe(usb_instance);

	// pass data pipe to slip interface
	Drivers::SlipInterface slip_interface;
	slip_interface.SetDataPipe(&data_pipe);

	// intiailze pwm interface
	HAL::PWM* pwm1 = HAL::PWM1::GetInstance();
	pwm1->Initialize();

	// setup servo
	Drivers::Servo servo1;
	servo1.SetPwmInterface(pwm1);
	servo1.Initiailze();
	// set start position to 1
	servo1.SetPosition(1);

	// initiailze range finder driver
//	Drivers::RangeFinder range_finder;
//	HAL::I2C_MASTER* i2c3 = HAL::I2C3_MASTER::GetInstance();
//	i2c3->Initialize();
//	range_finder.SetI2CInterface(i2c3);
//	int32_t distance = range_finder.GetRangeInMiliMeters();

	float pos = 0.0; // position fo the servo
	int sign = 1; // sign for direction of servo

    while(1)
    {
//    	uint8_t data[64];
//
//    	uint32_t ang_pos = (uint32_t)(pos * 1000);
//    	memcpy(data, (uint8_t*)&ang_pos, sizeof(uint32_t));
//
//    	uint32_t distance = range_finder.GetRangeInMiliMeters();
//    	memcpy(data+4, (uint8_t*)&distance, sizeof(uint32_t));
//
//    	// send data reading to computer
//    	slip_interface.SendSlipData(data, 8);
//
//
//    	if(pos > 1)
//    	{
//    		sign = -1;
//    	}
//    	else if(pos < 0)
//    	{
//    		sign = 1;
//    	}
//    	pos += sign * .01; // move to next position
//    	servo1.SetPosition(pos);
		OAL::Thread::Sleep(50);
    }

}


int main(int argc, char* argv[])
{
	(void)argc; (void)argv;

	// launch first thread
	OAL::Thread::Launch(main_task);

	// start scheduler
	OAL::Thread::StartScheduler();

	while(1);

	return 0;
}

