/**
 * @file atmega168_timer.h
 *
 * @brief 
 * Driver code for atmega168 8-bit timer.
 * Uses output compare match interrupt to count in 1ms increments.
 */

#ifndef _ATMEGA168_TIMER_H_
#define _ATMEGA168_TIMER_H_

#include <stdio.h>

void create_timer(void);
void on(void);
void off(void);
extern void TimerISR(void);

#endif

/*** end of file ***/