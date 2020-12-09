/*
 * serial_controller_for_diodes.c
 *
 * Created: 23.11.2020 13:41:32
 *  Author: Dusek,Svoboda
 */ 
/* Includes ----------------------------------------------------------*/
#define F_CPU 16000000
#include <util/delay.h>
#include "gpio.h"
#include "serial_controller_for_diodes.h"

/* Variables ---------------------------------------------------------*/


// Active-high position 0 to 8
uint8_t diode_number[] = {
	// d7d6d5d4....
	0b00000000,
	0b00000001,   
	0b00000011,   
	0b00000111,	  
	0b00001111,  
	0b00011111,  
	0b00111111,  
	0b01111111,	  
	0b11111111};  




/* Function definitions ----------------------------------------------*/
void DIODE_init(void)
{
	/* Configuration of SSD signals */
	GPIO_config_output(&DDRC, DATA_INPUT_REAR);
	GPIO_config_output(&DDRC, DATA_INPUT_FRONT);
	GPIO_config_output(&DDRC, SEGMENT_CLK);
	GPIO_config_output(&DDRC, SEGMENT_LATCH_BAR_1);
	GPIO_config_output(&DDRC, SEGMENT_LATCH_BAR_2);

}

/*--------------------------------------------------------------------*/
void DIODE_FRONT(uint8_t number_of_diodes_bar_1)
{
	uint8_t bit_number_diodes_1;
	number_of_diodes_bar_1 = diode_number[number_of_diodes_bar_1];
	
	// 
	GPIO_write_low(&PORTC,DATA_INPUT_FRONT);
	GPIO_write_low(&PORTC, SEGMENT_CLK);
	GPIO_write_low(&PORTC, SEGMENT_LATCH_BAR_1);	
	// Wait 1 us
	_delay_us(1);
	

	for (bit_number_diodes_1 = 0; bit_number_diodes_1 < 8; bit_number_diodes_1++)
	{
		// Output DATA value (bit 0 of "segments")
		if ((number_of_diodes_bar_1 & 1) == 0)
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
		 		
		number_of_diodes_bar_1 = number_of_diodes_bar_1 >> 1;
		
	}
	

// 	// Pull LATCH high
 	GPIO_write_high(&PORTC,SEGMENT_LATCH_BAR_1);

	// Wait 1 us
	_delay_us(1);

}

/*--------------------------------------------------------------------*/
void DIODE_REAR(uint8_t number_of_diodes_bar_2)
{
	uint8_t bit_number_diodes_2;
	number_of_diodes_bar_2 = diode_number[number_of_diodes_bar_2];
	
	
	//
	GPIO_write_low(&PORTC,DATA_INPUT_REAR);
	GPIO_write_low(&PORTC, SEGMENT_CLK);
	GPIO_write_low(&PORTC, SEGMENT_LATCH_BAR_2);
	// Wait 1 us
	_delay_us(1);
	

	for (bit_number_diodes_2 = 0; bit_number_diodes_2 < 8; bit_number_diodes_2++)
	{
		// Output DATA value (bit 0 of "segments")
		if ((number_of_diodes_bar_2 & 1) == 0)
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
		
		number_of_diodes_bar_2 = number_of_diodes_bar_2 >> 1;
		
	}
	
	
	// 	// Pull LATCH high
	GPIO_write_high(&PORTC,SEGMENT_LATCH_BAR_2);

	// Wait 1 us
	_delay_us(1);

}