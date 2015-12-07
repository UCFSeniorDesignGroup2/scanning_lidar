// somewhat tested. has not failed so far. more testing required.
// jack ritchey

#include <hal/hal_spi/hal_spi.h>
#include <oal/oal_common.h>

namespace HAL
{
	class HAL_SPI1 : public HAL_SPI
	{
	public:

		HAL_SPI1();

		virtual int32_t Initialize();
		virtual int32_t TransferData(const uint8_t* data_out, uint8_t* data_in, uint32_t length);

		void TransferComplete();

		static HAL_SPI* GetInstance();
	protected:

		OAL::Semaphore mTxRxSemaphore;

		static HAL_SPI1 mInstance;
	};
}
