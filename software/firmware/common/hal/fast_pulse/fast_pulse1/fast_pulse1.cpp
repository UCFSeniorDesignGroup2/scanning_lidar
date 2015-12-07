/*
 * fast_pulse1.cpp
 *
 *  Created on: Dec 5, 2015
 *      Author: jack
 */


#include "fast_pulse1.h"

#include <FreeRTOS.h>
#include <stm32f4xx.h>


using namespace HAL;

FastPulse1 FastPulse1::mInstance;

FastPulse1::FastPulse1()
{
	mInitialized = false;
}

int32_t FastPulse1::Initialize()
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef gpio;
	gpio.Pin = GPIO_PIN_4;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &gpio);
	GPIOC->BSRR = (uint32_t)GPIO_PIN_4 << 16; // TURN OFF
	mInitialized = true;
	return 0;
}

int32_t FastPulse1::Pulse()
{
	if(mInitialized)
	{
		vPortEnterCritical();
		GPIOC->BSRR = GPIO_PIN_4; // TURN ON
		GPIOC->BSRR = (uint32_t)GPIO_PIN_4 << 16; // TURN OFF
		vPortExitCritical();
		return 0;
	}
	return -1;
}

FastPulse* FastPulse1::GetInstance()
{
	return &mInstance;
}

