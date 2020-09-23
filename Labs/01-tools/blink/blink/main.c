/***********************************************************************
 * 
 * Blink a LED and use the function from the delay library.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_GREEN   PB5     // AVR pin where green LED is connected
#define UNIT_DELAY 200
#define DOT UNIT_DELAY
#define DASH UNIT_DELAY*3
#define MARK_PAUSE UNIT_DELAY
#define LETTER_PAUSE UNIT_DELAY*3
#define WORD_PAUSE UNIT_DELAY*7
#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay func
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions

/* Variables ---------------------------------------------------------*/



/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
 * Toggle one LED and use the function from the delay library.
 */
int main(void)
{

    // Set pin as output in Data Direction Register
    // DDRB = DDRB or 0010 0000
    DDRB = DDRB | (1<<LED_GREEN);

    // Set pin LOW in Data Register (LED off)
    // PORTB = PORTB and 1101 1111
    PORTB = PORTB & ~(1<<LED_GREEN);

    // Infinite loop
    while (1)
    {
		// D
        PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(DASH); // dash length is threefold as length of dot
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(MARK_PAUSE); // pause between marks is the same length as length of dot 
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(DOT);
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(MARK_PAUSE);
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(DOT);
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(LETTER_PAUSE); // pause between letters is threefold of dot length
		
		// E
		
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(DOT);
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(LETTER_PAUSE);
		
		// 2
		
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(DOT);
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(MARK_PAUSE);
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(DOT);
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(MARK_PAUSE);
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(DASH);
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(MARK_PAUSE);
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(DASH);
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(MARK_PAUSE);
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(DASH);
		PORTB = PORTB ^ (1<<LED_GREEN);
		_delay_ms(WORD_PAUSE); // pause between words is sevenfold as length of dot
    }

    // Will never reach this
    return 0;
}

/* Interrupt routines ------------------------------------------------*/
