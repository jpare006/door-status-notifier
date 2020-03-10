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

#include "uart.h"
#include "atmega168_uart.h"

//circular buffer used to store incoming data
//
static volatile cbuf_handle_t cbuf;

/*!
 * @brief Initialize microcontroller USART module and receive buffer.
 * @param[in] baud_rate - User specified baud rate.
 * @param[out] cbuf     - Variable that holds circular buffer handle.
 * 
 * @par
 * This function must be called at least once before data can be transmitted
 * or received by USART module.
 */
void uart_init(uint32_t baud_rate)
{
	create_uart(baud_rate);
	char * buffer = malloc(sizeof(char) * CBUF_SIZE);
	cbuf = circular_buf_init(buffer, CBUF_SIZE);
}

/*!
 * @brief Disable USART module and free memory allocated to the circular buffer.
 * 
 * @par
 * It is recommended that this function be called at end of program if uart_init()
 * is used.
 */
void uart_terminate(void)
{
	destroy();
	circular_buf_free(cbuf);
}

/*!
 * @brief Use to check if USART transmit is able to transmit data.
 * @return Boolean value answering if ready or not.
 */
BOOL uart_send_ready(void)
{
	return is_send_ready();
}

/*!
 * @brief Transmit one character.
 * @param[in] data - Character to be transmitted.
 */
void uart_send(char data)
{
	while(!(is_send_ready()));

	send(data);
}

/*!
 * @brief Transmit a string of size sz, including the null character.
 * @param[in] str - Pointer to array holding string to be sent.
 * @param[in] sz  - Size of string, including null character
 * @return Error code indicating if null or newline character was found.
 * 
 * @par
 * If you are sending a string to another microcontroller using this interface,
 * it is required to send the null character.
 * When calculating the size of the string to be sent, make sure to include the 
 * null charcter in the count.
 *
 * Known issue: If the string in uninitialized, this function will not detect
 * that it is empty since garbage values will be present in the array.
 */
int uart_send_string(char * str, size_t sz)
{
	//string must not be empty
	if(str[0] == '\0')
	{
		return FAIL;
	}

	//make sure null character present before sending (null characters should
	// always be at end of string)
	if(str[sz - 1] != '\0' && str[sz - 1] != '\n')
	{
		return FAIL;
	}

	for (size_t i = 0; i < sz; ++i)
	{
		uart_send(str[i]);
	}

	return SUCCESS;
}

/*!
 * @brief Find the amount of characters currently available for reading
 * @return The amount of characters available in buffer.
 */
size_t uart_available(void)
{
	return (size_t)circular_buf_size(cbuf);
}

/*!
 * @brief Read one charcater from internal buffer
 * @param[out] data - Pointer to variable where data is to be stored.
 */
int uart_read(char * data)
{
	return circular_buf_get(cbuf, data);
}

/*!
 * @brief Read an entire string
 * @param[out] data - Pointer to array where string is to be stored
 * @param[in] inputMethod - Choose between MCU or KEYBOARD
 * @return Size of string just read. If overflow occurs returns error code (CHAR_NOT_FOUND).
 *
 * @par 
 * Relies on null character or newline character being present to read one string 
 * at a time. Which character is looked for depends on the input method. This function
 * will look for a null charcter if MCU is selected as the input method or a newline 
 * charcater if KEYBOARD is selected. If a null/newline character is not found, and
 * the circular buffer reaches its max capacity, the function will return the
 * CHAR_NOT_FOUND error code.
 *
 * Known issue: timeout functionality not yet implemented. This function will not 
 * return until a null/newline character is found or the internal buffer has reached
 * max capcity.
 */
int uart_read_string(char * data, int inputMethod)
{
	char findMe;
	if (inputMethod == MCU)
	{
		findMe = '\0';
	}
	else if (inputMethod == KEYBOARD)
	{
		findMe = '\n';
	}
	else
	{
		return UNKNOWN;
	}

	char temp;
	size_t cnt = 0;
	int complete = 0;
	char backup[CBUF_SIZE];

	while(!complete)
	{
		if(uart_available() > 0)
		{
			uart_read(&temp);

			data[cnt] = temp;
			backup[cnt] = temp;
			++cnt;

			if (temp == findMe)
			{
				complete = 1;
			}
			else if (cnt >= CBUF_SIZE)
			{
				//restore internal circular buffer before returning
				for(size_t i = 0; i < CBUF_SIZE; ++i)
				{
					circular_buf_put(cbuf, backup[i]);
				}
				return CHAR_NOT_FOUND;
			}
		}
	}
	return cnt;
}

/*!
 * @brief Reset internal circular buffer.
 */
void uart_flush(void)
{
	//clears all stored values held in cbuf by modifying tail and head
	circular_buf_reset(cbuf);
}

/*!
 * @brief Called by interrupt handler every time data is received.
 *
 * @par
 * Interrupt handler disables interrupts before calling this funciton. Therefore, 
 * this function runs atomically.
 */
void uart_rx_complete_ISR(void)
{
	char incomingByte = receive();
	circular_buf_put(cbuf, incomingByte);
}