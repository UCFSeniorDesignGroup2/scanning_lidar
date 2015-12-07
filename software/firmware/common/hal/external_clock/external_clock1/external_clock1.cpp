#include "external_clock1.h"
#include <hal/sync_pulse/sync_pulse1/sync_pulse1.h>
#include <stm32f4xx.h>

using namespace HAL;

ExternalClock1 ExternalClock1::mInstance;

// structures for controlling hardware with stm32cubef4 library
static TIM_HandleTypeDef TimHandle;
static TIM_OC_InitTypeDef sConfig;

// tick period of the hardware
uint32_t period = 0;

ExternalClock1::ExternalClock1()
{
	mCb = NULL;
}

// initialize 16Mhz clock
// note: actual clock is about 16.8MHz.
//       this is closest possible value without changing
//       system clock.
int32_t ExternalClock1::Initialize()
{

	period = 5;
	TimHandle.Instance = TIM3;
	TimHandle.Init.Period            = period-1;
	TimHandle.Init.Prescaler         = 0;
	TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	TimHandle.Init.RepetitionCounter = 0;
	if(HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
	{
		return -1;
	}

	if(HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
	{
		return -1;
	}

	/*##-2- Configure the PWM channels #########################################*/
	/* Common configuration for all channels */
	sConfig.OCMode      = TIM_OCMODE_PWM2;
	sConfig.OCFastMode  = TIM_OCFAST_DISABLE;
	sConfig.OCPolarity  = TIM_OCPOLARITY_LOW;
	sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfig.OCIdleState = TIM_OCIDLESTATE_SET;
	sConfig.OCNIdleState= TIM_OCNIDLESTATE_RESET;
	sConfig.Pulse =  2;

	// output clock on channel 4
	if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_4) != HAL_OK)
	{
		return -1;
	}

	if(HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_4) != HAL_OK)
	{
		/* Starting Error */
		return -1;
	}
	return 0;
}

// allow for interrupts to be configured
void ExternalClock1::SetInterrupt(void (*cb)())
{
	mCb = cb;

	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/*##-2- Configure the NVIC for TIMx ########################################*/
	/* Set the TIMx priority */
	HAL_NVIC_SetPriority(TIM3_IRQn, 15, 0);

	/* Enable the TIMx global Interrupt */
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

// on interrupt:
//   timer overflow/reset
//   only fire interrupt 1 time then disable interrupts
void ExternalClock1::OnIsr()
{
	if(mCb != NULL)
		mCb();

	HAL_NVIC_DisableIRQ(TIM3_IRQn);
}

// return instance of ExtrernalClock1
ExternalClock* ExternalClock1::GetInstance()
{
	return &mInstance;
}

extern "C"
{
	/**
	  * @brief  Period elapsed callback in non blocking mode
	  * @param  htim: TIM handle
	  * @retval None
	  */
	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	{
		if(htim->Instance == TIM3)
			((ExternalClock1*)ExternalClock1::GetInstance())->OnIsr();
	}


	// initialize hardware
	void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
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

	/**
	  * @brief  This function handles TIM interrupt request.
	  * @param  None
	  * @retval None
	  */
	void TIM3_IRQHandler(void)
	{
	  HAL_TIM_IRQHandler(&TimHandle);
	}
}
