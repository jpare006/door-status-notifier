/**
 * @file atmega168_timer.c
 *
 * @brief 
 * Driver code for atmega168 8-bit timer.
 * Uses output compare match interrupt to count in 1ms increments.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "atmega168_timer.h"

void create_timer(void)
{
	//Set to Clear Timer on Compare Match (CTC) mode
	TCCR0A |= (1 << WGM01);
	TCCR0A &= ~(1 << WGM00);
}

void on(void)
{
	//Initialize Timer register
	TCNT0 = 0;

	//Clock select: internal 8MHz clock with prescaler of 64
	// 8MHz / 64 = 125,000 ticks/sec => Timer register increments at this speed
	TCCR0B |= (11 << CS00);

	//set Output Compare Register A to value that will trigger match every 1ms
	//          125,000 ticks
	// 0.001s x ------------- = 125 ticks
	//               s 
	// therefore, every 125 ticks of the timer represents an elapsed time of 1ms
	// an interrupt will be triggered each time
	OCR0A = 125;

	//output compare match A interrupt enable
	TIMSK0 |= (1 << OCIE0A);

	//enable global interrupts
	sei();
}

void off(void)
{
	//Clock select: no clock source -> timer stopped
	TCCR0B &= ~(11 << CS00);

	//disable output compare match A interrupt
	TIMSK0 &= ~(1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect)
{
	TimerISR();
}

/*** end of file ***/