/*
 * GccApplication2.c
 *
 * Created: 9/25/2017 1:46:21 PM
 * Author : Stanley
 */ 

#include <avr/io.h>
#include <stdbool.h>
volatile unsigned int half_period = 18182;

int main(void)
{
	// set output port
	DDRB |= (1 << DDB1);
	// set value for OCR1A, a frequency of 440HZ
	OCR1A = half_period;
	// set toggle on TCCR1A
	TCCR1A |= (1 << COM1A0);
	// set CTC
	TCCR1B |= (1 << WGM12);
	// enable clock on normal clock frequency
	TCCR1B |= (1 << CS10);
	
    /* Replace with your application code */
    while (1) 
    {
		// do nothing
    }
}

