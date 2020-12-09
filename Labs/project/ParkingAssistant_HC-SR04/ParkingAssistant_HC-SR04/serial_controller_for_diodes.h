/*
 * serial_controller_for_diodes.h
 *
 * Created: 23.11.2020 14:18:37
 *  Author: Dusek,Svoboda
 */ 

/**
 * @file
 * @defgroup our_LED Bargraphs controller <serial_controller_for_diodes.h>
 * @code #include <serial_controller_for_diodes.h> @endcode
 * @code #include <lcd_definitions.h> @endcode
 * @brief Basic routines for interfacing two bargraphs.
 *
 * @author Filip Dusek, Marek Svoboda
 */


#ifndef SERIAL_CONTROLLER_FOR_DIODES_H_
#define SERIAL_CONTROLLER_FOR_DIODES_H_
/**@{*/
/** 
* @name Connection of hardware.
*/
#define DATA_INPUT_FRONT		 PC0
#define DATA_INPUT_REAR			 PC3
#define SEGMENT_LATCH_BAR_1		 PC1
#define SEGMENT_LATCH_BAR_2		 PC4
#define SEGMENT_CLK				 PC2


//declaration init. function

/**
* @name Functions.

*/
/**
 * @brief    Initialization of bargraphs driver
 * @return   none
 */
void DIODE_init(void);

//declaration functions 

/**
 * @brief    Displaying front distance by bargraph.
 * @param    number_of_diodes_bar_1 number of LEDs that should be displayed
 * @return   none
 */
void DIODE_FRONT(uint8_t number_of_diodes_bar_1);

/**
 * @brief    Displaying rear distance by bargraph.
 * @param    number_of_diodes_bar_2 number of LEDs that should be displayed
 * @return   none
 */
void DIODE_REAR(uint8_t number_of_diodes_bar_2);

/**@}*/

#endif /* SERIAL_CONTROLLER_FOR_DIODES_H_ */