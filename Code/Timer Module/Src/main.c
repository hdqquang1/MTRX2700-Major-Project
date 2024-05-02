#include "stm32f303xc.h"
#include <stdint.h>


static void (*callback)(void) = 0;
static uint32_t
period_ms = 0;
static uint32_t one_shot_counter_ms = 0; // Counter for the one-shot event
static uint32_t one_shot_duration_ms = 0; // Duration for the one-shot event

// Function prototypes
void enable_clocks(void);
void initialise_board(void);
void timer_init(uint32_t interval_ms, void (*cb)(void));
void leds(void);
void set_timer_period(uint32_t new_period_ms); // Setter
uint32_t get_timer_period(void); // Getter
void reset_timer(void); // Resets the timer with the new period
void stop_timer(void);
void stop_leds(void);
void one_shot_init(uint32_t duration_ms);

int main(void) {
    enable_clocks();
    initialise_board();

    // Initialize timer with a 1-second period and the leds callback
    timer_init(1000, leds);
    one_shot_init(5000);
}

void enable_clocks() {

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}

void initialise_board() {
	// get a pointer to the second half word of the MODER register (for outputs pe8-15)
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;
}

void timer_init(uint32_t interval_ms, void (*cb)(void)) {
    TIM2->CR1 &= ~TIM_CR1_CEN; // Disable timer
    TIM2->PSC = 7999; // Prescaler for 1ms resolution at 8MHz
    TIM2->ARR = interval_ms; // Set auto-reload value for desired interval
    TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt
    NVIC_SetPriority(TIM2_IRQn, 0); // Set priority
    NVIC_EnableIRQ(TIM2_IRQn); // Enable TIM2 interrupt
    TIM2->CR1 |= TIM_CR1_CEN; // Enable timer
    callback = cb; // Store the callback function pointer
    period_ms = interval_ms;
}

void one_shot_init(uint32_t duration_ms) {
    one_shot_duration_ms = duration_ms; // Set the one-shot duration
}

// Interrupt Service Routine for Timer 2
void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF; // Clear update interrupt flag
        if (callback) {
            callback(); // Execute the callback function
        }
//        // Multiply the period by 1.05 without exceeding some maximum value
//        uint32_t new_period = period_ms * 1.05;
//        set_timer_period(new_period > 1500 ? 1500 : new_period); // Update the period

        one_shot_counter_ms += period_ms;
		if (one_shot_counter_ms >= one_shot_duration_ms) {
			stop_leds(); // Stop the timer and pause LEDs
			one_shot_counter_ms = 0; // Reset the counter if needed
		}
    }

}

uint32_t get_timer_period(void) {
    return period_ms; // Return the current period
}

void stop_leds(void) {
    stop_timer(); // Stop the timer
    // LED state is automatically preserved as no changes to GPIOE->ODR are made after this point
}

void stop_timer(void) {
    TIM2->CR1 &= ~TIM_CR1_CEN; // Disable timer
    // Optional: Reset the counter value if necessary
    TIM2->CNT = 0;
}

void set_timer_period(uint32_t new_period_ms) {
    period_ms = new_period_ms; // Update the period
    reset_timer(); // Apply the new period immediately
}

void reset_timer(void) {
    TIM2->CR1 &= ~TIM_CR1_CEN; // Disable timer
    TIM2->ARR = period_ms; // Update the auto-reload value with the new period
    TIM2->CR1 |= TIM_CR1_CEN; // Enable timer
}

void leds(void) {
    static uint32_t current_led = 0;
    // Assuming LEDs are connected to GPIOE pins 8 to 15
    GPIOE->ODR = (1 << (current_led + 8)); // Turn on the current LED
    current_led = (current_led + 1) % 8; // Move to the next LED
}





