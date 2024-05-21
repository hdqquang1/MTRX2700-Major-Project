#include "gyroscope.h"

#include <stdio.h>

#include "BSP/stm32f3_discovery.h"
#include "BSP/stm32f3_discovery_gyroscope.h"


// get_gyro_values - Get pitch gyroscope value
// Returns pitch gyroscope value in degree
float get_gyro_values(void){
	float gyro_values[3];

	BSP_GYRO_GetXYZ(&gyro_values[0]);

	return gyro_values[1]/20000;
}
