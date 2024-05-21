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
volatile uint16_t remaining_time; // Variable to store remaining time
volatile uint16_t total_time;     // Variable to store total game time
uint32_t last_print_time = 0;

char* randomWord() {
    static char* words[NUM_WORDS]; // Static array to store words
    static int usedIndices[NUM_WORDS] = {0}; // Static array to track used indices
    static int wordsCount = 0; // Count of words added to the array

    // Check if all words have been used, reset if needed
    if (wordsCount == NUM_WORDS) {
        wordsCount = 0;
        memset(usedIndices, 0, sizeof(usedIndices));
    }

    char *allWords[] = {
            "airplane", "boat", "baby", "ears", "scissors", "cough",
            "cold", "phone", "laugh", "blink", "hairbrush", "sneeze",
            "spin", "hammer", "book", "phone", "toothbrush", "jump",
            "clap", "slap", "archer", "ghost", "balance", "shoelaces", "sick", "balloon",
            "banana peel", "monster", "hiccup", "stomp", "hurt", "hungry",
            "slip", "karate", "ladder", "scare", "fishing", "dizzy",
            "read", "hot", "birthday", "president", "apartment", "cradle", "coffee", "trampoline",
            "waterfall", "window", "proud", "stuck-up", "flashlight", "marry",
            "overwhelm", "judge", "shadow", "halo", "measure", "clown",
            "chomp", "slither", "Whale", "Snake", "Elephant", "Giraffe", "Dog", "Cat",
            "Ant", "Rabbit", "Groundhog", "Hyena", "Kangaroo", "Shark",
            "Fish", "Polar Bear", "Caterpillar", "Cockroach", "Ram", "Monkey",
            "Jaguar", "Rooster", "Cycling", "Watching TV", "Swimming", "Running",
            "Flying in a plane", "Sleeping", "Flipping pancakes", "Listening to music",
            "Driving a car", "Writing a letter", "Skiing", "Giving a haircut",
            "Drinking", "Typing", "Eating", "Having a snowball fight", "Playing the guitar",
            "Surfing", "Yawning", "Playing golf", "Praying", "Brushing teeth",
            "Showering", "Meditating", "Singing"
    };

    int numWords = sizeof(allWords) / sizeof(allWords[0]); // Calculate the number of words in the array

    int randomIndex;
    do {
        randomIndex = rand() % numWords; // Generate a random index within the bounds of the array
    } while (usedIndices[randomIndex]); // Check if the word at this index has been used

    usedIndices[randomIndex] = 1; // Mark this index as used
    words[wordsCount++] = allWords[randomIndex]; // Add the word to the array

    return words[wordsCount - 1]; // Return the last added word
}

#define NUM_TOPICS 9 // Total number of topics in the array

