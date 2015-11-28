#include "i2c_hal_conf.h"

#include <oal_semaphore.h>

static OAL::Semaphore i2c1_tx_semaphore(1,0);
static OAL::Semaphore i2c1_rx_semaphore(1,0);
static OAL::Semaphore i2c2_tx_semaphore(1,0);
static OAL::Semaphore i2c2_rx_semaphore(1,0);

extern "C"
{
	#include <stm32f4xx.h>

	static I2C_HandleTypeDef I2C1Handle;
	static I2C_HandleTypeDef I2C2Handle;

	static DMA_HandleTypeDef hdma_i2c1_tx;
	static DMA_HandleTypeDef hdma_i2c1_rx;
	static DMA_HandleTypeDef hdma_i2c2_tx;
	static DMA_HandleTypeDef hdma_i2c2_rx;

	void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
	{
		if(hi2c == &I2C1Handle)
		{
			i2c1_tx_semaphore.Give();
		}
		else if(hi2c == &I2C2Handle)
		{
			i2c2_tx_semaphore.Give();
		}
	}

	void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
	{
		if(hi2c == &I2C1Handle)
		{
			i2c1_rx_semaphore.Give();
		}
		else if(hi2c == &I2C2Handle)
		{
			i2c2_rx_semaphore.Give();
		}
	}

	void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
	{

		if(hi2c == &I2C1Handle)
		{
			  GPIO_InitTypeDef  GPIO_InitStruct;

			  /*##-1- Enable peripherals and GPIO Clocks #################################*/
			  /* Enable GPIO TX/RX clock */
			  __HAL_RCC_GPIOB_CLK_ENABLE();
			  /* Enable I2C1 clock */
			  __HAL_RCC_I2C1_CLK_ENABLE();
			  /* Enable DMA2 clock */
			  __HAL_RCC_DMA1_CLK_ENABLE();

			  /*##-2- Configure peripheral GPIO ##########################################*/
			  /* I2C TX GPIO pin configuration  */
			  GPIO_InitStruct.Pin       = GPIO_PIN_8;
			  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
			  GPIO_InitStruct.Pull      = GPIO_NOPULL;
			  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
			  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;

			  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

			  /* I2C RX GPIO pin configuration  */
			  GPIO_InitStruct.Pin = GPIO_PIN_9;
			  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;

			  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

			  /*##-3- Configure the DMA streams ##########################################*/
			  /* Configure the DMA handler for Transmission process */
			  hdma_i2c1_tx.Instance                 = DMA1_Stream6;

			  hdma_i2c1_tx.Init.Channel             = DMA_CHANNEL_1;
			  hdma_i2c1_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
			  hdma_i2c1_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
			  hdma_i2c1_tx.Init.MemInc              = DMA_MINC_ENABLE;
			  hdma_i2c1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
			  hdma_i2c1_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
			  hdma_i2c1_tx.Init.Mode                = DMA_NORMAL;
			  hdma_i2c1_tx.Init.Priority            = DMA_PRIORITY_LOW;
			  hdma_i2c1_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
			  hdma_i2c1_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
			  hdma_i2c1_tx.Init.MemBurst            = DMA_MBURST_INC4;
			  hdma_i2c1_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

			  HAL_DMA_Init(&hdma_i2c1_tx);

			  /* Associate the initialized DMA handle to the the I2C handle */
			  __HAL_LINKDMA(hi2c, hdmatx, hdma_i2c1_tx);

			  /* Configure the DMA handler for Transmission process */
			  hdma_i2c1_rx.Instance                 = DMA1_Stream5;

			  hdma_i2c1_rx.Init.Channel             = DMA_CHANNEL_1;
			  hdma_i2c1_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
			  hdma_i2c1_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
			  hdma_i2c1_rx.Init.MemInc              = DMA_MINC_ENABLE;
			  hdma_i2c1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
			  hdma_i2c1_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
			  hdma_i2c1_rx.Init.Mode                = DMA_NORMAL;
			  hdma_i2c1_rx.Init.Priority            = DMA_PRIORITY_HIGH;
			  hdma_i2c1_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
			  hdma_i2c1_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
			  hdma_i2c1_rx.Init.MemBurst            = DMA_MBURST_INC4;
			  hdma_i2c1_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

			  HAL_DMA_Init(&hdma_i2c1_rx);

			  /* Associate the initialized DMA handle to the the I2C handle */
			  __HAL_LINKDMA(hi2c, hdmarx, hdma_i2c1_rx);

			  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

			  /*##-4- Configure the NVIC for DMA #########################################*/
			  /* NVIC configuration for DMA transfer complete interrupt (I2C1_TX) */
			  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 15, 0);
			  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

			  /* NVIC configuration for DMA transfer complete interrupt (I2C1_RX) */
			  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 15, 0);
			  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
		}
		else if(hi2c == &I2C2Handle)
		{
			  GPIO_InitTypeDef  GPIO_InitStruct;

			  /*##-1- Enable peripherals and GPIO Clocks #################################*/
			  /* Enable GPIO TX/RX clock */
			  __HAL_RCC_GPIOB_CLK_ENABLE();
			  /* Enable I2C1 clock */
			  __HAL_RCC_I2C2_CLK_ENABLE();
			  /* Enable DMA2 clock */
			  __HAL_RCC_DMA1_CLK_ENABLE();

			  /*##-2- Configure peripheral GPIO ##########################################*/
			  /* I2C TX GPIO pin configuration  */
			  GPIO_InitStruct.Pin       = GPIO_PIN_10;
			  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
			  GPIO_InitStruct.Pull      = GPIO_NOPULL;
			  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
			  GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;

			  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

			  /* I2C RX GPIO pin configuration  */
			  GPIO_InitStruct.Pin = GPIO_PIN_11;
			  GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;

			  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

			  /*##-3- Configure the DMA streams ##########################################*/
			  /* Configure the DMA handler for Transmission process */
			  hdma_i2c2_tx.Instance                 = DMA1_Stream7;

			  hdma_i2c2_tx.Init.Channel             = DMA_CHANNEL_7;
			  hdma_i2c2_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
			  hdma_i2c2_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
			  hdma_i2c2_tx.Init.MemInc              = DMA_MINC_ENABLE;
			  hdma_i2c2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
			  hdma_i2c2_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
			  hdma_i2c2_tx.Init.Mode                = DMA_NORMAL;
			  hdma_i2c2_tx.Init.Priority            = DMA_PRIORITY_LOW;
			  hdma_i2c2_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
			  hdma_i2c2_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
			  hdma_i2c2_tx.Init.MemBurst            = DMA_MBURST_INC4;
			  hdma_i2c2_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

			  HAL_DMA_Init(&hdma_i2c2_tx);

			  /* Associate the initialized DMA handle to the the I2C handle */
			  __HAL_LINKDMA(hi2c, hdmatx, hdma_i2c2_tx);

			  /* Configure the DMA handler for Transmission process */
			  hdma_i2c2_rx.Instance                 = DMA1_Stream3;

			  hdma_i2c2_rx.Init.Channel             = DMA_CHANNEL_7;
			  hdma_i2c2_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
			  hdma_i2c2_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
			  hdma_i2c2_rx.Init.MemInc              = DMA_MINC_ENABLE;
			  hdma_i2c2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
			  hdma_i2c2_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
			  hdma_i2c2_rx.Init.Mode                = DMA_NORMAL;
			  hdma_i2c2_rx.Init.Priority            = DMA_PRIORITY_HIGH;
			  hdma_i2c2_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
			  hdma_i2c2_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
			  hdma_i2c2_rx.Init.MemBurst            = DMA_MBURST_INC4;
			  hdma_i2c2_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

			  HAL_DMA_Init(&hdma_i2c2_rx);

			  /* Associate the initialized DMA handle to the the I2C handle */
			  __HAL_LINKDMA(hi2c, hdmarx, hdma_i2c2_rx);

			  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

			  /*##-4- Configure the NVIC for DMA #########################################*/
			  /* NVIC configuration for DMA transfer complete interrupt (I2C1_TX) */
			  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 15, 0);
			  HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);

			  /* NVIC configuration for DMA transfer complete interrupt (I2C1_RX) */
			  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 15, 0);
			  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
		}
	}

	void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
	{
		if(hi2c == &I2C1Handle)
		{

			  /*##-1- Reset peripherals ##################################################*/
			  __HAL_RCC_I2C1_FORCE_RESET();
			  __HAL_RCC_I2C1_RELEASE_RESET();

			  /*##-2- Disable peripherals and GPIO Clocks ################################*/
			  /* Configure I2C Tx as alternate function  */
			  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);
			  /* Configure I2C Rx as alternate function  */
			  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

			  /*##-3- Disable the DMA Streams ############################################*/
			  /* De-Initialize the DMA Stream associate to transmission process */
			  HAL_DMA_DeInit(&hdma_i2c1_tx);
			  /* De-Initialize the DMA Stream associate to reception process */
			  HAL_DMA_DeInit(&hdma_i2c1_rx);

			  /*##-4- Disable the NVIC for DMA ###########################################*/
			  HAL_NVIC_DisableIRQ(DMA1_Stream6_IRQn);
			  HAL_NVIC_DisableIRQ(DMA1_Stream5_IRQn);
		}
		else if(hi2c == &I2C2Handle)
		{
			  /*##-1- Reset peripherals ##################################################*/
			  __HAL_RCC_I2C2_FORCE_RESET();
			  __HAL_RCC_I2C2_RELEASE_RESET();

			  /*##-2- Disable peripherals and GPIO Clocks ################################*/
			  /* Configure I2C Tx as alternate function  */
			  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);
			  /* Configure I2C Rx as alternate function  */
			  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);

			  /*##-3- Disable the DMA Streams ############################################*/
			  /* De-Initialize the DMA Stream associate to transmission process */
			  HAL_DMA_DeInit(&hdma_i2c2_tx);
			  /* De-Initialize the DMA Stream associate to reception process */
			  HAL_DMA_DeInit(&hdma_i2c2_rx);

			  /*##-4- Disable the NVIC for DMA ###########################################*/
			  HAL_NVIC_DisableIRQ(DMA1_Stream7_IRQn);
			  HAL_NVIC_DisableIRQ(DMA1_Stream3_IRQn);
		}
	}

	void DMA1_Stream7_IRQHandler(void)
	{
		  HAL_DMA_IRQHandler(I2C2Handle.hdmatx);
	}

	void DMA1_Stream3_IRQHandler(void)
	{
		  HAL_DMA_IRQHandler(I2C2Handle.hdmarx);
	}

	void DMA1_Stream6_IRQHandler(void)
	{
		  HAL_DMA_IRQHandler(I2C1Handle.hdmatx);
	}

	void DMA1_Stream5_IRQHandler(void)
	{
		  HAL_DMA_IRQHandler(I2C1Handle.hdmarx);
	}


}

