#ifndef TIMERS_HEADER
#define TIMERS_HEADER

#include <stdint.h>

#include "stm32f303xc.h"


// game_timer: timing of each round of the game
// input: time in millisecond, pointer to the callback function
void game_timer(uint16_t time, void (*callback_function)());


// delay: delay for a period of time
// input: time in millisecond
void delay(uint16_t delay_period);


#endif
