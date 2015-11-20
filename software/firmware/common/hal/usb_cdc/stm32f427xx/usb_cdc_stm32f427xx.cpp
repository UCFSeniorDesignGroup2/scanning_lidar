#include "usb_cdc_stm32f427xx.h"
#include <oal/oal_thread.h>
#include <hal/usb_cdc/stm32f427xx/usbd_desc.h>

#include <usbd_core.h>
#include <usbd_cdc.h>


using namespace HAL;

extern "C"
{

	USBD_HandleTypeDef USBD_Device;
    extern PCD_HandleTypeDef hpcd;

	static int8_t Init     (void);
	static int8_t DeInit   (void);
	static int8_t Control  (uint8_t cmd, uint8_t* pbuf, uint16_t length);
	static int8_t Receive  (uint8_t* pbuf, uint32_t *Len);
	void TxFinish();

	USBD_CDC_ItfTypeDef USBD_CDC_fops =
	{
	  Init,
	  DeInit,
	  Control,
	  Receive,
	};

}

USB_CDC_STM32F427xx cdc_instance;

USB_CDC* USB_CDC::mInstance = &cdc_instance;


USB_CDC_STM32F427xx::USB_CDC_STM32F427xx() :
		mRxSemaphore(1,0), mTxSemaphore(1,0), mInitSemaphore(1,0)
{
	mRxHead = 0;
	mRxTail = 0;
	mState = USB_CDC_STM32F427xx::UNINITIALIZED;
}

USB_CDC_STM32F427xx::~USB_CDC_STM32F427xx()
{

}

// set up cdc device
int USB_CDC_STM32F427xx::Initialize()
{
    USBD_Init(&USBD_Device, &VCP_Desc, 0);

    USBD_RegisterClass(&USBD_Device, &USBD_CDC);
    USBD_CDC_RegisterInterface(&USBD_Device,
                               &USBD_CDC_fops);
    USBD_Start(&USBD_Device);

    mInitSemaphore.Take();
	return 0;
}
int USB_CDC_STM32F427xx::WriteDataRec(
				const unsigned char* buffer,
				unsigned int buffer_size)
{

	if(USBD_Device.dev_state != USBD_STATE_CONFIGURED || mState != INITIALIZED)
	{
		return -1;
	}

	if (buffer_size > CDC_DATA_FS_OUT_PACKET_SIZE)
    {
        int offset;
        for (offset = 0; offset < buffer_size; offset++)
        {
            int todo = MIN(CDC_DATA_FS_OUT_PACKET_SIZE,
            		buffer_size - offset);
            int done = WriteData(buffer + offset, todo);
            if (done != todo)
                return offset + done;
        }

        return buffer_size;
    }
	USBD_CDC_HandleTypeDef   *hcdc = (USBD_CDC_HandleTypeDef*) USBD_Device.pClassData;


//	if(hcdc->TxState == 1)
//		if(mTxSemaphore.TakeTimeout(10) < 0)
//			return -1;


	USBD_CDC_SetTxBuffer(&USBD_Device, (uint8_t*)buffer, (uint16_t)buffer_size);

	if(USBD_CDC_TransmitPacket(&USBD_Device) != USBD_OK)
	{
		return -1;
	}

//	if(hcdc->TxState == 1)
//		if(mTxSemaphore.TakeTimeout(10) < 0)
//			return -1;

	mTxSemaphore.Take();

	return buffer_size;
}
// writes `buffer_size` bytes over usb_cdc
// returns 0 if success
int USB_CDC_STM32F427xx::WriteData(
		const unsigned char* buffer,
		unsigned int buffer_size)
{
	mMutex.Lock();
	int ret = WriteDataRec(buffer, buffer_size);
	mMutex.Unlock();
	return ret;
}

// reads up to `buffer_size` bytes from usb_cdc buffer
// returns `number of bytes read` or -1
int USB_CDC_STM32F427xx::ReadData(
		unsigned char* buffer,
		unsigned int buffer_size)
{
	if(USBD_Device.dev_state != USBD_STATE_CONFIGURED || mState != INITIALIZED)
	{
		return -1;
	}
	USBD_CDC_ReceivePacket(&USBD_Device);
	mRxSemaphore.Take();

	return 0;
}

void USB_CDC_STM32F427xx::CDC_Init()
{
	USBD_CDC_SetRxBuffer(&USBD_Device, (uint8_t*)mRxBuffer);

	mState = USB_CDC_STM32F427xx::INITIALIZED;

	mInitSemaphore.Give();
}

