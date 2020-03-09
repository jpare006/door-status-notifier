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

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "atmega168_timer.h"

volatile uint8_t TimerFlag;

void timer_init(uint16_t period);
void timer_on(void);
void timer_off(void);

#endif // TIMER_H

/*** end of file ***/