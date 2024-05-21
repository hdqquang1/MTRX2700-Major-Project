#ifndef DIGITAL_IO_H
#define DIGITAL_IO_H
#include <stdint.h>

// correct_leds(): turn on LEDs to create a circle for 1s
void correct_leds(void);

// incorrect_leds(): turn on LEDs to create a cross for 1s
void incorrect_leds(void);

// end_leds(): turn on LEDs to create a circle for 5s
void end_leds(void);

void chase_leds(void);

uint8_t get_button_state(void);

void set_leds(uint8_t led_mask_pattern);

uint8_t get_leds(void);



#endif
