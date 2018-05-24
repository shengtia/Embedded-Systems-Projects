/*
 * GccApplication1.c
 *
 * Created: 9/24/2017 6:47:52 PM
 * Author : Stanley
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include "uart.h"

volatile bool light_on = false;

ISR(TIMER1_CAPT_vect){
	if(light_on)
		light_on = false;
	else
		light_on = true;
}

int main(void)
{
	uart_init();
	// Set PB5 (pin 13) as OUTPUT set PB0 as input
	DDRB|=(1 << DDB5);
	// enable input capture interrupt
	TIMSK1 |= (1 << ICIE1);
	// enable global interrupt
	sei();
	// enable clock TCCR1B, no pre_scaling, why do we need to enable the clock tho?
	TCCR1B |= (1 << CS10);
	
	printf("test printf!\n");
	/* Replace with your application code */
	while (1)
	{
		if(light_on){
			// turn on the led
			PORTB |= (1 << DDB5);
			// set rising edge to trigger interrupt
			TCCR1B |= (1 << ICES1);
			
		}else{
			// turn off the led
			PORTB &= ~(1 << DDB5);
			// set lowering edge to trigger interrupt
			TCCR1B &= !(1 << ICES1);
		}
	}
}

 