#include <stdint.h>
#include "led_display.h"
#include "stm32f303xc.h"
#include "main.h"
#include "initialise.h"
#include "serial.h"

// Initialise the discovery board I/O, and gets a pointer to the second half of the MODER register (8-15)
void initialise_led() {
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;
}


// Function to flash LEDs for 1 second
void flash_leds() {
    // Turn on LEDs
    GPIOE->ODR = 0xFFFF;

    // Delay for 1 second (adjust delay according to system clock)
    for (volatile int i = 0; i < 2000000; i++);

    // Turn off LEDs
    GPIOE->ODR = 0x0000;
}
