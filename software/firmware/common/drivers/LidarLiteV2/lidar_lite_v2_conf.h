// ---------------------------------------------------------------------------
// This software is in the public domain, furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// lidar_lite_v2_conf.h
// This file contains different configurations for the LidarLiteV2.
//
// Author: jack
//   Date: Sep 2, 2015
// ---------------------------------------------------------------------------

#ifndef DRIVERS_LIDARLITEV2_LIDAR_LITE_V2_CONF_H_
#define DRIVERS_LIDARLITEV2_LIDAR_LITE_V2_CONF_H_

#include "lidar_lite_v2.h"

namespace Drivers
{
	// Configures the device for high rep rate
	int LidarLiteConfig_High_RepRate(LidarLiteV2* instance);

	// Configures the device for high precision
	int LidarLiteConfig_High_Precision(LidarLiteV2* instance);
}

#endif