int32_t HAL::I2C1_Initialize_Hardware()
{
	I2C1Handle.Instance             = I2C1;
	I2C1Handle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	I2C1Handle.Init.ClockSpeed      = 400000;
	I2C1Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2C1Handle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
	I2C1Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2C1Handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	I2C1Handle.Init.OwnAddress1     = 0;
	I2C1Handle.Init.OwnAddress2     = 0xFE;


	HAL_I2C_DeInit(&I2C1Handle);
	if(HAL_I2C_Init(&I2C1Handle) != HAL_OK)
	{
		return -1;
	}



	return 0;
}

int32_t HAL::I2C2_Initialize_Hardware()
{
	I2C2Handle.Instance             = I2C2;
	I2C2Handle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	I2C2Handle.Init.ClockSpeed      = 400000;
	I2C2Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2C2Handle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
	I2C2Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2C2Handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	I2C2Handle.Init.OwnAddress1     = 0;
	I2C2Handle.Init.OwnAddress2     = 0xFE;


	HAL_I2C_DeInit(&I2C2Handle);
	if(HAL_I2C_Init(&I2C2Handle) != HAL_OK)
	{
		return -1;
	}


	return 0;
}

int32_t HAL::I2C1_WriteData(uint8_t addr, const uint8_t* data, uint32_t length)
{
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_DMA(&I2C1Handle, (uint16_t)addr, (uint8_t*)data, (uint16_t)length);
	i2c1_tx_semaphore.Take();
	if(status == HAL_OK)
	{
		return 0;
	}
	return -1;
}

int32_t HAL::I2C2_WriteData(uint8_t addr, const uint8_t* data, uint32_t length)
{
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_DMA(&I2C2Handle, (uint16_t)addr, (uint8_t*)data, (uint16_t)length);
	i2c2_tx_semaphore.Take();
	if(status == HAL_OK)
	{
		return 0;
	}
	return -1;
}

int32_t HAL::I2C1_ReadData(uint8_t addr, uint8_t* data, uint32_t length)
{
	HAL_StatusTypeDef status = HAL_I2C_Master_Receive_DMA(&I2C1Handle, (uint16_t)addr, (uint8_t*)data, (uint16_t)length);
	i2c1_rx_semaphore.Take();
	if(status == HAL_OK)
	{
		return 0;
	}
	return -1;
}

int32_t HAL::I2C2_ReadData(uint8_t addr, uint8_t* data, uint32_t length)
{
	HAL_StatusTypeDef status = HAL_I2C_Master_Receive_DMA(&I2C2Handle, (uint16_t)addr, (uint8_t*)data, (uint16_t)length);
	i2c2_rx_semaphore.Take();
	if(status == HAL_OK)
	{
		return 0;
	}
	return -1;
}
