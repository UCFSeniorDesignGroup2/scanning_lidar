/*
 * pwm.cpp
 *
 *  Created on: Dec 14, 2015
 *      Author: jack
 */

#include <stm32f4xx.h>


extern "C"
{

	// initialize hardware
	void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
	{
		if(htim->Instance == TIM3)
		{
		  __HAL_RCC_TIM3_CLK_ENABLE();
		  __HAL_RCC_GPIOC_CLK_ENABLE();

		  GPIO_InitTypeDef   GPIO_InitStruct;

		  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		  GPIO_InitStruct.Pull = GPIO_PULLUP;
		  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
		  GPIO_InitStruct.Pin = GPIO_PIN_9;

		  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		}
		else if(htim->Instance == TIM2)
		{
			  __HAL_RCC_TIM2_CLK_ENABLE();
			  __HAL_RCC_GPIOB_CLK_ENABLE();

			  GPIO_InitTypeDef   GPIO_InitStruct;

			  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
			  GPIO_InitStruct.Pull = GPIO_PULLUP;
			  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
			  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
			  GPIO_InitStruct.Pin = GPIO_PIN_10;

			  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		}
	}

}
