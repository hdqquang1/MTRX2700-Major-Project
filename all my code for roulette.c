```main.c

 //ALL CODE HAS BEEN ADAPTED FROM STEWART MTRX2700-2024

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "stm32f303xc.h"
#include <digital_io_interface.h>
#include <timers_interface.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void start_roulette_led(void) {
    uint32_t time_interval = 100; // Define a regular time interval for an event to occur
    TIM2->ARR = time_interval - 1; // Set the auto-reload value
    TIM2->CR1 |= TIM_CR1_CEN; // Enable the timer
    timer_callback = &LED_chaser; // Set the callback function
}

int main(void) {
    LED current_led;
    initialise_timer();
    enable_io_interrupt();
    srand(time(NULL)); //Generates random number

    // Pass start_led_chaser as the button press callback
    digital_io_init(&current_led, start_roulette_led);

    // Ensure LEDs are off initially
    GPIOE->ODR &= ~0xFF00;

    while (1) {
        // Main loop can be empty or perform other tasks
    }
}

```

```digital_io_interface.c


#include <stdint.h>
#include <timers_interface.h>
#include "digital_io_interface.h"
#include "stm32f303xc.h"

// Callback function pointer declaration
void (*on_button_press)(void) = 0x00;

void EXTI0_IRQHandler(void) {
    if (on_button_press != 0x00) {
        on_button_press();
    }
    EXTI->PR |= EXTI_PR_PR0; // Clear the pending interrupt
}

void initialise_board() {
    RCC->AHBENR |= RCC_AHBENR_GPIOEEN; // Enable GPIOE clock
    GPIOE->MODER &= ~0xFFFF0000; // Clear mode bits for pins 8-15
    GPIOE->MODER |= 0x55550000;  // Set pins 8-15 to output mode
}

void enable_io_interrupt() {
    __disable_irq();

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable SYSCFG clock

    SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI0_PA; // Map EXTI0 line to PA0

    EXTI->RTSR |= EXTI_RTSR_TR0; // Enable rising edge trigger for line 0
    EXTI->IMR |= EXTI_IMR_MR0;   // Unmask EXTI line 0

    NVIC_SetPriority(EXTI0_IRQn, 1); // Set priority
    NVIC_EnableIRQ(EXTI0_IRQn); // Enable EXTI0 interrupt

    __enable_irq();
}

void digital_io_init(LED *led_state, void (*button_callback)(void)) {
    enable_clocks();
    initialise_board();

    on_button_press = button_callback;
}

````
  

```timer_interface.c

#include <stdint.h>
#include <stdlib.h>
#include <timers_interface.h>
#include "stm32f303xc.h"

void (*timer_callback)(void) = 0;

void enable_clocks(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}

void initialise_timer(void) {
    enable_clocks();

    TIM2->PSC = 7999; // Prescaler to get 1 ms time base (assuming 8 MHz clock)
    TIM2->ARR = 1000 - 1; // Auto-reload value for 1 second interval (1000 ms)
    TIM2->CNT = 0;

    TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt
    NVIC_SetPriority(TIM2_IRQn, 3);
    NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) { // Check if update interrupt flag is set
        TIM2->SR &= ~TIM_SR_UIF; // Clear the update interrupt flag
        if (timer_callback) {
            timer_callback();
        }
    }
}

void LED_chaser(void) {
    static uint8_t led_position = 1;
    static uint8_t prev_led_position = 0;
    static uint8_t rotations = 0;
    static uint8_t state = 0; // 0 = spinning, 1 = random display
    static uint8_t led_count = 0; // Number of LEDs to display

    if (state == 0) {
        // Turn off the previous LED
        if (prev_led_position != 0) {
            GPIOE->ODR &= ~(prev_led_position << 8);
        }

        // Turn on the current LED
        GPIOE->ODR |= (led_position << 8);

        // Update positions
        prev_led_position = led_position;
        led_position <<= 1;

        if (led_position == 0) {
            led_position = 1;
            rotations++;
        }

        if (rotations >= 5) {
            // Generate a random number of LEDs to turn on
            led_count = (rand() % 8) + 1; // Random number between 1 and 8

            // Move to the next state
            state = 1;
            rotations = 0;
            prev_led_position = 0;
        }
    } else if (state == 1) {
        // Turn off all LEDs first
        GPIOE->ODR &= ~0xFF00;

        // Turn on the random number of LEDs
        for (uint8_t i = 0; i < led_count; i++) {
            GPIOE->ODR |= (1 << (i + 8));
        }

        // Stop the timer
        TIM2->CR1 &= ~TIM_CR1_CEN;

        // Reset the state for the next run
        state = 0;
        led_position = 1;
    }
}
```
