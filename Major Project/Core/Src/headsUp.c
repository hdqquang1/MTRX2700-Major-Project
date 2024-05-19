#include "headsUp.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "digital_io.h"
#include "gyroscope.h"
#include "serial.h"
#include "stm32f303xc.h"
#include "timers.h"

#define NUM_WORDS 100 // Total number of words in the array

char* randomWord() {
    static char *allWords[] = {
        "airplane", "boat", "baby", "ears", "scissors", "cough",
        "cold", "phone", "laugh", "blink", "hairbrush", "sneeze",
        "spin", "hammer", "book", "phone", "toothbrush", "jump",
        "clap", "slap", "archer", "ghost", "balance", "shoelaces", "sick",
        "balloon", "banana peel", "monster", "hiccup", "stomp", "hurt", "hungry",
        "slip", "karate", "ladder", "scare", "fishing", "dizzy", "read", "hot",
        "birthday", "president", "apartment", "cradle", "coffee", "trampoline",
        "waterfall", "window", "proud", "stuck-up", "flashlight", "marry",
        "overwhelm", "judge", "shadow", "halo", "measure", "clown", "chomp",
        "slither", "whale", "snake", "elephant", "giraffe", "dog", "cat", "ant",
        "rabbit", "groundhog", "hyena", "kangaroo", "shark", "fish", "polar bear",
        "caterpillar", "cockroach", "ram", "monkey", "jaguar", "rooster", "cycling",
        "watching TV", "swimming", "running", "flying in a plane", "sleeping",
        "flipping pancakes", "listening to music", "driving a car", "writing a letter",
        "skiing", "giving a haircut", "drinking", "typing", "eating", "having a snowball fight",
        "playing the guitar", "surfing", "yawning", "playing golf", "praying", "brushing teeth",
        "showering", "meditating", "singing"
    };

    static int usedIndices[NUM_WORDS] = {0}; // Static array to track used indices
    static int numUsed = 0; // Count of used words

    if (numUsed == NUM_WORDS) {
        // If all words have been used, reset the usedIndices array
        memset(usedIndices, 0, sizeof(usedIndices));
        numUsed = 0;
    }

    int numWords = sizeof(allWords) / sizeof(allWords[0]); // Calculate the number of words in the array

    int randomIndex;
    do {
        randomIndex = rand() % numWords; // Generate a random index within the bounds of the array
    } while (usedIndices[randomIndex]); // Check if the word at this index has been used

    usedIndices[randomIndex] = 1; // Mark this index as used
    numUsed++; // Increment the count of used words

    return allWords[randomIndex]; // Return the randomly chosen word
}

uint8_t timesUp = 0;

void set_timesUp(){
    if (timesUp == 0) {
        timesUp = 1;
    }
    else {
        timesUp = 0;
    }
}

void headsUp(uint8_t *P1score, uint8_t *P2score){

	uint8_t string_to_send[64] = "This is a string !\r\n";

	for (int i = 1; i <= 2; i++) {
		uint8_t Pscore = 0;

		sprintf(string_to_send, "Player %d your timer starts now!\r\n", i);
		SerialOutputString(string_to_send, &USART1_PORT);
		game_timer(30000, &set_timesUp);

		while (timesUp == 0) {
			uint8_t outcome = 0;


			uint8_t guessWord[20];
			char* random = randomWord();
			strcpy((char*)guessWord, random);
			sprintf(string_to_send, "%s\r\n", (char*)guessWord);
			SerialOutputString(string_to_send, &USART1_PORT);

			while (!outcome){
				if (get_gyro_values() >= 25){
					outcome = 1;
				}
				if (get_gyro_values() <= -25){
					outcome = 2;
				}
				if (timesUp){
					outcome = 0;
					break;
				}
				delay(100);
			}

			if (outcome == 1){
				Pscore++;
				sprintf(string_to_send, "Correct!\r\n");
				SerialOutputString(string_to_send, &USART1_PORT);
				correct_leds();
			}
			else if (outcome == 2){
				sprintf(string_to_send, "Pass!\r\n");
				SerialOutputString(string_to_send, &USART1_PORT);
				incorrect_leds();
			}
			else{
				break;
			}
		}
		sprintf(string_to_send, "Time's Up! Player %d's score is %d!\r\n", i, Pscore);
		SerialOutputString(string_to_send, &USART1_PORT);
		end_leds();
		if (i == 1){
			*P1score = Pscore;
			sprintf(string_to_send, "Press to continue\r\n");
			SerialOutputString(string_to_send, &USART1_PORT);
			while ((GPIOA->IDR & 0x01) == 0) {}
			set_timesUp();
		}
		else {
			*P2score = Pscore;
			sprintf(string_to_send, "Press to continue\r\n");
			SerialOutputString(string_to_send, &USART1_PORT);
			while ((GPIOA->IDR & 0x01) == 0) {}
			set_timesUp();
		}
	}
}
