#ifndef DIGITAL_IO_H
#define DIGITAL_IO_H

#include <stdint.h>


uint8_t get_button_state(void);


// set_leds - Set LEDs pattern by binary number
// Input: binary led mask pattern
void set_leds(uint8_t led_mask_pattern);


// get_leds - Get LEDs current state
// Returns LEDs current state
uint8_t get_leds(void);


// correct_leds - Turn on circle LEDs pattern for 1s
void correct_leds(void);


// incorrect_leds - Turn on cross LEDs pattern for 1s
void incorrect_leds(void);


// end_leds - Turn on circle LEDs pattern for 2s
void end_leds(void);


// chase_leds - Shift LED 1 by 1
void chase_leds(void);


#endif
