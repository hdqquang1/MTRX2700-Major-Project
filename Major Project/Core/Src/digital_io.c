#include "digital_io.h"

#include <stdio.h>
#include <stdint.h>

#include "stm32f303xc.h"
#include "timers.h"

#define CORRECT 0b11111111
#define INCORRECT 0b01010101


uint8_t get_button_state(void){
	return GPIOA->IDR;
}

// set_leds - Set LEDs pattern by binary number
// Input: binary led mask pattern
void set_leds(uint8_t led_mask_pattern)
{
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	if (led_mask_pattern <= 255 && led_mask_pattern >= 0)
	{
		*led_register = led_mask_pattern;
	}
}


// get_leds - Get LEDs current state
// Returns LEDs current state
uint8_t get_leds(void){
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	return *led_register;
}


// correct_leds - Turn on circle LEDs pattern for 1s
void correct_leds(void){
    // Turn on LEDs
    set_leds(CORRECT);

    // Delay for 1 second (adjust delay according to system clock)
    delay(1000);

    // Turn off LEDs
    set_leds(0);
}


// incorrect_leds - Turn on cross LEDs pattern for 1s
void incorrect_leds(void){
    // Turn on LEDs
    set_leds(INCORRECT);

    // Delay for 1 second (adjust delay according to system clock)
    delay(1000);

    // Turn off LEDs
    set_leds(0);
}


// end_leds - Turn on circle LEDs pattern for 2s
void end_leds(void){
	 // Turn on LEDs
	set_leds(CORRECT);

	// Delay for 5 second (adjust delay according to system clock)
	delay(2000);

	// Turn off LEDs
	 set_leds(0);
}


// chase_leds - Shift LED 1 by 1
void chase_leds(void){
	uint8_t mask = get_leds() << 1;
	set_leds(mask);
	if (get_leds() == 0) {
		set_leds(1);
	}
}
