#ifndef __USB_CDC_STM32F427xx
#define __USB_CDC_STM32F427xx

#include "hal/usb_cdc/hal_usb_cdc.h"

#include <oal/oal_semaphore.h>
#include <oal/oal_mutex.h>
#include <stdint.h>

namespace HAL
{
	class USB_CDC_STM32F427xx : public USB_CDC
	{
	public:

		#define USB_CDC_BUFFER_SIZE 128

		typedef enum
		{
			UNINITIALIZED = 0,
			INITIALIZED = 1
		} USB_CDC_STATE;


		USB_CDC_STM32F427xx();
		virtual ~USB_CDC_STM32F427xx();

		// set up cdc device
		virtual int Initialize();

		// writes `buffer_size` bytes over usb_cdc
		// returns 0 if success
		virtual int WriteData(
				const unsigned char* buffer,
				unsigned int buffer_size);

		// reads up to `buffer_size` bytes from usb_cdc buffer
		// returns `number of bytes read` or -1
		virtual int ReadData(
				unsigned char* buffer,
				unsigned int buffer_size);

		void CDC_Init();
		void OnRxData(uint8_t* data, uint16_t size);
		void OnTxFinish();

	protected:

		int WriteDataRec(
				const unsigned char* buffer,
				unsigned int buffer_size);

		OAL::Semaphore mRxSemaphore;
		OAL::Semaphore mTxSemaphore;
		OAL::Semaphore mInitSemaphore;
		OAL::Mutex mMutex;
		uint8_t mRxBuffer[USB_CDC_BUFFER_SIZE];
		uint16_t mRxHead;
		uint16_t mRxTail;

		USB_CDC_STATE mState;
	};
}

#endif
