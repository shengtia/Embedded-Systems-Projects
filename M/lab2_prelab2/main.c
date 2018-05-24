/*
 * GccApplication2.c
 *
 * Created: 9/22/2017 7:03:32 PM
 * Author : shengtia
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
    /* Replace with your application code */
	DDRB |= (1 << DDB1);
    while (1) 
    {
		PORTB |= (1<<DDB1);
		_delay_ms(200);
		
		PORTB&=~(1<<DDB1);
		_delay_ms(200);
    }
}

