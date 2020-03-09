/**
 * @file uart.h
 *
 * @brief 
 * UART library for the ATmega168. This library uses interrupts for data reception.
 * The library interface enables the user to send/read individual characters and entire strings. 
 * This library also makes use of a circular buffer library for storing data.
 *
 * Note: requires CircularBuffer.c and CircularBuffer.h which can be found in github.
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "CircularBuffer.h"

#define CBUF_SIZE 64

enum {SUCCESS = 0, FAIL = -1, CHAR_NOT_FOUND = -2, UNKNOWN = -3};
enum {MCU, KEYBOARD};

void uart_init(uint32_t baud_rate);
void uart_terminate(void);
BOOL uart_send_ready(void);
void uart_send(char data);
int uart_send_string(char * string, size_t sz);
void uart_rx_complete_ISR(void);
size_t uart_available(void);
int uart_read(char * data);
int uart_read_string(char * data, int inputMethod);
void uart_flush(void);

#endif // UART_H