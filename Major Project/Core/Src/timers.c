#include <stdio.h>
#include <stdint.h>

#include "stm32f303xc.h"
#include "timers.h"

#define PRESCALER 47999

// pointers to callback functions
void (*TIM2_callback_function)();

void enable_interrupt(IRQn_Type IRQn, uint32_t priority) {
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();

	// Tell the NVIC module that TIM2 interrupts should be handled
	NVIC_SetPriority(IRQn, priority);  // Set Priority
	NVIC_EnableIRQ(IRQn);

	// Re-enable all interrupts (now that we are finished)
	__enable_irq();
}

void TIM2_IRQHandler(){
// run the oneshot timer interrupt handler
	if (TIM2->SR & TIM_SR_UIF) {
		// clear oneshot timer interrupt
		TIM2->SR &= ~TIM_SR_UIF;

		if (TIM2_callback_function != NULL)
		{
			TIM2_callback_function();
		}

	}

}

void game_timer(uint16_t time, void (*callback_function)()) {

	// reset CR1
	TIM2->CR1 = 0x00;

	// set the prescaler to 999, slower than the default clock 1000 times
	TIM2->PSC = PRESCALER; // 1 millisecond or 1 millisecond per count

	// set the auto reload according to the prescaler
	TIM2->ARR = 0x01; // 1 - 1 millisecond
	TIM2->ARR = TIM2->ARR * time; // delay in millisecond
	TIM2->CR1 |= TIM_CR1_ARPE; // enable auto reload buffering

	// set to only counter overflow raises interrupt flag
	TIM2->CR1 |= TIM_CR1_URS;

	// set to one-pulse mode
	TIM2->CR1 |= TIM_CR1_OPM;

	// re-initialise the counter and generates an update of the registers
	TIM2->EGR |= TIM_EGR_UG;

	// enable the update interrupt
	TIM2->DIER |= TIM_DIER_UIE;

	// clear the interrupt
	TIM2->SR &= ~TIM_SR_UIF;

	TIM2_callback_function = callback_function;

	enable_interrupt(TIM2_IRQn, 0);

	// enable counter
	TIM2->CR1 |= TIM_CR1_CEN;

}

void delay(uint16_t delay){
	// reset CR1
	TIM3->CR1 = 0x00;
	TIM3->CNT = 0;

	// set the prescaler to 999, slower than the default clock 1000 times
	TIM3->PSC = PRESCALER; // 1 millisecond or 1 millisecond per count

	// re-initialise the counter and generates an update of the registers
	TIM3->EGR |= TIM_EGR_UG;

	// enable counter
	TIM3->CR1 |= TIM_CR1_CEN;

	while (TIM3->CNT < delay) {};

	// disable counter
	TIM3->CR1 &= ~TIM_CR1_CEN;
	TIM3->CNT = 0;
}
