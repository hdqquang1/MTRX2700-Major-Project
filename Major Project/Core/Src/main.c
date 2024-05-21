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
#include "roulette.h"
#include "testing.h"
/* USER CODE END Includes */

/**
  * @brief  The application entry point.
  * @retval int
  */

#define BUFFER_SIZE 100

int main(void)
{
	initialise_board();

	test_USART1();
	//test_UART4();
	//test_gyroscope();
	//test_io_and_timer();

	uint8_t buffer[BUFFER_SIZE];

	srand(get_gyro_values());

	uint8_t rounds = 1;
	uint8_t winner;
	uint8_t multiplier;
	uint8_t P1leaderboard = 0;
	uint8_t P2leaderboard = 0;
	uint8_t prize;

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
		else {
			break;
		}

		sprintf(buffer, "Player %d can play wavelength\r\n", winner);
		SerialOutputString(buffer, &USART1_PORT);
		uint8_t buffer[16];
		SerialInputString(buffer, 16, &UART4_PORT, '\r');
		multiplier = atoi(buffer);

		sprintf(buffer, "Wavelength result %d!\r\n", multiplier);
		SerialOutputString(buffer, &USART1_PORT);

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
		sprintf(buffer, "Player 1 your total score is %d!\r\n", P1leaderboard);
		SerialOutputString(buffer, &USART1_PORT);
		sprintf(buffer, "Player 2 your total score is %d!\r\n", P2leaderboard);
		SerialOutputString(buffer, &USART1_PORT);

		delay(1500);

		rounds++;
	}

	// Final winner announced
	if (P1leaderboard > P2leaderboard){
		sprintf(buffer, "Player 1 Wins with a final score of %d!\r\n", P1leaderboard);
		SerialOutputString(buffer, &USART1_PORT);
	}
	else if (P2leaderboard > P1leaderboard){
		sprintf(buffer, "Player 2 Wins with a final score of %d!\r\n", P2leaderboard);
		SerialOutputString(buffer, &USART1_PORT);
	}
	sprintf(buffer, "Winner can spin the wheel now!\r\n");
	SerialOutputString(buffer, &USART1_PORT);

	roulette(&prize);

	switch(prize) {
	case 1:
		sprintf(buffer, "You won a deluxe pen!\r\n");
		SerialOutputString(buffer, &USART1_PORT);
		break;
	case 2:
		sprintf(buffer, "You won an exclusive notebook!\r\n");
		SerialOutputString(buffer, &USART1_PORT);
		break;
	case 3:
		sprintf(buffer, "You won a high accuracy ruler!\r\n");
		SerialOutputString(buffer, &USART1_PORT);
		break;
	case 4:
		sprintf(buffer, "You won a world class hydration container!\r\n");
		SerialOutputString(buffer, &USART1_PORT);
		break;
	case 5:
		sprintf(buffer, "You won an exclusive rubber band!\r\n");
		SerialOutputString(buffer, &USART1_PORT);
		break;
	case 6:
		sprintf(buffer, "You won an exclusive signature from Omar!\r\n");
		SerialOutputString(buffer, &USART1_PORT);
		break;
	case 7:
		sprintf(buffer, "You won a luxurious charger!\r\n");
		SerialOutputString(buffer, &USART1_PORT);
		break;
	case 8:
		sprintf(buffer, "You won a high precision pencil sharpener!\r\n");
		SerialOutputString(buffer, &USART1_PORT);
		break;
	}


	for(;;){};

}
