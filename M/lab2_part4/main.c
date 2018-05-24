/*
 * Lab2_part4.c
 *
 * Created: 9/28/2017 3:15:00 PM
 * Author : Stanley
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"
#include <stdbool.h>
#define F_CPU 16000000UL
#include <util/delay.h>

volatile bool trigger = true;
unsigned int trigger_duration = 120, edge1, edge2, diff;
volatile unsigned int overflows;
unsigned long pulse_width;

ISR(TIMER1_OVF_vect){
	overflows++;
}

ISR(TIMER2_COMPB_vect){
	trigger = false;
}

void sendTrigger(){
	sei();					 // enable global interrupt
	TIMSK2 |= (1 << OCIE2B); // enable output compare interrupt
	PORTB |= (1 << DDB1);	 // set pb1 high
	TCNT2 = 0;		         // set timer to 0
	TCCR2B |= (1 << CS20);   // enable clock
	while(trigger);			 // generate the trigger
	PORTB &= ~(1 << DDB1);	 // set pb1 low
	TCCR2B &= ~(1 << CS20);  // disable clock
	trigger = true;			 // reset the trigger
	cli();				     //disable global interrupt
}

int main(void)
{
    uart_init();            // initialize uart for printing outputs to terminal
	DDRB |= (1 << DDB1);	// set port pb1 to output the trigger signal	
	TIMSK1 |= 0x01;			// enable overflow interrupt
	TCCR1B |= (1 << CS10);  // start the timer
	
	// setup timer 2 for output compare
	OCR2B = trigger_duration;	 // set the trigger duration
	
	
    while (1) 
    {
		sendTrigger();
		
		TCCR1B |= 0x40;	// capture rising edge
		TIFR1 |= 0x20; // clear input capture flag
		
		while(!(TIFR1 & 0x20));	// wait until an rising edge
		sei(); // enable global interrupt
		overflows = 0;
		TCNT1 = 0; //initialize timer
		TCCR1B &= 0XBF; // capture falling edge
		TIFR1 |= 0x20; // clear input capture flag
		
		while(!(TIFR1 & 0x20));
		cli();
		edge2 = TCNT1; // record the timer
		int over = overflows; // record the overflow
		
		pulse_width = (long) over * 65536u + (long)edge2;
		printf("pulse width = 0.0625 * %lu us \n", pulse_width);
		
		_delay_ms(20);
    }
} 

