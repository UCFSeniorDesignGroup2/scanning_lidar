/*
 * sync_pulse.cpp
 *
 *  Created on: Dec 6, 2015
 *      Author: jack
 */


#include "sync_pulse1.h"
#include <stm32f4xx.h>

using namespace HAL;

SyncPulse1 SyncPulse1::mInstance;

static void ON_ISR();

SyncPulse1::SyncPulse1() :
		mSemaphore(1,0)
{
	mInitialized = false;
	mState = 0;
}

int32_t SyncPulse1::Initialize()
{
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef gpio;
	gpio.Pin = GPIO_PIN_11;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &gpio);
	GPIOC->BSRR = (uint32_t)GPIO_PIN_4 << 16; // TURN OFF
	mInitialized = true;
	return 0;
}

int32_t SyncPulse1::SetState(uint8_t state)
{
	mState = state;
	if(mInitialized == false)
	{
		return -1;
	}
	if(mExtClock == 0)
	{
		return -1;
	}
	mExtClock->SetInterrupt(ON_ISR);
	mSemaphore.Take();
	return 0;
}

void SyncPulse1::SyncState()
{
	if(mState == 1)
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);

	mSemaphore.Give();
}

SyncPulse* SyncPulse1::GetInstance()
{
	return &mInstance;
}


static void ON_ISR()
{
	((SyncPulse1*)SyncPulse1::GetInstance())->SyncState();
}
