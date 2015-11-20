#include "usb_cdc_stm32f427xx.h"
#include "oal/oal_thread.h"
#include "hal/usb_cdc/stm32f427xx/usbd_desc.h"

#include <usbd_core.h>
#include <usbd_cdc.h>
#include <usbd_cdc_if_template.h>


using namespace HAL;

extern "C"
{
    extern PCD_HandleTypeDef hpcd;

    void OTG_FS_IRQHandler(void)
    {
    	((USB_CDC_STM32F427xx*)USB_CDC::GetInstance())->UnblockRxThread();
    	((USB_CDC_STM32F427xx*)USB_CDC::GetInstance())->UnblockTxThread();
    	HAL_PCD_IRQHandler(&hpcd);
    }
    void HAL_Delay(__IO uint32_t Delay)
    {
    	OAL::Thread::Sleep((unsigned long)Delay);
    }
    uint32_t HAL_GetTick(void)
    {
    	return OAL::Thread::GetTicks();
    }

    int VCP_write(const void *pBuffer, int size)
    {

    	if(USBD_Device.dev_state != USBD_STATE_CONFIGURED || hpcd.State != HAL_PCD_STATE_READY)
    	{
    		return -1;
    	}


    	if (size > CDC_DATA_FS_OUT_PACKET_SIZE)
        {
            int offset;
            for (offset = 0; offset < size; offset++)
            {
                int todo = MIN(CDC_DATA_FS_OUT_PACKET_SIZE,
                               size - offset);
                int done = VCP_write(((char *)pBuffer) + offset, todo);
                if (done != todo)
                    return offset + done;
            }

            return size;
        }

        USBD_CDC_HandleTypeDef *pCDC =
                (USBD_CDC_HandleTypeDef *)USBD_Device.pClassData;


        while(pCDC->TxState) {
        	((USB_CDC_STM32F427xx*)USB_CDC::GetInstance())->BlockTxThread(10);
        } //Wait for previous transfer

        USBD_CDC_SetTxBuffer(&USBD_Device, (uint8_t *)pBuffer, size);
        if (USBD_CDC_TransmitPacket(&USBD_Device) != USBD_OK)
            return 0;

        while(pCDC->TxState) {
        	((USB_CDC_STM32F427xx*)USB_CDC::GetInstance())->BlockTxThread(10);
        } //Wait until transfer is done
        return size;
    }
}


USB_CDC_STM32F427xx cdc_instance;

USB_CDC* USB_CDC::mInstance = &cdc_instance;


USB_CDC_STM32F427xx::USB_CDC_STM32F427xx() :
		mTxSemaphore(1,0), mRxSemaphore(1,0)
{
	mTxThreadBlocked = false;
	mRxThreadBlocked = false;
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
                               &USBD_CDC_Template_fops);
    USBD_Start(&USBD_Device);

	return 0;
}

// writes `buffer_size` bytes over usb_cdc
// returns 0 if success
int USB_CDC_STM32F427xx::WriteData(
		const unsigned char* buffer,
		unsigned int buffer_size)
{
	return VCP_write(buffer, buffer_size);
}

// reads up to `buffer_size` bytes from usb_cdc buffer
// returns `number of bytes read` or -1
int USB_CDC_STM32F427xx::ReadData(
		unsigned char* buffer,
		unsigned int buffer_size)
{
#if 1


	if(USBD_Device.dev_state != USBD_STATE_CONFIGURED || USBD_Device.dev_config != 1)
	{
		return -1;
	}

	int size = VCP_read(buffer, buffer_size);

	while(size <= 0)
	{
		BlockRxThread(10);
		size = VCP_read(buffer, buffer_size);
	}
	return size;
#else
	return VCP_read(buffer, buffer_size);
#endif
}


void USB_CDC_STM32F427xx::BlockRxThread(unsigned int timeout)
{
	mRxThreadBlocked = true;
	mRxSemaphore.TakeTimeout(timeout);
}

void USB_CDC_STM32F427xx::UnblockRxThread()
{
	if(mRxThreadBlocked)
	{
		mRxThreadBlocked = false;
		mRxSemaphore.Give();
	}
}

void USB_CDC_STM32F427xx::BlockTxThread(unsigned int timeout)
{
	mTxThreadBlocked = true;
	mTxSemaphore.TakeTimeout(timeout);
}

void USB_CDC_STM32F427xx::UnblockTxThread()
{
	if(mTxThreadBlocked)
	{
		mTxThreadBlocked = false;
		mTxSemaphore.Give();

	}
}
