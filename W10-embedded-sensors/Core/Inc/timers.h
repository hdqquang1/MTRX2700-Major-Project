#ifndef TIMERS_HEADER
#define TIMERS_HEADER

#include <stdint.h>

#include "stm32f303xc.h"

// enable timer interrupts
// input: interrupt to enable and priority
void enable_interrupt(IRQn_Type IRQn);

// timer_iniitialise: run the callback function continuously at the input interval
// Input: interval in millisecond, pointer to the callback function
void timer_initialise(uint16_t interval, void (*callback_function)());

// trigger_oneshot: trigger the callback function once after the input delay
//Input: delay in millisecond, pointer to the callback function
void trigger_oneshot(uint16_t delay, void (*callback_function)());

#endif
