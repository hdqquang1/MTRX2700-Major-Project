#include <timers.h>
#include <stdio.h>
#include "stm32f303xc.h"

#define PRESCALER 999
#define MILLISECOND_SCALAR 8

// pointer to callback function for continuous timer
void (*continuous_callback_function)();
// pointer to callback function for oneshot timer
void (*oneshot_callback_function)();

void enable_interrupt(IRQn_Type IRQn) {
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();

	// Tell the NVIC module that TIM2 interrupts should be handled
	uint32_t prioritygroup = 0x00U;
	uint32_t PreemptPriority = 0;
	uint32_t SubPriority = 0;
	prioritygroup = NVIC_GetPriorityGrouping();
	NVIC_SetPriority(IRQn, 0);
	NVIC_EnableIRQ(IRQn);
	NVIC_SetPriority(SPI1_IRQn, 1);
	NVIC_EnableIRQ(SPI1_IRQn);

	// Re-enable all interrupts (now that we are finished)
	__enable_irq();
}

void TIM2_IRQHandler(){
	// run the continuous timer interrupt handler
	if (TIM2->SR & TIM_SR_UIF) {
		TIM2->SR &= ~TIM_SR_UIF; // clear continuous timer interrupt
		continuous_callback_function();
	}

}

void timer_initialise(uint16_t interval, void (*callback_function)()) {

	// reset CR1
	TIM2->CR1 = 0x00;

	// enable counter
	TIM2->CR1 |= TIM_CR1_CEN;

	// set the prescaler to 999, slower than the default clock 1000 times
	TIM2->PSC = PRESCALER; // 125 microseconds = 0.125 milliseconds per count

	// set the auto reload according to the prescaler
	TIM2->ARR = MILLISECOND_SCALAR; // 8 - 1 millisecond
	TIM2->ARR = TIM2->ARR * interval; // interval in millisecond
	TIM2->CR1 |= TIM_CR1_ARPE; // enable auto reload buffering

	// set to only counter overflow raises interrupt flag
	TIM2->CR1 |= TIM_CR1_URS;

	// re-initialise the counter and generates an update of the registers
	TIM2->EGR |= TIM_EGR_UG;

	// enable the update interrupt
	TIM2->DIER |= TIM_DIER_UIE;

	// clear the interrupt
	TIM2->SR &= ~TIM_SR_UIF;

	continuous_callback_function = callback_function;

	enable_interrupt(TIM2_IRQn);

}

void TIM3_IRQHandler(){
// run the oneshot timer interrupt handler
	if (TIM3->SR & TIM_SR_UIF) {
		// clear oneshot timer interrupt
		TIM3->SR &= ~TIM_SR_UIF;

		if (oneshot_callback_function != NULL)
		{
			oneshot_callback_function();
		}

	}

}

void trigger_oneshot(uint16_t delay, void (*callback_function)()) {

	// reset CR1
	TIM3->CR1 = 0x00;

	// set the prescaler to 999, slower than the default clock 1000 times
	TIM3->PSC = 0x3E7; // 125 microseconds = 0.125 milliseconds per count

	// set the auto reload according to the prescaler
	TIM3->ARR = 0x08; // 8 - 1 millisecond
	TIM3->ARR = TIM3->ARR * delay; // delay in millisecond
	TIM3->CR1 |= TIM_CR1_ARPE; // enable auto reload buffering

	// set to only counter overflow raises interrupt flag
	TIM3->CR1 |= TIM_CR1_URS;

	// set to one-pulse mode
	TIM3->CR1 |= TIM_CR1_OPM;

	// re-initialise the counter and generates an update of the registers
	TIM3->EGR |= TIM_EGR_UG;

	// enable the update interrupt
	TIM3->DIER |= TIM_DIER_UIE;

	// clear the interrupt
	TIM3->SR &= ~TIM_SR_UIF;

	oneshot_callback_function = callback_function;

	enable_interrupt(TIM3_IRQn);

	// enable counter
	TIM3->CR1 |= TIM_CR1_CEN;

}
