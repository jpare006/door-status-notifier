/**
 * @file uart_atmega168.c
 *
 * @brief 
 * ATmega168 driver code. This code implements an interface to control the ATmega168's
 * USART peripheral.
 */
#ifndef F_CPU
#define F_CPU 8000000UL //assumes fuses configured for 8MHz clock speed
#endif

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "atmega168_uart.h"



static void enable_interrupt(void)
{
	// macro used to set global interrupt mask
	//
	sei();

	//enable receive complete interrupt
	//
	UCSR0B |= (1 << RXCIE0);
}

static void disable_interrupt(void)
{
	//macro used to clear global interrupt mask
	//
	cli();

	//disable receive complete interrupt
	//
	UCSR0B &= ~(1 << RXCIE0);
}



void create_uart(uint32_t baud_rate)
{
	//set baud rate
	uint8_t baud_prescaler = ((F_CPU/16)/baud_rate) - 1;
	UBRR0H = (uint8_t)(baud_prescaler >> 8);
	UBRR0L = (uint8_t)baud_prescaler;

	//do not use double USART transmission speed
	UCSR0A &= ~(1 << U2X0);

	// enable transmitter and receiver
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);

	// select mode of operation
	// set frame format: 8 bit data size, 1 stop bit, no parity
	UCSR0C = ~(3 << UMSEL00) & (3 << UCSZ00);

	enable_interrupt();
}

void destroy(void)
{
	// return control registers to initial values
	//
	UCSR0A = 0x20;
	UCSR0B = 0x00;
	UCSR0C = 0x06;

	disable_interrupt();
}

BOOL is_send_ready(void)
{
	return UCSR0A & (1 << UDRE0);
}

void send(uint8_t data)
{
	//initiate transmission
	UDR0 = data;
}

BOOL is_data_available(void)
{
	return UCSR0A & (1 << RXC0);
}

void flush(void)
{
	uint8_t dummy = 0;
	dummy += 1; //to make compiler happy

	//read buffer until flag becomes zero
	//
	while (UCSR0A & (1 << RXC0))
	{
		dummy = UDR0;
	}
}

uint8_t receive(void)
{
	uint8_t data = UDR0;
	return data;
}

ISR(USART_RX_vect)
{
	//calls function that implements required functionality
	//designed this way to make the ISR testable with Unity
	//
	uart_rx_complete_ISR();
}