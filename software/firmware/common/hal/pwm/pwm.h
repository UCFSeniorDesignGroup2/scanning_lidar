/*
 * pwm.h
 *
 *  Created on: Nov 19, 2015
 *      Author: jack
 */

#ifndef HAL_PWM_PWM_H_
#define HAL_PWM_PWM_H_

namespace HAL
{
	class PWM
	{
	public:

		virtual ~PWM() {}
		virtual int Initialize() = 0;
		virtual void SetDutyCycle(float) = 0;
	};
}


#endif /* HAL_PWM_PWM_H_ */