void USB_CDC_STM32F427xx::OnRxData(uint8_t* data, uint16_t size)
{

	if(mRxHead <= mRxTail)
	{


	}
	else
	{

	}


	mRxSemaphore.Give();
}

void USB_CDC_STM32F427xx::OnTxFinish()
{
	mTxSemaphore.Give();
}
extern "C"
{
    void OTG_FS_IRQHandler(void)
    {
    	HAL_PCD_IRQHandler(&hpcd);
    }


    USBD_CDC_LineCodingTypeDef linecoding =
      {
        115200, /* baud rate*/
        0x00,   /* stop bits-1*/
        0x00,   /* parity - none*/
        0x08    /* nb. of bits 8*/
      };

    /* Private functions ---------------------------------------------------------*/

    /**
      * @brief  TEMPLATE_Init
      *         Initializes the CDC media low layer
      * @param  None
      * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
      */
    static int8_t Init(void)
    {
    	/*
         Add your initialization code here
    	 */
    	((USB_CDC_STM32F427xx*)USB_CDC_STM32F427xx::GetInstance())->CDC_Init();
    	return USBD_OK;
    }

    /**
      * @brief  TEMPLATE_DeInit
      *         DeInitializes the CDC media low layer
      * @param  None
      * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
      */
    static int8_t DeInit(void)
    {
    	/*
         Add your deinitialization code here
    	 */
    	return USBD_OK;
    }


    /**
      * @brief  TEMPLATE_Control
      *         Manage the CDC class requests
      * @param  Cmd: Command code
      * @param  Buf: Buffer containing command data (request parameters)
      * @param  Len: Number of data to be sent (in bytes)
      * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
      */
    static int8_t Control  (uint8_t cmd, uint8_t* pbuf, uint16_t length)
    {
      switch (cmd)
      {
      case CDC_SEND_ENCAPSULATED_COMMAND:
        /* Add your code here */
        break;

      case CDC_GET_ENCAPSULATED_RESPONSE:
        /* Add your code here */
        break;

      case CDC_SET_COMM_FEATURE:
        /* Add your code here */
        break;

      case CDC_GET_COMM_FEATURE:
        /* Add your code here */
        break;

      case CDC_CLEAR_COMM_FEATURE:
        /* Add your code here */
        break;

      case CDC_SET_LINE_CODING:
        linecoding.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |\
                                (pbuf[2] << 16) | (pbuf[3] << 24));
        linecoding.format     = pbuf[4];
        linecoding.paritytype = pbuf[5];
        linecoding.datatype   = pbuf[6];

        /* Add your code here */
        break;

      case CDC_GET_LINE_CODING:
        pbuf[0] = (uint8_t)(linecoding.bitrate);
        pbuf[1] = (uint8_t)(linecoding.bitrate >> 8);
        pbuf[2] = (uint8_t)(linecoding.bitrate >> 16);
        pbuf[3] = (uint8_t)(linecoding.bitrate >> 24);
        pbuf[4] = linecoding.format;
        pbuf[5] = linecoding.paritytype;
        pbuf[6] = linecoding.datatype;

        /* Add your code here */
        break;

      case CDC_SET_CONTROL_LINE_STATE:
        /* Add your code here */
        break;

      case CDC_SEND_BREAK:
         /* Add your code here */
        break;


      default:
        break;
      }

      return USBD_OK;
    }

    /**
      * @brief  TEMPLATE_Receive
      *         Data received over USB OUT endpoint are sent over CDC interface
      *         through this function.
      *
      *         @note
      *         This function will issue a NAK packet on any OUT packet received on
      *         USB endpoint untill exiting this function. If you exit this function
      *         before transfer is complete on CDC interface (ie. using DMA controller)
      *         it will result in receiving more data while previous ones are still
      *         not sent.
      *
      * @param  Buf: Buffer of data to be received
      * @param  Len: Number of data received (in bytes)
      * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
      */
    static int8_t Receive (uint8_t* Buf, uint32_t *Len)
    {
    	((USB_CDC_STM32F427xx*)USB_CDC_STM32F427xx::GetInstance())->OnRxData(Buf, *Len);
    	return USBD_OK;
    }

    void TxFinish()
	{
    	((USB_CDC_STM32F427xx*)USB_CDC_STM32F427xx::GetInstance())->OnTxFinish();
	}


}
