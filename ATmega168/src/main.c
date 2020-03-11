#ifndef F_CPU
#define F_CPU 8000000UL //assumes MCU fuses configured for 8MHz system clock
#endif

#include <stdint.h>
#include "adc.h"
#include "timer.h"
#include "uart.h"

#define BAUD      9600
#define PERIOD    1000 // 1 sec FSM tick rate
#define THRESHOLD 100  //adc threshold for determining if door is open or closed
#define DELAY     2    //used to delay the sending of another "OPEN" message

enum fsm_states {INIT, OPEN00, OPEN01, CLOSED00, CLOSED01} state;
typedef enum door_status {IS_OPEN, IS_CLOSED, UNCHANGED} door;

static void send_status(door status);

int main(void)
{
	//initialize peripherals
	adc_init();
	uart_init(BAUD);
	timer_init(PERIOD);

	//shared variables
	uint8_t curr_adc;
	int cnt = 0;
	state = INIT;
	door status = IS_OPEN; 

	timer_on();
	
	while(1)
	{
		//get input
		curr_adc = adc_read();

		switch(state) //transitions
		{
			case INIT:
				curr_adc = 0;
				cnt      = 0;
				status   = IS_OPEN;
				state    = OPEN00;
				break;

			case OPEN00:
				state = OPEN01;
				break;

			case OPEN01:
				if (curr_adc > THRESHOLD)
				{
					state = CLOSED00;
				}
				else if (cnt == DELAY)
				{
					state = OPEN00;
				}
				else
				{
					state = OPEN01;
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
					state = OPEN00;
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

			case OPEN00:
				status = IS_OPEN;
				cnt = 0;
				break;

			case OPEN01:
				status = UNCHANGED;
				cnt += 1;
				break;

			case CLOSED00:
				status = IS_CLOSED;
				break;

			case CLOSED01:
				status = UNCHANGED;
				break;

			default: break;
		}

		//output
		send_status(status);

		while(!TimerFlag);
		TimerFlag = 0;
	}
}

static void send_status(door status)
{
	switch(status)
		{
			case IS_OPEN:
				uart_send('o'); // o for open
				break;
			case IS_CLOSED:
				uart_send('c'); // c for closed
				break;
			case UNCHANGED:
				break;
			default: break;
		}
}