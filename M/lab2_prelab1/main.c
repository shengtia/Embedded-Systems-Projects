/*
 * GccApplication1.c
 *
 * Created: 9/22/2017 5:04:14 PM
 * Author : shengtia
 */ 

 //C:\avrdude\avrdude.exe
 //-C "C:\avrdude\avrdude.conf" -p atmega328p -c arduino -P COM6 -b 115200 -U flash:w:"$(ProjectDir)Debug\$(TargetName).hex":i
 
#include <avr/io.h>

int main(void)
{
	// Set PB5 (pin 13) as OUTPUT set PB0 as input
	DDRB|=(1 << DDB5);	
	
    /* Replace with your application code */
    while (1) 
    {
		// Set pin 13 to HIGH. This is equivalent to digitalWrite(13, HIGH).
		if(PINB & (1 << PINB0))
			PORTB &= ~(1<<DDB5);
		else
			PORTB |= (1<<DDB5);
    }
}