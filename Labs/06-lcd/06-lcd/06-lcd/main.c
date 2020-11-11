/***********************************************************************
 * 
 * Stopwatch with LCD display output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function
#define F_CPU 16000000
#include <util/delay.h>

/* Variables ---------------------------------------------------------*/
// Custom character definition using https://omerk.github.io/lcdchargen/

uint8_t barGraph0[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

uint8_t barGraph1[8] = {
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b10000,
	0b10000
};

uint8_t barGraph2[8] = {
	0b11000,
	0b11000,
	0b11000,
	0b11000,
	0b11000,
	0b11000,
	0b11000,
	0b11000
};

uint8_t barGraph3[8] = {
	0b11100,
	0b11100,
	0b11100,
	0b11100,
	0b11100,
	0b11100,
	0b11100,
	0b11100	
};

uint8_t barGraph4[8] = {
	0b11110,
	0b11110,
	0b11110,
	0b11110,
	0b11110,
	0b11110,
	0b11110,
	0b11110
};

uint8_t barGraph5[8] = {
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};

char ownText[] = "Muj velmi dlouhy a velmi pomalu bezici text...";

int main(void)
{
	// Initialize LCD display
	lcd_init(LCD_DISP_ON);

	// Set pointer to beginning of CGRAM memory
	lcd_command(1 << LCD_CGRAM);
	for (uint8_t i = 0; i < 8; i++)
	{
		lcd_data(barGraph0[i]);
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		lcd_data(barGraph1[i]);
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		lcd_data(barGraph2[i]);
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		lcd_data(barGraph3[i]);
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		lcd_data(barGraph4[i]);
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		lcd_data(barGraph5[i]);
	}
	// Set DDRAM address
	lcd_command(1 << LCD_DDRAM);

    // Put string(s) at LCD display
    lcd_gotoxy(1, 0);
    lcd_puts("00:00.0");
	lcd_gotoxy(11,0);
	lcd_putc('a');
	lcd_gotoxy(1,1);
	lcd_putc('b');
	lcd_gotoxy(11,1);
	lcd_putc('c');

    // Configure 8-bit Timer/Counter2 for Stopwatch
    // Set prescaler and enable overflow interrupt every 16 ms
	TIM2_overflow_16ms();
	TIM2_overflow_interrupt_enable();
	
	// Configuration of Timer/Counter0
	TIM0_overflow_16ms();
	TIM0_overflow_interrupt_enable();


    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter2 overflows. Update the stopwatch on
 * LCD display every sixth overflow, ie approximately every 100 ms
 * (6 x 16 ms = 100 ms).
 */
ISR(TIMER2_OVF_vect)
{
    static uint8_t number_of_overflows = 0;
	static uint8_t tens = 0;        // Tenths of a second
	static uint8_t secs = 0;        // Seconds
	static uint8_t minutes = 0;     // Minutes
	char lcd_string[2] = "  ";      // String for converting numbers by itoa()
	
    number_of_overflows++;
    if (number_of_overflows >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        number_of_overflows = 0;
		tens++;
		if (tens == 10)
		{
			tens = 0;
			secs++;
			if (secs == 60)
			{
				secs = 0;
				lcd_gotoxy(11, 0);
				lcd_puts("    ");
				minutes++;
				if (minutes == 60)
				{
					minutes = 0;
				}
				itoa(minutes, lcd_string, 10);
				lcd_gotoxy(1, 0);
				lcd_puts(lcd_string);
				/*itoa(minutes/10, lcd_string, 10);
				lcd_gotoxy(1, 0);
				lcd_puts(lcd_string);
				itoa(minutes%10, lcd_string, 10);
				lcd_gotoxy(2, 0);
				lcd_puts(lcd_string);*/
			}
			itoa(secs, lcd_string, 10);
			lcd_gotoxy(4, 0);
			lcd_puts(lcd_string);
			/*itoa(secs/10, lcd_string, 10);
			lcd_gotoxy(4, 0);
			lcd_puts(lcd_string);
			itoa(secs%10, lcd_string, 10);
			lcd_gotoxy(5, 0);
			lcd_puts(lcd_string);*/
			
			itoa(pow(secs, 2), lcd_string, 10);
			lcd_gotoxy(11, 0);
			lcd_puts(lcd_string);

		}
		itoa(tens, lcd_string, 10);
		lcd_gotoxy(7, 0);
		lcd_puts(lcd_string);
    }
}


ISR(TIMER0_OVF_vect)
{
	static uint8_t symbol = 0;
	static uint8_t position = 0;
	
	symbol++;
	if (symbol > 5)
	{
		symbol = 0;
		position++;
		if (position > 9)
		{
			position = 0;
			lcd_gotoxy(1+position, 1);
			lcd_puts("          ");
		}
	}
	lcd_gotoxy(1+position, 1);
	lcd_putc(symbol);		
}
