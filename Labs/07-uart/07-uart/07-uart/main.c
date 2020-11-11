/***********************************************************************
 * 
 * Analog-to-digital conversion with displaying result on LCD and 
 * transmitting via UART.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2020 Tomas Fryza
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
#include "uart.h"           // Peter Fleury's UART library

#ifndef F_CPU
#define F_CPU 16000000
#endif

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Use Timer/Counter1
 * and start ADC conversion four times per second. Send value to LCD
 * and UART.
 */
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0);
    lcd_puts("value:");
    lcd_gotoxy(3, 1);
    lcd_puts("key:");

    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);
    // Set input channet to ADC0
	ADMUX &= ~(1<<MUX3);
	ADMUX &= ~(1<<MUX2);
	ADMUX &= ~(1<<MUX1);
	ADMUX &= ~(1<<MUX0);
    // Enable ADC module
	ADCSRA |= (1 << ADEN);
    // Enable conversion complete interrupt
	ADCSRA |= (1 << ADIE);
    // Set clock prescaler to 128
	ADCSRA |= (1 << ADPS2);
	ADCSRA |= (1 << ADPS1);
	ADCSRA |= (1 << ADPS0);
    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Enable interrupt and set the overflow prescaler to 262 ms
	ADCSRB |= (1 << ADTS2);
	ADCSRB |= (1 << ADTS1);
	ADCSRB |= ~(1 << ADTS0);
	TIM1_overflow_interrupt_enable();
	TIM1_overflow_262ms();
    // Initialize UART to asynchronous, 8N1, 9600 Bauds
	uart_init(UART_BAUD_SELECT(9600, F_CPU));
	
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
 * ISR starts when Timer/Counter1 overflows. Use single conversion mode
 * and start conversion four times per second.
 */
ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
	ADCSRA |= (1 << ADSC);
}

/* -------------------------------------------------------------------*/
/* ADC complete interrupt routine.
 * Update LCD and UART transmiter. */
/**
 * ISR starts when ADC complets the convertion. Display value on LCD
 * and send it to UART.
 */
ISR(ADC_vect)
{
	
    // WRITE YOUR CODE HERE
	uint16_t value;
	char lcd_string[5];
	
	value = ADC;
	// Print on LCD in decimal
	itoa(value, lcd_string, 10);
	lcd_gotoxy(8, 0);
	lcd_puts("       ");
	lcd_gotoxy(8, 0);
	lcd_puts(lcd_string);
	
	if(value < 700){
	// Send to UART in decimal
		uart_puts("ADC value in decimal: ");
		uart_puts(lcd_string);
		uart_puts("\n\r");
	}
	
	// Print pressed key
	if (value > 1000)
	{
		lcd_gotoxy(8, 1);
		lcd_puts("       ");
		lcd_gotoxy(8, 1);
		lcd_puts("none");	
	}
	if ((value > 600) && (value < 700))
	{
		lcd_gotoxy(8, 1);
		lcd_puts("       ");
		lcd_gotoxy(8, 1);
		lcd_puts("Select");
	}
	if ((value > 350) && (value < 450))
	{
		lcd_gotoxy(8, 1);
		lcd_puts("       ");
		lcd_gotoxy(8, 1);
		lcd_puts("Left");
	}
	if ((value > 200) && (value < 300))
	{
		lcd_gotoxy(8, 1);
		lcd_puts("       ");
		lcd_gotoxy(8, 1);
		lcd_puts("Down");
	}
	if ((value > 50) && (value < 150))
	{
		lcd_gotoxy(8, 1);
		lcd_puts("       ");
		lcd_gotoxy(8, 1);
		lcd_puts("Up");
	}
	if (value < 50)
	{
		lcd_gotoxy(8, 1);
		lcd_puts("       ");
		lcd_gotoxy(8, 1);
		lcd_puts("Right");
	}
}