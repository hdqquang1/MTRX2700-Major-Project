#ifndef GYROSCOPE_HEADER
#define GYROSCOPE_HEADER

#include <stdio.h>

#include "BSP/stm32f3_discovery.h"
#include "BSP/stm32f3_discovery_gyroscope.h"

// get_gyro_values - Get pitch gyroscope value
// Returns pitch gyroscope value in degree
float get_gyro_values(void);

#endif
