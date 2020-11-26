/*
 * serial_controller_for_diodes.c
 *
 * Created: 23.11.2020 13:41:32
 *  Author: XXX
 */ 
/* Includes ----------------------------------------------------------*/
#define F_CPU 16000000
#include <util/delay.h>
#include "gpio.h"
#include "serial_controller_for_diodes.h"


/* Variables ---------------------------------------------------------*/


// Active-high position 0 to 7
uint8_t diode_number[] = {
	// p3p2p1p0....
	0b00000001,   // Position 0 = 2 cm
	0b00000011,   // Position 1 = 4 cm 
	0b00000111,	  // Position 2 = 6 cm
	0b00001111,   // Position 3 = 8 cm
	0b00011111,   // Position 4 = 10 cm
	0b00111111,   // Position 5 = 12 cm
	0b01111111,	  // Position 6 = 14 cm
	0b11111111};   // Position 7 = 16 cm

uint8_t diode_number1[] = {
	// p3p2p1p0....
	0b00000001,   // Position 0 = 2 cm
	0b00000011,   // Position 1 = 4 cm
	0b00000111,	  // Position 2 = 6 cm
	0b00001111,   // Position 3 = 8 cm
	0b00011111,   // Position 4 = 10 cm
	0b00111111,   // Position 5 = 12 cm
	0b01111111,	  // Position 6 = 14 cm
	0b11111111};   // Position 7 = 16 cm





/* Function definitions ----------------------------------------------*/
void DIODE_init(void)
{
	/* Configuration of SSD signals */
	GPIO_config_output(&DDRC, DATA_INPUT_REAR);
	GPIO_config_output(&DDRC, DATA_INPUT_FRONT);
	GPIO_config_output(&DDRC, SEGMENT_CLK);
	GPIO_config_output(&DDRC, SEGMENT_LATCH);

}

/*--------------------------------------------------------------------*/
void DIODE_update_shift_regs_FRONT(uint8_t number)
{
	uint8_t bit_number;
	number = diode_number[number];
/*	row = diode_row[row];*/
	
	// 
	GPIO_write_low(&PORTC,DATA_INPUT_FRONT);
	GPIO_write_low(&PORTC, SEGMENT_CLK);
	GPIO_write_low(&PORTC, SEGMENT_LATCH);	
	// Wait 1 us
	_delay_us(1);
	

	for (bit_number = 0; bit_number < 8; bit_number++)
	{
		// Output DATA value (bit 0 of "segments")
		if ((number & 1) == 0)
		{
			GPIO_write_low(&PORTC,DATA_INPUT_FRONT);
		}
		else
		{
			GPIO_write_high(&PORTC, DATA_INPUT_FRONT);
		}

 		// Wait 1 us
 		_delay_us(1);
 		// Pull CLK high
 		GPIO_write_high(&PORTC,SEGMENT_CLK);
 		// Wait 1 us
 		_delay_us(1);
 		// Pull CLK low
 		GPIO_write_low(&PORTC,SEGMENT_CLK);
		 		
		number = number >> 1;
		
	}
	
	
// 	for (bit_number = 0; bit_number < 8; bit_number++)
// 	{
// 		// Output DATA value (bit 0 of "segments")
// 		if ((number & 1) == 0)
// 		{
// 			GPIO_write_low(&PORTC,DATA_INPUT_REAR);
// 		}
// 		else
// 		{
// 			GPIO_write_high(&PORTC, DATA_INPUT_REAR);
// 		}
// 
// 		// Wait 1 us
// 		_delay_us(1);
// 		// Pull CLK high
// 		GPIO_write_high(&PORTC,SEGMENT_CLK);
// 		// Wait 1 us
// 		_delay_us(1);
// 		// Pull CLK low
// 		GPIO_write_low(&PORTC,SEGMENT_CLK);
// 			
// 		number = number >> 1;
// 			
// 
// 
// 		}

// 	// Pull LATCH high
 	GPIO_write_high(&PORTC,SEGMENT_LATCH);

	// Wait 1 us
	_delay_us(1);

}

/*--------------------------------------------------------------------*/
void DIODE_update_shift_regs_REAR(uint8_t number1)
{
	uint8_t bit_number;
	number1 = diode_number1[number1];
	/*	row = diode_row[row];*/
	
	//
	GPIO_write_low(&PORTC,DATA_INPUT_REAR);
	GPIO_write_low(&PORTC, SEGMENT_CLK);
	GPIO_write_low(&PORTC, SEGMENT_LATCH);
	// Wait 1 us
	_delay_us(1);
	

	for (bit_number = 0; bit_number < 8; bit_number++)
	{
		// Output DATA value (bit 0 of "segments")
		if ((number1 & 1) == 0)
		{
			GPIO_write_low(&PORTC,DATA_INPUT_REAR);
		}
		else
		{
			GPIO_write_high(&PORTC, DATA_INPUT_REAR);
		}

		// Wait 1 us
		_delay_us(1);
		// Pull CLK high
		GPIO_write_high(&PORTC,SEGMENT_CLK);
		// Wait 1 us
		_delay_us(1);
		// Pull CLK low
		GPIO_write_low(&PORTC,SEGMENT_CLK);
		
		number1 = number1 >> 1;
		
	}
	
	
	// 	// Pull LATCH high
	GPIO_write_high(&PORTC,SEGMENT_LATCH);

	// Wait 1 us
	_delay_us(1);

}
/*--------------------------------------------------------------------*/
/* SEG_clear */

/*--------------------------------------------------------------------*/
/* SEG_clk_2us */