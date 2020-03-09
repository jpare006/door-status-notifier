/**
 * @file adc.c
 *
 * @brief 
 * ADC library for the ATmega168. 
 * The library interface enables the user to access the ATmega168's ADC peripheral.
 */

#include "adc.h"
#include "atmega168_adc.h"

void adc_init(void)
{
	create();
}

uint8_t adc_read(void)
{
	return read();
}

/*** end of file ***/