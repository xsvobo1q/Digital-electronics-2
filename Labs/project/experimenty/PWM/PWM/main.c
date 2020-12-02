/*
 * PWM.c
 *
 * Created: 01.12.2020 23:58:51
 * Author : Marek
 */ 

#include <avr/io.h>
#include "gpio.h"

#define PWM_pin PD6


int main(void)
{
    /* Replace with your application code */
	GPIO_config_output(&DDRD, PWM_pin);
	OCR0A = 230;	// nastaveni komparovaci urovne
	TCCR0A |= ((1<<COM0A1) | (1<<COM0A0));	// nastaveni invertujiciho modu
	TCCR0A |= (1 << WGM01) | (1 << WGM00);  // nastaveni Fast PWM modu
	TCCR0B |= ((1<<CS02) | (1<<CS00));	// prescaler citace
	TCCR0B &= ~(1<<CS01);	// prscaler citace
	
//	TIMSK0 |= 1<<OCIE0A;	// povoleni preruseni pri shode
	
    while (1) 
    {
    }
}

