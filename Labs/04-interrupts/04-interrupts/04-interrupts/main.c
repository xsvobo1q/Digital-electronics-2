/* Defines -----------------------------------------------------------*/
#define LED_D1  PB5
#define LED_D2  PB4
#define LED_D3  PB3

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

int main(void)
{
    /* Configuration of three LEDs */
    GPIO_config_output(&DDRB, LED_D1);
    GPIO_write_low(&PORTB, LED_D1);
  
    GPIO_config_output(&DDRB, LED_D2);
    GPIO_write_low(&PORTB, LED_D2);
	
	GPIO_config_output(&DDRB, LED_D3);
	GPIO_write_low(&PORTB, LED_D3);
	
	/* Configuration timers and enable interrupt them */
    TIM0_overflow_16us();
    TIM0_overflow_interrupt_enable();

    TIM1_overflow_4ms();
    TIM1_overflow_interrupt_enable();

    TIM2_overflow_2ms();
    TIM2_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
 
    }

 
    return 0;
}

/* Interrupt service routines ----------------------------------------*/

ISR(TIMER0_OVF_vect) /*Interrupt Servis Routine - obsluha preruseni*/
{
	// WRITE YOUR CODE HERE
	GPIO_toggle(&PORTB, LED_D1);
}

ISR(TIMER1_OVF_vect) /*Interrupt Servis Routine - obsluha preruseni*/
{
	// WRITE YOUR CODE HERE
	GPIO_toggle(&PORTB, LED_D2);
}

ISR(TIMER2_OVF_vect) /*Interrupt Servis Routine - obsluha preruseni*/
{
    // WRITE YOUR CODE HERE
	GPIO_toggle(&PORTB, LED_D3);
}