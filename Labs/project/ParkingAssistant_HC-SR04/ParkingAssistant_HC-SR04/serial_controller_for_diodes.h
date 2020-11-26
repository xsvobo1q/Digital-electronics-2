/*
 * serial_controller_for_diodes.h
 *
 * Created: 23.11.2020 14:18:37
 *  Author: XXX
 */ 


#ifndef SERIAL_CONTROLLER_FOR_DIODES_H_
#define SERIAL_CONTROLLER_FOR_DIODES_H_

#define DATA_INPUT_FRONT     PC0
#define DATA_INPUT_REAR		 PC3
#define SEGMENT_LATCH   PC1
#define SEGMENT_CLK     PC2

void DIODE_init(void);

/**
 * @brief Display segments at one position of the SSD.
 * @param segments - Segments to be displayed (abcdefgDP, active low)
 * @param position - Position of the display where the segments is to 
 *                   be displayed (p3 p2 p1 p0 ...., active high)
 * @note  Two shift registers are connected in series, ie 16 bits are 
 *        transmitted.
 */
void DIODE_update_shift_regs_FRONT(uint8_t number);
void DIODE_update_shift_regs_REAR(uint8_t number1);

/* SEG_clear */

/* SEG_clk_2us */

#endif /* SERIAL_CONTROLLER_FOR_DIODES_H_ */