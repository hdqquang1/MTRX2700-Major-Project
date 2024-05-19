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
#include "initialise.h"
#include "headsUp.h"
/* USER CODE END Includes */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	initialise_board();

	uint8_t buffer[8];
	SerialInputString(buffer, 8, &UART4_PORT, '\r');
	uint8_t multiplier = atoi(buffer);

	uint8_t string_to_send[64] = "This is a string!\r\n";
	sprintf(string_to_send, "Multiplier is %d\r\n", multiplier);
	SerialOutputString(string_to_send, &USART1_PORT);

	for(;;){};
}
