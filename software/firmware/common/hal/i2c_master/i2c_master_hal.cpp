#include "i2c3_master/i2c3_master.h"

using namespace HAL;

extern "C"
{
	#include <stm32f4xx.h>

	static DMA_HandleTypeDef hdma_i2c3_tx;
	static DMA_HandleTypeDef hdma_i2c3_rx;

	void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
	{
		if(hi2c->Instance == I2C3)
		{
			  GPIO_InitTypeDef  GPIO_InitStruct;

			  /*##-1- Enable peripherals and GPIO Clocks #################################*/
			  /* Enable GPIO TX/RX clock */
			  __HAL_RCC_GPIOA_CLK_ENABLE();
			  __HAL_RCC_GPIOC_CLK_ENABLE();

			  /*##-2- Configure peripheral GPIO ##########################################*/
			  /* I2C TX GPIO pin configuration  */
			  GPIO_InitStruct.Pin       = GPIO_PIN_8;
			  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
			  GPIO_InitStruct.Pull      = GPIO_NOPULL;
			  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
			  GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;

			  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

			  /* I2C RX GPIO pin configuration  */
			  GPIO_InitStruct.Pin = GPIO_PIN_9;
			  GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;

			  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

			  /* Enable DMA2 clock */
			  __HAL_RCC_DMA1_CLK_ENABLE();

			  /*##-3- Configure the DMA streams ##########################################*/
			  /* Configure the DMA handler for Transmission process */
			  hdma_i2c3_tx.Instance                 = DMA1_Stream4;

			  hdma_i2c3_tx.Init.Channel             = DMA_CHANNEL_3;
			  hdma_i2c3_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
			  hdma_i2c3_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
			  hdma_i2c3_tx.Init.MemInc              = DMA_MINC_ENABLE;
			  hdma_i2c3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
			  hdma_i2c3_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
			  hdma_i2c3_tx.Init.Mode                = DMA_NORMAL;
			  hdma_i2c3_tx.Init.Priority            = DMA_PRIORITY_LOW;
			  hdma_i2c3_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
			  hdma_i2c3_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
			  hdma_i2c3_tx.Init.MemBurst            = DMA_MBURST_INC4;
			  hdma_i2c3_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

			  HAL_DMA_Init(&hdma_i2c3_tx);

			  /* Associate the initialized DMA handle to the the I2C handle */
			  __HAL_LINKDMA(hi2c, hdmatx, hdma_i2c3_tx);

			  /* Configure the DMA handler for Transmission process */
			  hdma_i2c3_rx.Instance                 = DMA1_Stream2;

			  hdma_i2c3_rx.Init.Channel             = DMA_CHANNEL_3;
			  hdma_i2c3_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
			  hdma_i2c3_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
			  hdma_i2c3_rx.Init.MemInc              = DMA_MINC_ENABLE;
			  hdma_i2c3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
			  hdma_i2c3_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
			  hdma_i2c3_rx.Init.Mode                = DMA_NORMAL;
			  hdma_i2c3_rx.Init.Priority            = DMA_PRIORITY_HIGH;
			  hdma_i2c3_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
			  hdma_i2c3_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
			  hdma_i2c3_rx.Init.MemBurst            = DMA_MBURST_INC4;
			  hdma_i2c3_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

			  HAL_DMA_Init(&hdma_i2c3_rx);

			  /* Associate the initialized DMA handle to the the I2C handle */
			  __HAL_LINKDMA(hi2c, hdmarx, hdma_i2c3_rx);

			  /* Enable I2C3 clock */
			  __HAL_RCC_I2C3_CLK_ENABLE();


			  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

			  /*##-4- Configure the NVIC for DMA #########################################*/
			  /* NVIC configuration for DMA transfer complete interrupt (I2C1_TX) */
			  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 15, 0);
			  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

			  /* NVIC configuration for DMA transfer complete interrupt (I2C1_RX) */
			  HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 15, 0);
			  HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
		}
	}

	void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
	{
		if(hi2c->Instance == I2C3)
		{

			  /*##-1- Reset peripherals ##################################################*/
			  __HAL_RCC_I2C3_FORCE_RESET();
			  __HAL_RCC_I2C3_RELEASE_RESET();

			  /*##-2- Disable peripherals and GPIO Clocks ################################*/
			  /* Configure I2C Tx as alternate function  */
			  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);
			  /* Configure I2C Rx as alternate function  */
			  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_9);

			  /*##-3- Disable the DMA Streams ############################################*/
			  /* De-Initialize the DMA Stream associate to transmission process */
			  HAL_DMA_DeInit(&hdma_i2c3_tx);
			  /* De-Initialize the DMA Stream associate to reception process */
			  HAL_DMA_DeInit(&hdma_i2c3_rx);

			  /*##-4- Disable the NVIC for DMA ###########################################*/
			  HAL_NVIC_DisableIRQ(DMA1_Stream4_IRQn);
			  HAL_NVIC_DisableIRQ(DMA1_Stream2_IRQn);
		}
	}

	void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
	{
		if(hi2c->Instance == I2C3)
		{
			I2C3_MASTER::GetInstance()->UnblockCallingThread();
		}
	}

	void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
	{
		if(hi2c->Instance == I2C3)
		{
			I2C3_MASTER::GetInstance()->UnblockCallingThread();
		}
	}



	void DMA1_Stream4_IRQHandler(void)
	{

		  HAL_DMA_IRQHandler(&hdma_i2c3_tx);
	}

	void DMA1_Stream2_IRQHandler(void)
	{

		  HAL_DMA_IRQHandler(&hdma_i2c3_rx);
	}
}
