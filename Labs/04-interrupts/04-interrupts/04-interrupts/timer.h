#ifndef TIMER_H
#define TIMER_H

/***********************************************************************
 * 
 * Timer library for AVR-GCC.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2019-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

/**
 * @file  timer.h
 * @brief Timer library for AVR-GCC.
 *
 * @details
 * The library contains macros for controlling the timer modules.
 *
 * @note
 * Based on Microchip Atmel ATmega328P manual and no source file is 
 * needed for the library.
 * 
 * @copyright (c) 2019-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>

/* Defines -----------------------------------------------------------*/
/*Timer0-----------------------------*/
#define TIM0_stop()				TCCR0B &= ~((1<<CS02) | (1<<CS01) | (1<<CS00));
#define TIM0_overflow_16u()		TCCR0B &= ~((1<<CS02) | (1<<CS01)); TCCR0B |= (1<<CS00);
#define TIM0_overflow_128u()	TCCR0B &= ~((1<<CS02) | (1<<CS00)); TCCR0B |= (1<<CS01);
#define TIM0_overflow_1m()		TCCR0B &= ~(1<<CS02); TCCR0B |= ((1<<CS01) | (1<<CS00));
#define TIM0_overflow_4m()		TCCR0B &= ~((1<<CS01) | (1<<CS00)); TCCR0B |= (1<<CS02); 
#define TIM0_overflow_16m()		TCCR0B &= ~(1<<CS01); TCCR0B |= ((1<<CS02) | (1<<CS00));
#define TIM0_overflow_interrupt_enable()    TIMSK0 |= (1<<TOIE0);
#define TIM0_overflow_interrupt_disable()   TIMSK0 &= ~(1<<TOIE0);  
/*Timer0-----------------------------*/

/*Timer2-----------------------------*/
#define TIM2_stop()				TCCR2B &= ~((1<<CS22) | (1<<CS21) | (1<<CS20));
#define TIM2_overflow_16u()		TCCR2B &= ~((1<<CS22) | (1<<CS21)); TCCR0B |= (1<<CS20);
#define TIM2_overflow_128u()	TCCR2B &= ~((1<<CS22) | (1<<CS20)); TCCR0B |= (1<<CS21);
#define TIM2_overflow_512u()	TCCR2B &= ~(1<<CS22); TCCR2B |= ((1<<CS21) | (1<<CS20));
#define TIM2_overflow_1m()		TCCR2B &= ~((1<<CS21) | (1<CS20)); TCCR0B |= (1<<CS22);
#define TIM2_overflow_2m()		TCCR2B &= ~(1<<CS21); TCCR2B |= ((1<<CS22) | (1<CS20));
#define TIM2_overflow_4m()		TCCR2B &= ~(1<<CS20); TCCR0B |= ((1<<CS22) | (1<<CS21));
#define TIM2_overflow_16m()		TCCR2B |= ((1<<CS22) | (1<<CS21) | (1<<CS20));
#define TIM2_overflow_interrupt_enable()    TIMSK2 |= (1<<TOIE2);
#define TIM2_overflow_interrupt_disable()   TIMSK2 &= ~(1<<TOIE2);
/*Timer2-----------------------------*/

/**
 * @brief Defines prescaler CPU frequency values for Timer/Counter1.
 * @note  F_CPU = 16 MHz
 */
#define TIM1_stop()             TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
#define TIM1_overflow_4ms()     TCCR1B &= ~((1<<CS12) | (1<<CS11)); TCCR1B |= (1<<CS10);
#define TIM1_overflow_33ms()    TCCR1B &= ~((1<<CS12) | (1<<CS10)); TCCR1B |= (1<<CS11);
#define TIM1_overflow_262ms()   TCCR1B &= ~(1<<CS12); TCCR1B |= (1<<CS11) | (1<<CS10);
#define TIM1_overflow_1s()      TCCR1B &= ~((1<<CS11) | (1<<CS10)); TCCR1B |= (1<<CS12);
#define TIM1_overflow_4s()      TCCR1B &= ~(1<<CS11); TCCR1B |= (1<<CS12) | (1<<CS10);

/**
 * @brief Defines interrupt enable/disable modes for Timer/Counter1.
 */
#define TIM1_overflow_interrupt_enable()    TIMSK1 |= (1<<TOIE1);
#define TIM1_overflow_interrupt_disable()   TIMSK1 &= ~(1<<TOIE1);

#endif