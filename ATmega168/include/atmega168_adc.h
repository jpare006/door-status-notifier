/**
 * @file atmega168_adc.h
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
#ifndef _ATMEGA168_ADC_H_
#define _ATMEGA168_ADC_H_

#include <stdint.h>

void create_adc(void);
uint8_t read(void);

#endif /*_ATMEGA168_ADC_H_*/

/*** end of file ***/