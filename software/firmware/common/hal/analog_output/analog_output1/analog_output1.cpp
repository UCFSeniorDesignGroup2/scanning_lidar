#include <hal/analog_output/analog_output1/analog_output1.h>
#include <stm32f4xx.h>

using namespace HAL;

// instantiate instance of analogoutput1
AnalogOutput1 AnalogOutput1::mInstance;

// structures for controlling the hardware using stm32cubef4 library
static DAC_HandleTypeDef dac;
static DAC_ChannelConfTypeDef sConfig;

AnalogOutput1::AnalogOutput1()
{
	mInitialized = false;
}

// initialize the device
int32_t AnalogOutput1::Initialize()
{
	// nothing fancy just call HAL_DAC_Init
	dac.Instance = DAC;
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
	HAL_DAC_DeInit(&dac);
	if(HAL_DAC_Init(&dac) == HAL_OK)
	{
		// Configure with channel 1 (gpio pin 4)
		if(HAL_DAC_ConfigChannel(&dac, &sConfig, DAC_CHANNEL_1) == HAL_OK)
		{
			  /*##-2- Enable DAC Channel1 and associated DMA #############################*/
			  if(HAL_DAC_Start(&dac, DAC_CHANNEL_1) == HAL_OK)
			  {
				  mInitialized = true;
				  return 0;
			  }
		}
	}
	return -1;
}

int32_t AnalogOutput1::SetVoltage(float voltage)
{
	// set dac value
	if(mInitialized)
	{
		HAL_DAC_SetValue(&dac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)(voltage*4096/3.3));
		return 0;
	}
	else
	{
		return -1;
	}
}

// get referance to the analogoutput1 peripheral
AnalogOutput* AnalogOutput1::GetInstance()
{
	return &mInstance;
}

extern "C"
{
	// initialize hardware
	void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
	{
		  GPIO_InitTypeDef          GPIO_InitStruct;

		  /*##-1- Enable peripherals and GPIO Clocks #################################*/
		  /* DAC Periph clock enable */
		  __HAL_RCC_DAC_CLK_ENABLE();
		  /* Enable GPIO clock ****************************************/
		  __HAL_RCC_GPIOA_CLK_ENABLE();

		  /*##-2- Configure peripheral GPIO ##########################################*/
		  /* DAC Channel1 GPIO pin configuration */
		  GPIO_InitStruct.Pin = GPIO_PIN_4;
		  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	}
}
