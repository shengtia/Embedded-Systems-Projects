/*art
 * 9.30.c
 *
 * Created: 2017/9/30 15:57:29
 * Author : Administrator
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "uart.h"
#include <avr/interrupt.h>

int main(void)
{
	uart_init();
	ADMUX |= (1 << REFS0);										//AREF = AVcc
	ADMUX &= ~(1 << REFS1);
	
	ADCSRA |= (1 << ADEN);										//ADC enable
	ADCSRA |= 0x07;	// prescale
	ADCSRA |= (1 << ADATE);										//auto trigger enable
	ADCSRA |= (1 << ADSC);										// start the first conversion
	
	while (1) 
    {
		printf("the voltage is %u / 1024 * 5 V \n", ADC);   // print the voltage to console
    }
}

