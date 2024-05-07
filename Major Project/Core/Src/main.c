/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BSP/stm32f3_discovery.h"
#include "BSP/stm32f3_discovery_gyroscope.h"
#include "initialise.h"
#include "serial.h"
#include "timers.h"
/* USER CODE END Includes */

uint8_t timesUp = 0;

void set_timesUp(){
    if (timesUp == 0) {
        timesUp = 1;
    }
    else {
        timesUp = 0;
    }
}

void set_LEDs(){
	uint8_t *led_output_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	*led_output_register = 0b01010101;
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	initialise_board();

	SerialInitialise(BAUD_115200, &USART1_PORT, 0x00);
	uint8_t string_to_send[64] = "This is a string !\r\n";

	float gyro_values[3];

	uint8_t outcome;

	while (1){

		trigger_oneshot(30000, &set_timesUp);

		while (timesUp == 0) {
			outcome = 0;

			while (!outcome){
				BSP_GYRO_GetXYZ(&gyro_values[0]);
				if (gyro_values[1]/20000 >= 25){
					outcome = 1;
				}
				if (gyro_values[1]/20000 <= -25){
					outcome = 2;
				}
			}

			if (outcome == 1){
				sprintf(string_to_send, "Correct!\r\n");
				SerialOutputString(string_to_send, &USART1_PORT);
			}
			else if (outcome == 2){
				sprintf(string_to_send, "Pass!\r\n");
				SerialOutputString(string_to_send, &USART1_PORT);
			}
			HAL_Delay(500);
		}

		sprintf(string_to_send, "Time's Up!\r\n");
		SerialOutputString(string_to_send, &USART1_PORT);

		for(;;){}
	}
}
