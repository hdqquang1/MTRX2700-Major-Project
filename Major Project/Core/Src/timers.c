#include <timers.h>
#include <stdio.h>
#include "stm32f303xc.h"

#define PRESCALER 7999
#define HAL_MULTIPLY_FACTOR 6

// pointer to callback function for oneshot timer
void (*oneshot_callback_function)();

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

		if (oneshot_callback_function != NULL)
		{
			oneshot_callback_function();
		}

	}

}

void trigger_oneshot(uint16_t delay, void (*callback_function)()) {

	// reset CR1
	TIM2->CR1 = 0x00;

	// set the prescaler to 999, slower than the default clock 1000 times
	TIM2->PSC = PRESCALER; // 1 millisecond or 1 millisecond per count

	// set the auto reload according to the prescaler
	TIM2->ARR = 0x01; // 8 - 1 millisecond
	TIM2->ARR = TIM2->ARR * delay * HAL_MULTIPLY_FACTOR; // delay in millisecond
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

	oneshot_callback_function = callback_function;

	enable_interrupt(TIM2_IRQn, 0);

	// enable counter
	TIM2->CR1 |= TIM_CR1_CEN;

}
