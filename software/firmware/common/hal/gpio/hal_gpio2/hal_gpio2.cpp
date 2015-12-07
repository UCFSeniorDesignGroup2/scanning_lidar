#include <hal/gpio/hal_gpio2/hal_gpio2.h>

#include <stm32f4xx.h>

using namespace HAL;

// instance of HAL_GPIO2
HAL_GPIO2 HAL_GPIO2::mInstance;


HAL_GPIO2::HAL_GPIO2()
{
	mInitialized = false;
}

// initailize hardware
int HAL_GPIO2::Initialize()
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	/* Enable GPIOG clock */
	__HAL_RCC_GPIOE_CLK_ENABLE();

	/* Configure PG15 pin as input floating */
	GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/* Enable and set EXTI15_10 Interrupt to the lowest priority */
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	mInitialized = true;

	return 0;
}

// do nothing.
// this pin is an input pin
void HAL_GPIO2::SetPinState(PIN_STATE state)
{
	(void)state;
}

// do nothing.
// this pin generates interrupts on a falling edge of PE6
// use case does not require checking the pin state.
HAL_GPIO_PIN::PIN_STATE HAL_GPIO2::ReadPinState()
{
	return PIN_STATE::LOW;
}

// on falling edge process the interrupt
void HAL_GPIO2::ProcessISR()
{
	if(mOnFalling != 0)
		mOnFalling();
}

// return instance of HAL_GPIO2
HAL_GPIO_PIN* HAL_GPIO2::GetInstance()
{
	return &mInstance;
}

extern "C"
{
	/**
	  * @brief EXTI line detection callbacks
	  * @param GPIO_Pin: Specifies the pins connected EXTI line
	  * @retval None
	  */
	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
	{
	  if(GPIO_Pin == GPIO_PIN_10)
	  {
		  ((HAL_GPIO2*)HAL_GPIO2::GetInstance())->ProcessISR();
	  }
	}

	/**
	  * @brief  This function handles External lines 15 to 10 interrupt request.
	  * @param  None
	  * @retval None
	  */
	void EXTI15_10_IRQHandler(void)
	{
	  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
	}


}
