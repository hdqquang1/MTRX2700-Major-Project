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

	uint8_t string_to_send[64] = "This is a string!\r\n";

	uint8_t rounds = 1;
	uint8_t winner;
	uint8_t multiplier;
	uint8_t P1leaderboard;
	uint8_t P2leaderboard;

	while (rounds <= 3) {
		uint8_t P1score = 0;
		uint8_t P2score = 0;

		headsUp(&P1score, &P2score);

		// Winner determined
		if (P1score > P2score){
			// player 1 will play wavelength
			winner = 1;
		}
		else if (P2score > P1score){
			// player 2 will play wavelength
			winner = 2;
		}
		//else {
		//}

		// Multiplier
		//uint8_t buffer[8];
		//SerialInputString(buffer, 8, &UART4_PORT, '\r');
		//multiplier = atoi(buffer);
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

	for(;;){};
}
