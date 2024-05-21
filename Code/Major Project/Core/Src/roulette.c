#include "roulette.h"

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "stm32f303xc.h"

#include "digital_io.h"
#include "serial.h"

void roulette(uint8_t *prize){
	while ((GPIOA->IDR & 0x01) == 0) {}
	uint16_t delay_period = 25;

	for (uint8_t i = 1; i < 6; i++) {
		for(uint8_t j = 0; j < 8; j++){
			chase_leds();
			delay(delay_period*i);
		}
	}

	uint8_t string_to_send[64] = "This is a string !\r\n";

	uint8_t r = rand() % 8 + 1;
	*prize = r;
	sprintf(string_to_send, "Random number is %d\r\n", r);
	SerialOutputString(string_to_send, &USART1_PORT);

	uint8_t mask = 0;
	// Turn on the random number of LEDs
	for (uint8_t i = 0; i < r; i++) {
		mask |= (1 << i);
	}

	set_leds(mask);
}
