#include <hal/hal_spi/hal_spi1/hal_spi1.h>

#include <stm32f4xx.h>

using namespace HAL;


/* Private variables ---------------------------------------------------------*/
/* SPI handler declaration */
static SPI_HandleTypeDef SpiHandle;

HAL_SPI1 HAL_SPI1::mInstance;

HAL_SPI1::HAL_SPI1() :
		mTxRxSemaphore(1,0)
{

}

int32_t HAL_SPI1::Initialize()
{
	SpiHandle.Instance               = SPI4;
	SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
	SpiHandle.Init.CLKPhase          = SPI_PHASE_2EDGE;
	SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
	SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	SpiHandle.Init.CRCPolynomial     = 7;
	SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
	SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	SpiHandle.Init.NSS               = SPI_NSS_SOFT;
	SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
	SpiHandle.Init.Mode = SPI_MODE_MASTER;
	if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
	{
		/* Initialization Error */
		return -1;
	}
	return 0;
}

int32_t HAL_SPI1::TransferData(const uint8_t* data_out, uint8_t* data_in, uint32_t length)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
	if(HAL_SPI_TransmitReceive_DMA(&SpiHandle, (uint8_t*)data_out, (uint8_t *)data_in, length) != HAL_OK)
	{
		return -1;
	}
	mTxRxSemaphore.Take();
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
	return 0;
}

void HAL_SPI1::TransferComplete()
{
	mTxRxSemaphore.Give();
}

HAL_SPI* HAL_SPI1::GetInstance()
{
	return &mInstance;
}

extern "C"
{
	/**
	  * @brief  TxRx Transfer completed callback.
	  * @param  hspi: SPI handle.
	  * @note   This example shows a simple way to report end of DMA TxRx transfer, and
	  *         you can add your own implementation.
	  * @retval None
	  */
	void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
	{
		if(hspi->Instance == SPI4)
			((HAL_SPI1*)HAL_SPI1::GetInstance())->TransferComplete();
	}

	/** @defgroup HAL_MSP_Private_Functions
	 * @{
	 */

	/**
	 * @brief SPI MSP Initialization
	 *        This function configures the hardware resources used in this example:
	 *           - Peripheral's clock enable
	 *           - Peripheral's GPIO Configuration
	 *           - DMA configuration for transmission request by peripheral
	 *           - NVIC configuration for DMA interrupt request enable
	 * @param hspi: SPI handle pointer
	 * @retval None
	 */
	void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
	{
		static DMA_HandleTypeDef hdma_tx;
		static DMA_HandleTypeDef hdma_rx;

		GPIO_InitTypeDef  GPIO_InitStruct;

		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		__HAL_RCC_GPIOE_CLK_ENABLE();

		/* Enable SPI3 clock */
		__HAL_RCC_SPI4_CLK_ENABLE();
		/* Enable DMA1 clock */
		__HAL_RCC_DMA2_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/
		/* SPI SCK GPIO pin configuration  */
		GPIO_InitStruct.Pin       = GPIO_PIN_2;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;

		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

		/* SPI MISO GPIO pin configuration  */
		GPIO_InitStruct.Pin = GPIO_PIN_5;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;

		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

		/* SPI MOSI GPIO pin configuration  */
		GPIO_InitStruct.Pin = GPIO_PIN_6;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;

		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

		/* SPI CHIP SELECT */
		GPIO_InitStruct.Pin = GPIO_PIN_4;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);


		/*##-3- Configure the DMA streams ##########################################*/
		/* Configure the DMA handler for Transmission process */
		hdma_tx.Instance                 = DMA2_Stream1;

		hdma_tx.Init.Channel             = DMA_CHANNEL_4;
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

		/* Associate the initialized DMA handle to the the SPI handle */
		__HAL_LINKDMA(hspi, hdmatx, hdma_tx);

		/* Configure the DMA handler for Transmission process */
		hdma_rx.Instance                 = DMA2_Stream0;

		hdma_rx.Init.Channel             = DMA_CHANNEL_4;
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

		/* Associate the initialized DMA handle to the the SPI handle */
		__HAL_LINKDMA(hspi, hdmarx, hdma_rx);

		HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (SPI3_TX) */
		HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 15, 0);
		HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

		/* NVIC configuration for DMA transfer complete interrupt (SPI3_RX) */
		HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 15, 0);
		HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	}

	/**
	 * @brief SPI MSP De-Initialization
	 *        This function frees the hardware resources used in this example:
	 *          - Disable the Peripheral's clock
	 *          - Revert GPIO, DMA and NVIC configuration to their default state
	 * @param hspi: SPI handle pointer
	 * @retval None
	 */
	void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
	{

		static DMA_HandleTypeDef hdma_tx;
		static DMA_HandleTypeDef hdma_rx;

		/*##-1- Reset peripherals ##################################################*/
		__HAL_RCC_SPI4_FORCE_RESET();
		__HAL_RCC_SPI4_RELEASE_RESET();

		/*##-2- Disable peripherals and GPIO Clocks ################################*/
		/* Configure SPI SCK as alternate function  */
		HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2);
		/* Configure SPI MISO as alternate function  */
		HAL_GPIO_DeInit(GPIOE, GPIO_PIN_5);
		/* Configure SPI MOSI as alternate function  */
		HAL_GPIO_DeInit(GPIOE, GPIO_PIN_6);

		/*##-3- Disable the DMA Streams ############################################*/
		/* De-Initialize the DMA Stream associate to transmission process */
		HAL_DMA_DeInit(&hdma_tx);
		/* De-Initialize the DMA Stream associate to reception process */
		HAL_DMA_DeInit(&hdma_rx);

		/*##-4- Disable the NVIC for DMA ###########################################*/
		HAL_NVIC_DisableIRQ(DMA2_Stream1_IRQn);
		HAL_NVIC_DisableIRQ(DMA2_Stream0_IRQn);
	}

	/**
	  * @brief  This function handles DMA Rx interrupt request.
	  * @param  None
	  * @retval None
	  */
	void DMA2_Stream0_IRQHandler(void)
	{
	  HAL_DMA_IRQHandler(SpiHandle.hdmarx);
	}

	/**
	  * @brief  This function handles DMA Tx interrupt request.
	  * @param  None
	  * @retval None
	  */
	void DMA2_Stream1_IRQHandler(void)
	{
	  HAL_DMA_IRQHandler(SpiHandle.hdmatx);
	}

}
