#include <i2c/stm32f4xx/i2c1_master/i2c1_master.h>
#include <oal_thread.h>

using namespace HAL;

extern "C"
{
	#include <stm32f4xx.h>

	I2C_HandleTypeDef I2cHandle;
	static DMA_HandleTypeDef hdma_tx;
	static DMA_HandleTypeDef hdma_rx;

	void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
	{
		(void)hi2c;
		I2C1_MASTER::GetInstance()->UnblockTxThread();
	}
	void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
	{
		(void)hi2c;
		I2C1_MASTER::GetInstance()->UnblockRxThread();
	}
	void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
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
	  hdma_tx.Instance                 = DMA1_Stream6;

	  hdma_tx.Init.Channel             = DMA_CHANNEL_1;
	  hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	  hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	  hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
	  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	  hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	  hdma_tx.Init.Mode                = DMA_NORMAL;
	  hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
	  hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	  hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	  hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
	  hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

	  HAL_DMA_Init(&hdma_tx);

	  /* Associate the initialized DMA handle to the the I2C handle */
	  __HAL_LINKDMA(hi2c, hdmatx, hdma_tx);

	  /* Configure the DMA handler for Transmission process */
	  hdma_rx.Instance                 = DMA1_Stream5;

	  hdma_rx.Init.Channel             = DMA_CHANNEL_1;
	  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
	  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
	  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	  hdma_rx.Init.Mode                = DMA_NORMAL;
	  hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
	  hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	  hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	  hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
	  hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

	  HAL_DMA_Init(&hdma_rx);

	  /* Associate the initialized DMA handle to the the I2C handle */
	  __HAL_LINKDMA(hi2c, hdmarx, hdma_rx);

	  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	  /*##-4- Configure the NVIC for DMA #########################################*/
	  /* NVIC configuration for DMA transfer complete interrupt (I2C1_TX) */
	  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 15, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

	  /* NVIC configuration for DMA transfer complete interrupt (I2C1_RX) */
	  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 15, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	}

	/**
	  * @brief I2C MSP De-Initialization
	  *        This function frees the hardware resources used in this example:
	  *          - Disable the Peripheral's clock
	  *          - Revert GPIO, DMA and NVIC configuration to their default state
	  * @param hi2c: I2C handle pointer
	  * @retval None
	  */
	void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
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
	  HAL_DMA_DeInit(&hdma_tx);
	  /* De-Initialize the DMA Stream associate to reception process */
	  HAL_DMA_DeInit(&hdma_rx);

	  /*##-4- Disable the NVIC for DMA ###########################################*/
	  HAL_NVIC_DisableIRQ(DMA1_Stream6_IRQn);
	  HAL_NVIC_DisableIRQ(DMA1_Stream5_IRQn);
	}
	void DMA1_Stream6_IRQHandler(void)
	{
	  HAL_DMA_IRQHandler(I2cHandle.hdmatx);
	}
	void DMA1_Stream5_IRQHandler(void)
	{
	  HAL_DMA_IRQHandler(I2cHandle.hdmarx);
	}

}



I2C1_MASTER I2C1_MASTER::mInstance;

I2C1_MASTER::I2C1_MASTER() :
	mTxSemaphore(1,0), mRxSemaphore(1,0)
{
}

int32_t I2C1_MASTER::Initialize()
{
	static DMA_HandleTypeDef hdma_tx;
	static DMA_HandleTypeDef hdma_rx;

	GPIO_InitTypeDef  GPIO_InitStruct;


	I2cHandle.Instance             = I2C1;
	I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	I2cHandle.Init.ClockSpeed      = 400000;
	I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
	I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	I2cHandle.Init.OwnAddress1     = 0;
	I2cHandle.Init.OwnAddress2     = 0xFE;



	HAL_I2C_DeInit(&I2cHandle);
	if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
	{
		return -1;
	}



	return 0;
}

int32_t I2C1_MASTER::WriteData(uint8_t addr, const uint8_t* data, uint32_t length)
{
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_DMA(&I2cHandle, (uint16_t)addr, (uint8_t*)data, (uint16_t)length);
	BlockTxThread();
	if(status == HAL_OK)
	{
		return 0;
	}
	return -1;
}

int32_t I2C1_MASTER::ReadData(uint8_t addr, uint8_t* data, uint32_t length)
{
	HAL_StatusTypeDef status = HAL_I2C_Master_Receive_DMA(&I2cHandle, (uint16_t)addr, (uint8_t*)data, (uint16_t)length);
	BlockRxThread();
	if(status == HAL_OK)
	{
		return 0;
	}
	return -1;
}

void I2C1_MASTER::BlockTxThread()
{
	mTxSemaphore.Take();
}

void I2C1_MASTER::UnblockTxThread()
{
	mTxSemaphore.Give();
}

void I2C1_MASTER::BlockRxThread()
{
	mRxSemaphore.Take();
}

void I2C1_MASTER::UnblockRxThread()
{
	mRxSemaphore.Give();
}


I2C1_MASTER* I2C1_MASTER::GetInstance()
{
	return &mInstance;
}


