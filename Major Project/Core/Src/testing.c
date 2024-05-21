#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "BSP/stm32f3_discovery.h"
#include "BSP/stm32f3_discovery_gyroscope.h"
#include "digital_io.h"
#include "serial.h"
#include "timers.h"


#define BUFFER_SIZE 100


void test_USART1(void) {
	uint8_t buffer[BUFFER_SIZE];

	while(1) {
		SerialInputString(buffer, BUFFER_SIZE, &USART1_PORT, '\r');
		strcat(buffer, "\n");
		SerialOutputString(buffer, &USART1_PORT);
	}
}


void test_UART4(void){
	uint8_t buffer_to_send[BUFFER_SIZE] = "Test string\r\n";
	uint8_t buffer_to_receive[BUFFER_SIZE];

	SerialOutputString(buffer_to_send, &UART4_PORT);
	SerialInputString(buffer_to_receive, BUFFER_SIZE, &UART4_PORT, '\r');
}


void test_gyroscope(void){
	uint8_t buffer[BUFFER_SIZE];

	float gyro_values[3];

	while (1) {
		BSP_GYRO_GetXYZ(&gyro_values[0]);
		sprintf(buffer, "%0.3f,%0.3f,%0.3f\r\n",
		gyro_values[0]/20000, gyro_values[1]/20000, gyro_values[2]/20000);
		SerialOutputString(buffer, &USART1_PORT);
	}
}


void test_led_pattern(void){
	if (get_leds() == 0){
		set_leds(0xFF);
	}
	else{
		set_leds(0x00);
	}
}

void test_io_and_timer(void){
	while (1){
		while((get_button_state() & 0x01) == 0){}
		game_timer(1000, &test_led_pattern);
		delay(1000);
	}
}
