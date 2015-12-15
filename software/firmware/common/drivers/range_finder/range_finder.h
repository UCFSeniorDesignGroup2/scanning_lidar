/*
 * range_finder.h
 *
 *  Created on: Nov 19, 2015
 *      Author: jack
 */

#ifndef DRIVERS_RANGE_FINDER_RANGE_FINDER_H_
#define DRIVERS_RANGE_FINDER_RANGE_FINDER_H_

#include <stdint.h>

#include <oal/oal_common.h>

#include <hal/pwm/external_clock/external_clock.h>
#include <hal/gpio/hal_gpio.h>
#include <hal/sync_pulse/sync_pulse.h>
#include <hal/hal_spi/hal_spi.h>
#include <hal/analog_output/analog_output.h>
#include <hal/fast_pulse/fast_pulse.h>

namespace Drivers
{
	class RangeFinder
	{
	public:

		RangeFinder();

		// provide hardware interfaces to the RangeFinder driver
		void SetTDCExternalClock(HAL::ExternalClock* ext_clock);
		void SetTDCEnablePin(HAL::HAL_GPIO_PIN* gpio);
		void SetPeakResetPin(HAL::HAL_GPIO_PIN* gpio);
		void SetTDCStartPin(HAL::SyncPulse* sync_pulse);
		void SetTDCIntBPin(HAL::HAL_GPIO_PIN* gpio);
		void SetTDCSPIInterface(HAL::HAL_SPI* spi_interface);
		void SetThresholdPin(HAL::AnalogOutput* analog_out);
		void SetLaserFirePin(HAL::FastPulse* fast_pulse);

		// initialize the device using hardware provided
		int32_t Initialize();

		// Configure the number of averages to sample.
		void ConfigAveraging(uint32_t avg);

		// configure TDC7200 time of flight measurement mode
		// parameter:
		// 1. mode:
		//      mode=1(mode1), mode=2(mode2)
		void ConfigTofMode(uint8_t mode);



		int32_t GetRangeInMiliMeters(int32_t& section);

		// write to a tdc7200 register.  note reg length is either 1 byte or 3 bytes.
		int32_t WriteRegister(uint8_t addr, const uint8_t* data, uint32_t length);
		// read from a tdc7200 register. note reg length is either 1 byte or 3 bytes.
		int32_t ReadRegister(uint8_t addr, uint8_t* data, uint32_t length);

		// called when an interrupt is received on intb pin
		void HandleIntBISR();


	protected:

		int32_t CalcTOF();

		HAL::ExternalClock* mExternalClock;
		HAL::HAL_GPIO_PIN* mEnablePin;
		HAL::HAL_GPIO_PIN* mResetPin;
		HAL::HAL_GPIO_PIN* mIntBPin;
		HAL::SyncPulse* mStartPin;
		HAL::HAL_SPI* mSPIInterface;
		HAL::AnalogOutput* mThreshold;
		HAL::FastPulse* mLaserPulse;

		OAL::Semaphore mSemaphore;

		bool mInitialized;
	};

}

#endif /* DRIVERS_RANGE_FINDER_RANGE_FINDER_H_ */
