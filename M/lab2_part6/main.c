/*
 * GccApplication1.c
 *
 * Created: 10/1/2017 2:32:53 PM
 * Author : Stanley
 */ 

#include <avr/io.h>
#include <stdio.h>

void setFrequency(unsigned int x){
	if(x < 184)
		OCR0A = 62;
	else if(x < 288)
		OCR0A = 66;
	else if(x < 392)
		OCR0A = 74;
	else if(x < 496)
		OCR0A = 83;
	else if(x < 600)
		OCR0A = 93;
	else if(x < 704)
		OCR0A = 99;
	else if (x <808)
		OCR0A = 111;
	else
		OCR0A = 124;
}

int main(void)
{
	// default for selecting pc0 to read from the sensor
	ADMUX &= ~(0 << REFS1); // set voltage reference to 5 v
	ADMUX |= (1 << REFS0);	
	
    ADCSRA |= 0x7;			// pre_scale ADC by 128
	ADCSRA |= (1 << ADATE); // set auto trigger mode
	ADCSRA |= (1 << ADEN);  // enable the ADC
	ADCSRA |= (1 << ADSC);  // start the first measurement

	DDRD |= (1 << DDD6);    // set pd6 as the output pin
	
	TCCR0A |= (1 << WGM01); // set timer 0 on CTC mode
	TCCR0A |= (1 << COM0A0); // set timer 0 toggle
	
		
	TCCR0B |= 0x3; // enable clock prescaled by 64
	
    while (1) 
    {
		setFrequency(ADC); // read sensor value and set frequency
    }
}