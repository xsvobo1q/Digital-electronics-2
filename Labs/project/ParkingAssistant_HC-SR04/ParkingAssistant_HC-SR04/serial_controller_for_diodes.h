/*
 * serial_controller_for_diodes.h
 *
 * Created: 23.11.2020 14:18:37
 *  Author: Dusek,Svoboda
 */ 


#ifndef SERIAL_CONTROLLER_FOR_DIODES_H_
#define SERIAL_CONTROLLER_FOR_DIODES_H_

//define pins
#define DATA_INPUT_FRONT		 PC0
#define DATA_INPUT_REAR			 PC3
#define SEGMENT_LATCH_BAR_1		 PC1
#define SEGMENT_LATCH_BAR_2		 PC4
#define SEGMENT_CLK				 PC2


//declaration init. function
void DIODE_init(void);

//declaration functions 
void DIODE_FRONT(uint8_t number_of_diodes_bar_1);
void DIODE_REAR(uint8_t number_of_diodes_bar_2);

#endif /* SERIAL_CONTROLLER_FOR_DIODES_H_ */