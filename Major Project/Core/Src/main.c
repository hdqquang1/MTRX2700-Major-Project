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
#include "headsUp.h"
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

	srand(time(NULL));  // Seed the random number generator once

	while (1){

		trigger_oneshot(30000, &set_timesUp);
		sprintf(string_to_send, "Timer starts now!\r\n");
		SerialOutputString(string_to_send, &USART1_PORT);

		uint8_t score = 0;

		while (timesUp == 0) {

			outcome = 0;


			uint8_t guessWord[20];
			char* random = randomWord();
			strcpy((char*)guessWord, random);

			sprintf(string_to_send, "%s\r\n", (char*)guessWord);
			SerialOutputString(string_to_send, &USART1_PORT);

			while (!outcome){
				BSP_GYRO_GetXYZ(&gyro_values[0]);
				if (gyro_values[1]/20000 >= 25){
					outcome = 1;
				}
				if (gyro_values[1]/20000 <= -25){
					outcome = 2;
				}
				HAL_Delay(100);
			}

			if (outcome == 1){
				score++;
				sprintf(string_to_send, "Correct!\r\n");
				SerialOutputString(string_to_send, &USART1_PORT);
				flash_leds();
			}
			else if (outcome == 2){
				sprintf(string_to_send, "Pass!\r\n");
				SerialOutputString(string_to_send, &USART1_PORT);
			}
			HAL_Delay(750);
		}

		sprintf(string_to_send, "Time's Up! Your score is %d!\r\n", score);
		SerialOutputString(string_to_send, &USART1_PORT);

		for(;;){}
	}
}
