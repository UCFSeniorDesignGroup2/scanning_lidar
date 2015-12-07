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
#include <hal/pwm/pwm1/pwm1.h>
#include <hal/usb_cdc/hal_usb_cdc.h>
#include <hal/analog_output/analog_output1/analog_output1.h>
#include <hal/fast_pulse/fast_pulse1/fast_pulse1.h>
#include <hal/hal_spi/hal_spi1/hal_spi1.h>
#include <hal/external_clock/external_clock1/external_clock1.h>
#include <hal/gpio/hal_gpio1/hal_gpio1.h>
#include <hal/sync_pulse/sync_pulse1/sync_pulse1.h>
#include <hal/gpio/hal_gpio2/hal_gpio2.h>

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
	uint8_t tx_data[(const uint32_t)length+1];

	if(length == 1)
	{

	}
	else if(length > 1)
	{

	}
	return mInterface->WriteData(data, length);
}

// test function for getting falling edge of intb
volatile int a = 0;
void test()
{
	a++;
}

void main_task(void* args)
{
	(void)args;

	// 16MHz clock for tdc
	HAL::ExternalClock* tdc_clock = HAL::ExternalClock1::GetInstance();
	tdc_clock->Initialize(); // start 16 MHz clock

	// tdc enable pin
	HAL::HAL_GPIO_PIN* tdc_enable = HAL::HAL_GPIO1::GetInstance();
	tdc_enable->Initialize();

	// pin for starting the tdc counter
	HAL::SyncPulse* tdc_sync_start = HAL::SyncPulse1::GetInstance();
	tdc_sync_start->SetExternalClock(tdc_clock);
	tdc_sync_start->Initialize();

	// intb pin. this needs to be tested!
	HAL::HAL_GPIO_PIN* tdc_intb = HAL::HAL_GPIO2::GetInstance();
	tdc_intb->Initialize();
	tdc_intb->OnFallingEdge(test);

	// spi for tdc
	HAL::HAL_SPI* tdc_spi_interface = HAL::HAL_SPI1::GetInstance();
	tdc_spi_interface->Initialize(); // initialize tdc spi interface


	// dac output for threshold of comparator
	HAL::AnalogOutput* dac_out = HAL::AnalogOutput1::GetInstance();
	dac_out->Initialize();
	dac_out->SetVoltage(0.1);

	// quick pulse method for pulse laser
	HAL::FastPulse* pulse = HAL::FastPulse1::GetInstance();
	pulse->Initialize();



	// power on sequence for tdc
	tdc_enable->SetPinState(HAL::HAL_GPIO_PIN::LOW); // enable the tdc
	OAL::Thread::Sleep(100);
	tdc_enable->SetPinState(HAL::HAL_GPIO_PIN::HIGH); // enable the tdc
	OAL::Thread::Sleep(100);

	// set the start bit in the TDC
	uint8_t tx_data[] = {0x40,1};
	uint8_t rx_data[] = {0,0};
	tdc_spi_interface->TransferData(tx_data, rx_data, 2);

	// send start signal
	tdc_sync_start->SetState(1);
	OAL::Thread::Sleep(100);
	tdc_sync_start->SetState(0);


	// initialize RangeFinder driver
//	Drivers::RangeFinder range_finder;
//	range_finder.SetTDCExternalClock(tdc_clock);
//	range_finder.SetTDCEnablePin(tdc_enable);
//	range_finder.SetTDCStartPin(tdc_sync_start);
//	range_finder.SetTDCIntBPin(tdc_intb);
//	range_finder.SetTDCSPIInterface(tdc_spi_interface);
//	range_finder.SetThresholdPin(dac_out);
//	range_finder.SetLaserFirePin(pulse);




	// create USB data pipe
//	HAL::USB_CDC* usb_instance = HAL::USB_CDC::GetInstance();
//	usb_instance->Initialize();
//	USBDataPipe data_pipe(usb_instance);

	// pass data pipe to slip interface
//	Drivers::SlipInterface slip_interface;
//	slip_interface.SetDataPipe(&data_pipe);

//	// intiailze pwm interface
//	HAL::PWM* pwm1 = HAL::PWM1::GetInstance();
//	pwm1->Initialize();
//
//	// setup servo
//	Drivers::Servo servo1;
//	servo1.SetPwmInterface(pwm1);
//	servo1.Initiailze();
//	// set start position to 1
//	servo1.SetPosition(1);
//
//
//	float pos = 0.0; // position fo the servo
//	int sign = 1; // sign for direction of servo
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


//    	pulse->Pulse();
		OAL::Thread::Sleep(1);
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


