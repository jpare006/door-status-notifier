/**
 * @file timer.h
 *
 * @brief 
 * Timer library for the ATmega168. This library gives the user access to the underlying
 * driver code. Meant to generate a period for use with a finit-state machine. Periods
 * are not to exceed 65,532 ms ~= 65 seconds.
 *
 * Example usage of the library can be found in main.c
 */

#include "timer.h"

//used to determine length of period. Set by user.
static uint16_t avr_timer_count;

//used to count down to 0, starts at avr_timer_count
static volatile uint16_t avr_timer_curr_count = 1;

/*!
 * @brief Initialize timer peripheral and 2 global variables
 * @param[in] period Period duration in milliseconds
 */
void timer_init(uint16_t period)
{
	TimerFlag = 0;
	avr_timer_count = period;

	create_timer();
}

/*!
 * @brief Initialize global variable and start timer
 */
void timer_on(void)
{
	//initialize count down variable
	avr_timer_curr_count = avr_timer_count;
	on();
}

/*!
 * @brief Turn of timer
 * @par Disbales timer interrupt but not global interrupts
 */
void timer_off(void)
{
	off();
}

/*!
 * @brief 
 * Called by interrupt handler every millisecond. Only sets TimerFlag to 1 after
 * period has elapsed.
 * 
 * @par
 * User must reset TimerFlag to 0 for proper operation
 */
void TimerISR(void)
{
	avr_timer_curr_count -= 1;

	if(avr_timer_curr_count == 0)
	{
		TimerFlag = 1;
		avr_timer_curr_count = avr_timer_count;
	}
}

/*** end of file ***/