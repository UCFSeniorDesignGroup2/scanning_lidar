/*
 * sync_pulse1.h
 *
 *  Created on: Dec 6, 2015
 *      Author: jack
 */

// tested and works.
// jack ritchey

#ifndef HAL_SYNC_PULSE_SYNC_PULSE1_SYNC_PULSE1_H_
#define HAL_SYNC_PULSE_SYNC_PULSE1_SYNC_PULSE1_H_

#include <hal/sync_pulse/sync_pulse.h>

#include <oal/oal_common.h>

namespace HAL
{
	// this pin is used to signal the TDC7200 to
	// start its internal counter.
	// this pin must be synchronized with the external
	// 16MHz clock in order to get accurate range readings.
	class SyncPulse1 : public SyncPulse
	{
	public:

		SyncPulse1();

		// initialize the hardware
		virtual int32_t Initialize();

		// set the state of the pin.
		virtual int32_t SetState(uint8_t state);

		// called from ExternalClock interrupt to set the state
		// of the gpio pin
		void SyncState();

		// get instance of SyncPulse1
		static SyncPulse* GetInstance();

	protected:
		// state of the gpio pin
		uint8_t mState;
		// is initialized
		bool mInitialized;
		// semaphore synchronize calling thread with the gpio pin state
		OAL::Semaphore mSemaphore;
		// instance of SyncPulse1
		static SyncPulse1 mInstance;
	};
}




#endif /* HAL_SYNC_PULSE_SYNC_PULSE1_SYNC_PULSE1_H_ */
