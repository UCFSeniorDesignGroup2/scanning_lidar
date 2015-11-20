/*
 * pwm_servo.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: jack
 */

#include <hal/pwm/pwm1/pwm1.h>

#include <stm32f4xx.h>

using namespace HAL;

extern "C"
{
	static void PWM_Init();
	static void SetPulseWidth(float);
}


PWM1 PWM1::mInstance;

int PWM1::Initialize()
{
	PWM_Init();
	return 0;
}

void PWM1::SetDutyCycle(float duty_cycle)
{
	SetPulseWidth(duty_cycle);
}

PWM* PWM1::GetInstance()
{
	return &mInstance;
}


extern "C"
{
	TIM_HandleTypeDef    TimHandle;
	TIM_OC_InitTypeDef sConfig;

	uint32_t period = 0;
	uint8_t err = 0;
	static void PWM_Init()
	{
		period = 64000;
		TimHandle.Instance = TIM1;
		TimHandle.Init.Period            = period-1;
		TimHandle.Init.Prescaler         = SystemCoreClock / 3200000 - 1;
		TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
		TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
		TimHandle.Init.RepetitionCounter = 0;
		if(HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
		{
			// error
			err = 1;
		}
	}

	static void SetPulseWidth(float pulse_width)
	{


		/*##-2- Configure the PWM channels #########################################*/
		/* Common configuration for all channels */
		sConfig.OCMode      = TIM_OCMODE_PWM2;
		sConfig.OCFastMode  = TIM_OCFAST_DISABLE;
		sConfig.OCPolarity  = TIM_OCPOLARITY_LOW;
		sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
		sConfig.OCIdleState = TIM_OCIDLESTATE_SET;
		sConfig.OCNIdleState= TIM_OCNIDLESTATE_RESET;
		sConfig.Pulse =  (period - 1) * pulse_width;

		if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
		{
			// error
			err = 1;
		}

		if(HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
		{
			/* Starting Error */
			err = 1;
		}
	}

	void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
	{
		  __HAL_RCC_TIM1_CLK_ENABLE();
		  __HAL_RCC_GPIOA_CLK_ENABLE();

		  GPIO_InitTypeDef   GPIO_InitStruct;

		  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		  GPIO_InitStruct.Pull = GPIO_PULLUP;
		  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
		  GPIO_InitStruct.Pin = GPIO_PIN_8;

		  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}
