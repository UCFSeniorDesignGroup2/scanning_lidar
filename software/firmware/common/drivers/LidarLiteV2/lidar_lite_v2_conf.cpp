// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// lidar_lite_v2_conf.cpp
// <very brief class description>
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#include "lidar_lite_v2_conf.h"

using namespace Drivers;

/*
 * LidarLiteConfig_High_RepRate:
 * Parameters:
 * 1. (LidarLiteV2*) instance
 *
 * Uses the instance to write all of the configuration registers
 * required to make the LidarLiteV2 use the fastest possible
 * repetition rate.
 *
 * Returns:
 * -1 if failure. 0 if success.
 */
int Drivers::LidarLiteConfig_High_RepRate(LidarLiteV2* instance)
{
	(void) instance;
	/*
	 * Todo
	 */
	return 0;
}

/*
 * LidarLiteConfig_High_Precision:
 * Parameters:
 * 1. (LidarLiteV2*) instance
 *
 * Uses the instance to write all of the configuration registers
 * required to make the LidarLiteV2 use the highest possible
 * precision.
 *
 * Returns:
 * -1 if failure. 0 if success.
 */
int Drivers::LidarLiteConfig_High_Precision(LidarLiteV2* instance)
{
	(void) instance;
	/*
	 * Todo
	 */
	return 0;
}
