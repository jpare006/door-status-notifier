/**
 * @file atmega168_adc.c
 *
 * @brief 
 * Driver code for atmega168 analog-to-digital converter.
 * Features used in driver:
 * 		- 10-bit res. available only need 8 bit res. (min: 0,  max: 255)
 * 		- uses single channel (ADC0 -> PC0)
 * 		- uses AREF as reference voltage
 * 		- by default circuitry requires btw. 50kHz - 200kHz, since only 8-bit res.
 * 		  being used can go higher. Code sets CLKadc = 250kHz
 */
#ifndef F_CPU
#define F_CPU 8000000UL //assumes fuses configured for 8MHz clock speed
#endif

#include <avr/io.h>
#include "atmega168_adc.h"

void create_adc(void)
{
	//select AREF as Vref
	ADMUX &= ~(11 << REFS0); //set bits REFS1 & REFS0 to 0

	//select input channel, ADC0
	ADMUX &= ~(1111 << MUX0); //set bits MUX3:0 to 0

	//left adjust adc result (since only using 8 bit resolution)
	ADMUX |= (1 << ADLAR);

	//set ADC clock prescaler to 32. CLKadc = F_CPU/prescaler = 250kHz
	ADCSRA |= (1 << ADPS2);
	ADCSRA &= ~(1 << ADPS1);
	ADCSRA |= (1 << ADPS0);

	//enable ADC
	ADCSRA |= (1 << ADEN);
}

uint8_t read(void)
{
	//start conversion
	ADCSRA |= (1 << ADSC);

	//wait for conversion to complete
	while(!(ADCSRA & (1 << ADSC)));

	//since left shifted, only need to read high value of adc
	return ADCH;
}

/*** end of file ***/