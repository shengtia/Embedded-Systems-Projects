/*
 * GccApplication1.c
 *
 * Created: 9/24/2017 11:11:19 PM
 * Author : Stanley
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

volatile unsigned int half_period = 18182;

ISR(TIMER1_COMPA_vect){
	// toggle output
	OCR1A += half_period;
}


int main(void)
{
    // set portDB1 as the output driver
	DDRB |= (1 << DDB1);
	
	// set OCR1A 
	OCR1A = half_period;
	
	// enable timer1 output compare interrupt
	TIMSK1 |= (1 << OCIE1A);
	
	// enable global interrupt
	sei();
	
	// set output to toggle
	TCCR1A |= 0x40;
	
	// enable counter/clock
	TCCR1B |= (1 << CS10); 
	
    while (1) 
    {
    }
}

