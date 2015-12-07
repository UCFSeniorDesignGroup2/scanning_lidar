/*
 * servo.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: jack
 */




#include <drivers/servo/servo.h>

using namespace Drivers;

void Servo::SetPwmInterface(HAL::PWM* interface)
{
	mInterface = interface;
}

int Servo::Initiailze()
{
	if(mInterface == 0)
	{
		return -1;
	}
	return 0;
}

void Servo::SetPosition(float pos)
{
	// .03 - .12
	float duty = (.09*pos + .03);
	mInterface->SetDutyCycle(duty);
}
