/*
 * pwm_servo.h
 *
 *  Created on: Nov 19, 2015
 *      Author: jack
 */

#ifndef HAL_PWM_PWM1_PWM1_H_
#define HAL_PWM_PWM1_PWM1_H_

#include "hal/pwm/pwm.h"

#include "oal/oal_common.h"

namespace HAL
{
	class PWM1 : public PWM, OAL::Thread
	{
	public:

		virtual int Initialize();
		virtual void SetDutyCycle(float);

		static PWM* GetInstance();

		virtual void Run();

	protected:

		static PWM1 mInstance;

		OAL::Mutex mMutex;
		float mDutyCycle;

	};

}

#endif /* HAL_PWM_PWM1_PWM1_H_ */
