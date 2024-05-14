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

	uint8_t rounds = 1;
	uint8_t outcome;
	uint8_t winner;
	uint8_t multiplier;
	uint8_t P1leaderboard;
	uint8_t P2leaderboard;

	srand(time(NULL));  // Seed the random number generator once

	while (rounds <= 3){

		uint8_t P1score = 0;
		uint8_t P2score = 0;

		trigger_oneshot(30000, &set_timesUp);
		sprintf(string_to_send, "Player 1 your timer starts now!\r\n");
		SerialOutputString(string_to_send, &USART1_PORT);

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
				P1score++;
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
		sprintf(string_to_send, "Time's Up! Player 1's score is %d!\r\n", P1score);
		SerialOutputString(string_to_send, &USART1_PORT);
		set_timesUp();

		// Player 2 plays heads up
		trigger_oneshot(30000, &set_timesUp);
		sprintf(string_to_send, "Player 2 your timer starts now!\r\n");
		SerialOutputString(string_to_send, &USART1_PORT);

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
				P2score++;
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
		sprintf(string_to_send, "Time's Up! Player 2's score is %d!\r\n", P2score);
		SerialOutputString(string_to_send, &USART1_PORT);
		set_timesUp();


		// Winner determined
		if (P1score > P2score){
			// player 1 will play wavelength
			winner = 1;
		}
		else if (P2score > P1score){
			// player 2 will play wavelength
			winner = 2;
		}

		// Multiplier
		multiplier = 2;

		// Wavelength multiplier
		if (winner == 1){
			P1score = P1score * multiplier;
		}
		else if (winner == 2){
			P2score = P2score * multiplier;
		}

		// Update leaderboard
		P1leaderboard = P1leaderboard + P1score;
		P2leaderboard = P2leaderboard + P2score;
		sprintf(string_to_send, "Player 1 your total score is %d!\r\n", P1leaderboard);
		SerialOutputString(string_to_send, &USART1_PORT);
		sprintf(string_to_send, "Player 2 your total score is %d!\r\n", P2leaderboard);
		SerialOutputString(string_to_send, &USART1_PORT);

		rounds++;
	}

	// Final winner announced
	if (P1leaderboard > P2leaderboard){
		sprintf(string_to_send, "Player 1 Wins with a final score of %d!\r\n", P1leaderboard);
		SerialOutputString(string_to_send, &USART1_PORT);
	}
	else if (P2leaderboard > P1leaderboard){
		sprintf(string_to_send, "Player 2 Wins with a final score of %d!\r\n", P2leaderboard);
		SerialOutputString(string_to_send, &USART1_PORT);
	}
}
