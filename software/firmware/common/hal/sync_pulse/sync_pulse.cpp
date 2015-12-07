/*
 * sync_pulse.cpp
 *
 *  Created on: Dec 6, 2015
 *      Author: jack
 */


#include "sync_pulse.h"

using namespace HAL;

SyncPulse::SyncPulse()
{
	mExtClock = 0;
}

void SyncPulse::SetExternalClock(ExternalClock* ext_clock)
{
	mExtClock = ext_clock;
}
