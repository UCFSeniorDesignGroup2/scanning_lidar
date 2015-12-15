/*
 * hal_gpio3.cpp
 *
 *  Created on: Dec 10, 2015
 *      Author: jack
 */




/*
 * hal_gpio1.cpp
 *
 *  Created on: Dec 5, 2015
 *      Author: jack
 */




#include "hal_gpio3.h"
#include <stm32f4xx.h>

using namespace HAL;

// instance of HAL_GPIO3
HAL_GPIO3 HAL_GPIO3::mInstance;

HAL_GPIO3::HAL_GPIO3()
{
	mInitialized = false;
}

// initialize hardware
int HAL_GPIO3::Initialize()
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef gpio;
	gpio.Pin = GPIO_PIN_3;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &gpio);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	mInitialized = true;
	return 0;
}

// set the pin state
void HAL_GPIO3::SetPinState(PIN_STATE state)
{
	if(state == LOW)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
	}
}

// pin is an output.
// reading this pin does nothing
HAL_GPIO_PIN::PIN_STATE HAL_GPIO3::ReadPinState()
{
	return LOW;
}

HAL_GPIO_PIN* HAL_GPIO3::GetInstance()
{
	return &mInstance;
}
