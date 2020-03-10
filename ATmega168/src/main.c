#ifndef F_CPU
#define F_CPU 8000000UL //assumes MCU fuses configured for 8MHz system clock
#endif

#include <stdint.h>
#include "adc.h"
#include "timer.h"
#include "uart.h"

#define BAUD      9600
#define PERIOD    1000 // 1 sec FSM tick rate
#define THRESHOLD 100  //adc threshold for determing if door open or closed

enum fsm_states {INIT, OPEN, CLOSED00, CLOSED01} state;
enum door_status {IS_OPEN, IS_CLOSED00, IS_CLOSED01} door;

int main(void)
{
	//initialize peripherals
	adc_init();
	uart_init(BAUD);
	timer_init(PERIOD);

	timer_on();
	state = INIT;

	//shared variables
	uint8_t curr_adc;

	while(1)
	{
		//get input
		curr_adc = adc_read();

		switch(state) //transitions
		{
			case INIT:
				curr_adc = 0;
				door = IS_OPEN;
				state = OPEN;
				break;

			case OPEN:
				if (curr_adc <= THRESHOLD)
				{
					state = OPEN;
				}
				else
				{
					state = CLOSED00;
				}
				break;

			case CLOSED00:
				state = CLOSED01;
				break;

			case CLOSED01:
				if (curr_adc >= THRESHOLD)
				{
					state = CLOSED01;
				}
				else
				{
					state = OPEN;
				}
				break;

			default:
				state = INIT;
				break;
		}

		switch(state) //actions
		{
			case INIT:
				break;

			case OPEN:
				door = IS_OPEN;
				break;

			case CLOSED00:
				door = IS_CLOSED00;
				break;

			case CLOSED01:
				door = IS_CLOSED01;
				break;

			default: break;
		}

		//output
		switch(door)
		{
			case IS_OPEN:
				uart_send('o'); // o for open
				break;
			case IS_CLOSED00:
				uart_send('c'); // c for closed
				break;
			case IS_CLOSED01:
				break;
			default: break;
		}

		while(!TimerFlag);
		TimerFlag = 0;
	}
}