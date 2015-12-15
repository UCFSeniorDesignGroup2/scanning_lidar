/*
 * sync_pulse.cpp
 *
 *  Created on: Dec 6, 2015
 *      Author: jack
 */


#include "sync_pulse1.h"
#include <stm32f4xx.h>
#include <FreeRTOS.h>

using namespace HAL;

#define SEND_LASER_PULSE

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
	__HAL_RCC_GPIOE_CLK_ENABLE(); // tdc start pin
	GPIO_InitTypeDef gpio;
	gpio.Pin = GPIO_PIN_11;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &gpio);
	GPIOC->BSRR = (uint32_t)GPIO_PIN_4 << 16; // TURN OFF

	__HAL_RCC_GPIOC_CLK_ENABLE(); // laser pulse pin
//	GPIO_InitTypeDef gpio;
	gpio.Pin = GPIO_PIN_4;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &gpio);
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
	if(mSemaphore.TakeTimeout(1000) < 0)
	{
		return -1;
	}

	return 0;
}

void SyncPulse1::SyncState()
{
	if(mState == 1)
	{
#ifdef SEND_LASER_PULSE
		uint32_t uxSavedInterruptStatus;
		uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
//		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
		GPIOE->BSRR = GPIO_PIN_11; // TURN ON
		GPIOC->BSRR = GPIO_PIN_4; // TURN ON
		portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );
		GPIOC->BSRR = (uint32_t)GPIO_PIN_4 << 16; // TURN OFF
#endif
	}
	else
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
	}
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
