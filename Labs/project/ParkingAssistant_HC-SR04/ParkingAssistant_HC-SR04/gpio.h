#ifndef GPIO_H
#define GPIO_H

/***********************************************************************
 * 
 * GPIO library for AVR-GCC.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2019-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

/**
 * @mainpage
 * @author Filip Dusek
 * @author Marek Svoboda
 * 
 * @file  gpio.h
 *
 * @file gpio.h
 * @defgroup gpio GPIO <gpio.h>
 * @code #include <gpio.h> @endcode
 * @brief Library for basic pin operation as set pin as OUT/IN...
 *
 * @details
 * The library contains functions for controlling AVRs' gpio pin(s).
 *
 * @note
 * Based on AVR Libc Reference Manual. Tested on ATmega328P (Arduino Uno),
 * 16 MHz, AVR 8-bit Toolchain 3.6.2.
 * 
 * @copyright (c) 2019-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>

/* Function prototypes -----------------------------------------------*/

/**@{*/

/**
 * @brief Configure one output pin in Data Direction Register.
 * @param reg_name - Address of Data Direction Register, such as &DDRA,
 *                   &DDRB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
 */
void GPIO_config_output(volatile uint8_t *reg_name, uint8_t pin_num);

/* Function prototypes -----------------------------------------------*/
/**
 * @brief Configure one input pin in Data Direction Register without pull-up resistor.
 * @param reg_name - Address of Data Direction Register, such as &DDRA,
 *                   &DDRB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
 */
void GPIO_config_input_nopull(volatile uint8_t *reg_name, uint8_t pin_num);

/* Function prototypes -----------------------------------------------*/
/**
 * @brief Configure one input pin in Data Direction Register with pull-up resistor.
 * @param reg_name - Address of Data Direction Register, such as &DDRA,
 *                   &DDRB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
 */
void GPIO_config_input_pullup(volatile uint8_t *reg_name, uint8_t pin_num);

/* Function prototypes -----------------------------------------------*/
/**
 * @brief Set output to LOW.
 * @param reg_name - Address of Data Direction Register, such as &DDRA,
 *                   &DDRB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
 */
void GPIO_write_low(volatile uint8_t *reg_name, uint8_t pin_num);

/* Function prototypes -----------------------------------------------*/
/**
 * @brief Set output to HIGH.
 * @param reg_name - Address of Data Direction Register, such as &DDRA,
 *                   &DDRB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
 */
void GPIO_write_high(volatile uint8_t *reg_name, uint8_t pin_num);

/* Function prototypes -----------------------------------------------*/
/**
 * @brief Change state of output (LOW to HIGH or HIGH to LOW).
 * @param reg_name - Address of Data Direction Register, such as &DDRA,
 *                   &DDRB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
 */
void GPIO_toggle(volatile uint8_t *reg_name, uint8_t pin_num);

/* Function prototypes -----------------------------------------------*/
/**
 * @brief Read value of one input pin.
 * @param reg_name - Address of Data Direction Register, such as &DDRA,
 *                   &DDRB, ...
 * @param pin_num - Pin designation in the interval 0 to 7
 */
uint8_t GPIO_read(volatile uint8_t *reg_name, uint8_t pin_num);

/**@}*/

#endif