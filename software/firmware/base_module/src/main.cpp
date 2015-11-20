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

#include <hal/pwm/pwm.h>
#include <hal/pwm/pwm1/pwm1.h>

#include <drivers/servo/servo.h>
#include <hal/usb_cdc/hal_usb_cdc.h>

#include <drivers/slip_interface/slip_interface.h>
#include <drivers/range_finder/range_finder.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>


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
	HAL::USB_CDC* usb_instance = HAL::USB_CDC::GetInstance();
	usb_instance->Initialize();

	USBDataPipe data_pipe(usb_instance);

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

	Drivers::RangeFinder range_finder;


	float pos = 0.05;
	int sign = 1;
    while(1)
    {
    	uint8_t data[64];

    	uint32_t ang_pos = (uint32_t)(pos * 1000);
    	memcpy(data, (uint8_t*)&ang_pos, sizeof(uint32_t));

    	uint32_t distance = range_finder.GetRangeInMiliMeters();
    	memcpy(data+4, (uint8_t*)&distance, sizeof(uint32_t));


    	slip_interface.SendSlipData(data, 8);


    	if(pos > 1)
    	{
    		sign = -1;
    	}
    	else if(pos < 0)
    	{
    		sign = 1;
    	}
    	pos += sign * .001; // slowly change the position
    	servo1.SetPosition(pos);
		OAL::Thread::Sleep(5);
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