char* randomTopic() {
    static char* topics[NUM_TOPICS]; // Static array to store words
    static int usedIndices[NUM_TOPICS] = {0}; // Static array to track used indices
    static int topicCount = 0; // Count of words added to the array

    // Check if all words have been used, reset if needed
    if (topicCount == NUM_TOPICS) {
    	topicCount = 0;
        memset(usedIndices, 0, sizeof(usedIndices));
    }

	char *allTopics[] = {
	        "<-Normal pet  Exotic pet->", "<-Useless body part  Useful body part->", "<-Person you could beat up  Person who'd beat you up->",
			"<-Bad pizza topping  Good pizza topping->", "<-Useless major  Useful major->", "<-Worst living person  Greatest living person->",
			"<-Boring hobby  Interesting hobby->", "<-Unsexy emoji  sexy emoji->", "<-Good subject  Bad subject->"
	};

    int numTopics = sizeof(allTopics) / sizeof(allTopics[0]); // Calculate the number of words in the array

    int randomIndex;
    do {
        randomIndex = rand() % numTopics; // Generate a random index within the bounds of the array
    } while (usedIndices[randomIndex]); // Check if the word at this index has been used

    usedIndices[randomIndex] = 1; // Mark this index as used
    topics[topicCount++] = allTopics[randomIndex]; // Add the word to the array

    return topics[topicCount - 1]; // Return the last added word
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

void update_game_timer_to_serial() {
    uint32_t current_time = HAL_GetTick();
    if (current_time - last_print_time >= 1000) { // Print every second
        last_print_time = current_time;

        if (remaining_time > 0) {
            remaining_time -= 1000;
            char string_to_send[128];
            sprintf(string_to_send, "Remaining time: %d ms\r\n", remaining_time);
            SerialOutputString((uint8_t *)string_to_send, &USART1_PORT);
        }
    }
}

void headsUp(uint8_t *P1score, uint8_t *P2score){
    uint8_t string_to_send[128] = "This is a string !\r\n";

    BSP_GYRO_Init();
    BSP_ACCELERO_Init();

    for (int i = 1; i <= 2; i++) {
        uint8_t Pscore = 0;

        sprintf(string_to_send, "Player %d your timer starts now!\r\n", i);
        SerialOutputString(string_to_send, &USART1_PORT);
        delay(1000);
        total_time = 10000; // 10 seconds for testing
        remaining_time = total_time;
        last_print_time = HAL_GetTick();

        game_timer(total_time, &set_timesUp);

        while (timesUp == 0) {
            update_game_timer_to_serial();

            uint8_t outcome = 0;

            uint8_t guessWord[20];
            char* random = randomWord();
            strcpy((char*)guessWord, random);
            sprintf(string_to_send, "%s\r\n", (char*)guessWord);
            SerialOutputString(string_to_send, &USART1_PORT);

            while (!outcome) {
                update_game_timer_to_serial(); // Ensure the timer is updated in the inner loop as well

                float gyro_values[3];
                int16_t acc_values[3];
                float float_acc_values[3];

                BSP_GYRO_GetXYZ(gyro_values);
                BSP_ACCELERO_GetXYZ(acc_values);

                float_acc_values[0] = (float)acc_values[0] / 1500.;
                float_acc_values[1] = (float)acc_values[1] / 1500.;
                float_acc_values[2] = (float)acc_values[2] / 1500.;

                sprintf((char*)string_to_send, "%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f\r\n",
                        gyro_values[0] / 20000, gyro_values[1] / 20000, gyro_values[2] / 20000,
                        float_acc_values[0], float_acc_values[1], float_acc_values[2]);
                SerialOutputString(string_to_send, &USART1_PORT);

                if (gyro_values[1] / 20000 >= 25) {
                    outcome = 1;
                }
                if (gyro_values[1] / 20000 <= -25) {
                    outcome = 2;
                }
                if (timesUp) {
                    outcome = 0;
                    break;
                }
                delay(100);
            }

            if (outcome == 1) {
                Pscore++;
                sprintf(string_to_send, "Correct!\r\n");
                SerialOutputString(string_to_send, &USART1_PORT);
                correct_leds();
            }
            else if (outcome == 2) {
                sprintf(string_to_send, "Pass!\r\n");
                SerialOutputString(string_to_send, &USART1_PORT);
                incorrect_leds();
            }
            else {
                break;
            }
        }
        sprintf(string_to_send, "Time's Up! Player %d's score is %d!\r\n", i, Pscore);
        SerialOutputString(string_to_send, &USART1_PORT);
        end_leds();
        if (i == 1) {
            *P1score = Pscore;
            sprintf(string_to_send, "Press to continue\r\n");
            SerialOutputString(string_to_send, &USART1_PORT);
            while ((get_button_state()) == 0) {}
            set_timesUp();
        }
        else {
            *P2score = Pscore;
            sprintf(string_to_send, "Press to continue\r\n");
            SerialOutputString(string_to_send, &USART1_PORT);
            while ((get_button_state()) == 0) {}
            set_timesUp();
        }
    }
}
