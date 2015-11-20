/*
 * servo.h
 *
 *  Created on: Nov 19, 2015
 *      Author: jack
 */

#ifndef DRIVERS_SERVO_SERVO_H_
#define DRIVERS_SERVO_SERVO_H_


#include "hal/pwm/pwm.h"


namespace Drivers
{
	class Servo
	{
	public:

		// give driver refrence to pwm interface
		void SetPwmInterface(HAL::PWM* interface);

		// initialize servo motor
		int Initiailze();

		// SetPosition:
		// Parameters:
		// 1. pos, 0 all the way to left, 1 all the way to right
		void SetPosition(float pos);
	protected:

		HAL::PWM* mInterface;
	};
}


#endif /* DRIVERS_SERVO_SERVO_H_ */
