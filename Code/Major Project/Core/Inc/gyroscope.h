#ifndef GYROSCOPE_HEADER
#define GYROSCOPE_HEADER

#include <stdio.h>

#include "BSP/stm32f3_discovery.h"
#include "BSP/stm32f3_discovery_gyroscope.h"

// get necessary gyroscope's value for Heads Up
// input: void
float get_gyro_values(void);

#endif
