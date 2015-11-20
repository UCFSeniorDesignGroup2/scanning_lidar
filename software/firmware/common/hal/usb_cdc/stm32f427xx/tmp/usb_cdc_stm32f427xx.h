#ifndef __USB_CDC_STM32F427xx
#define __USB_CDC_STM32F427xx

#include "hal/usb_cdc/hal_usb_cdc.h"

#include <oal/oal_semaphore.h>

namespace HAL
{
	class USB_CDC_STM32F427xx : public USB_CDC
	{
	public:

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

		void UnblockRxThread();
		void BlockRxThread(unsigned int timeout);

		void UnblockTxThread();
		void BlockTxThread(unsigned int timeout);

	protected:

		bool mTxThreadBlocked;
		bool mRxThreadBlocked;

		OAL::Semaphore mTxSemaphore;
		OAL::Semaphore mRxSemaphore;
	};
}

#endif
