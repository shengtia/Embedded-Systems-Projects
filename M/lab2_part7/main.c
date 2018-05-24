/*
 * Lab2_part7.c
 *
 * Created: 10/1/2017 5:26:00 PM
 * Author : Stanley
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"

volatile bool trigger = true;
unsigned int trigger_duration = 120, edge1, edge2, diff;
volatile unsigned int overflows;
unsigned long pulse_width;
unsigned int segment = 5000;

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

unsigned char classify(unsigned long width){
	
	if(999 <= width && width <= segment)
		return (unsigned char)0;
	else if(segment <= width && width <= segment*2)
		return (unsigned char)1;
	else if(segment*2 <= width && width <= segment*3)
		return (unsigned char)2;
	else if(segment*3 <= width && width <= segment*4)
		return (unsigned char)3;
	else if(segment*4 <= width && width <= segment*5)
		return (unsigned char)4;
	else if(segment*5 <= width && width <= segment*6)
		return (unsigned char)5;
	else if(segment*6 <= width && width <= segment*7)
		return (unsigned char)6;
	else if(width >= segment*7)
		return (unsigned char)7;
	return (unsigned char)10;
}

void caseSelect(char mode){
	switch(mode){
		case 0:
			PORTB &= ~(1 << DDB2);
			PORTB &= ~(1 << DDB3);
			PORTB &= ~(1 << DDB4);
			break;
		case 1:
			PORTB |= (1 << DDB2);
			PORTB &= ~(1 << DDB3);
			PORTB &= ~(1 << DDB4);
			break;
		case 2:
			PORTB &= ~(1 << DDB2);
			PORTB |= (1 << DDB3);
			PORTB &= ~(1 << DDB4);
			break;
		case 3:
			PORTB |= (1 << DDB2);
			PORTB |= (1 << DDB3);
			PORTB &= ~(1 << DDB4);
			break;
		case 4:
			PORTB &= ~(1 << DDB2);
			PORTB &= ~(1 << DDB3);
			PORTB |= (1 << DDB4);
			break;
		case 5:
			PORTB |= (1 << DDB2);
			PORTB &= ~(1 << DDB3);
			PORTB |= (1 << DDB4);
			break;
		case 6:
			PORTB &= ~(1 << DDB2);
			PORTB |= (1 << DDB3);
			PORTB |= (1 << DDB4);
			break;
		case 7:
			PORTB |= (1 << DDB2);
			PORTB |= (1 << DDB3);
			PORTB |= (1 << DDB4);
			break;	
		
	}
}



int main(void)
{
	
	uart_init();
	DDRB |= (1 << DDB1);	// set port pb1 to output the trigger signal	
	TIMSK1 |= 0x01;			// enable overflow interrupt
	TCCR1B |= (1 << CS10);  // start the timer
	
	DDRB |= (1 << DDB2);	// set the output ports pb2, pb3, pb4
	DDRB |= (1 << DDB3);
	DDRB |= (1 << DDB4);
	
	// setup timer 2 for output compare
	OCR2B = trigger_duration;	 // set the trigger duration

    while (1) 
    {
    	sendTrigger();
		
		TCCR1B |= 0x40;	// capture rising edge
		TIFR1 |= 0x20; // clear input capture flag
		
		while(!(TIFR1 & 0x20));	// wait until an rising edge
		sei();
		overflows = 0;
		TCNT1 = 0; //initialize timer
		TCCR1B &= 0XBF; // capture falling edge
		TIFR1 |= 0x20; // clear input capture flag 
		
		while(!(TIFR1 & 0x20));
		cli();
		edge2 = TCNT1; // record the timer
		int over = overflows; // record the overflow
		
		pulse_width = (long) over * 65536u + (long)edge2;
		printf("pulse width is %lu \n", pulse_width);
		caseSelect(classify(pulse_width)); // select the proper output on portb based on the pulse width
		_delay_ms(25);
    }
} 

