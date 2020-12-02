/*
 * ParkingAssistant_HC-SR04.c
 *
 * Created: 18.11.2020 12:46:06
 * Author : Marek
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "gpio.h"
#include "uart.h"
#include "serial_controller_for_diodes.h"
#include "lcd.h"
#include "lcd_definitions.h"
#include "timer.h"
#include <stdbool.h>

#define sensTrigFront PB2
#define sensEchoFront PD3

#define sensTrigRear PB3
#define sensEchoRear PD2

#define buzzer PB4

volatile int beepTiming = 0;
volatile uint8_t trigEnable = 1;
volatile uint8_t averaging = 0;
volatile float distanceFront = 0;
volatile float distanceRear = 0;
uint8_t mux = 1;
bool help = true;

int displayValues(float distance, float distanceFront);

int main(void)
{
	lcd_init(LCD_DISP_ON);
	lcd_gotoxy(5, 0);
	lcd_puts("Rid at");
	lcd_gotoxy(2, 1);
	lcd_puts("jedes lip...");
	_delay_ms(1000);
	lcd_clrscr();
	lcd_puts("Front:");
	lcd_gotoxy(0, 1);
	lcd_puts("Rear:");
	lcd_gotoxy(14, 0);
	lcd_puts("cm");
	lcd_gotoxy(14, 1);
	lcd_puts("cm");

	DIODE_init();
	
	uart_init(UART_BAUD_SELECT(9600, F_CPU));
	
	GPIO_config_input_nopull(&DDRD, sensEchoFront);
	GPIO_config_input_nopull(&DDRD, sensEchoRear);
	
	GPIO_config_output(&DDRB, sensTrigFront);
	GPIO_config_output(&DDRB, sensTrigRear);
	
	GPIO_write_low(&PORTB, sensTrigFront);
	GPIO_write_low(&PORTB, sensTrigRear);
	
	GPIO_config_output(&DDRB, buzzer);
	GPIO_write_low(&PORTB, buzzer);
	
	TIM1_overflow_1s();
	TIM1_overflow_interrupt_enable();
	
	EIMSK |= (1<<INT1);	// interrupt enable on INT1
	EICRA &= ~(1<<ISC11); EICRA |= (1<<ISC10);
	EIMSK |= (1<<INT0);	// interrupt enable on INT0
	EICRA &= ~(1<<ISC01); EICRA |= (1<<ISC00);
	
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		if ((mux == 1) & (trigEnable == 1))
		{	
			trigEnable = 0;
			help = true;
			_delay_us(50);
			GPIO_write_high(&PORTB, sensTrigFront);
			_delay_us(10);
			GPIO_write_low(&PORTB, sensTrigFront);
			
 			if (averaging == 4)
 			{
				distanceFront = distanceFront/averaging;
	 			averaging = 0;
	 			mux++;
 			}
			
		}
		if ((mux == 2) & (trigEnable == 1))
		{
			trigEnable = 0;
			help = true;
			_delay_us(50);
			GPIO_write_high(&PORTB, sensTrigRear);
			_delay_us(10);
			GPIO_write_low(&PORTB, sensTrigRear);		
			
			if (averaging == 4){
				distanceRear = distanceRear/averaging;
				averaging = 0;	
				mux++;		
			}
			
		}
		if (mux == 3)
		{
			beepTiming = displayValues(distanceRear, distanceFront);
			distanceFront = 0;
			distanceRear = 0;
			mux = 1;
		}
		
    }
}

ISR(INT1_vect){
	
	if (help){
		TIM0_overflow_16ms();
		help = false;
	}else{
		TIM0_stop();
		distanceFront += TCNT0;
		TCNT0 = 0;
		averaging++;
		trigEnable = 1;
		help = true;
	}
	
}

ISR(INT0_vect){

	if (help){
		TIM0_overflow_16ms();
		help = false;
	}else{
		TIM0_stop();
		distanceRear += TCNT0;
		TCNT0 = 0;
		averaging++;
		trigEnable = 1;
		help = true;
	}

}

ISR(TIMER1_OVF_vect){
	GPIO_toggle(&PORTB, buzzer);
	TCNT1 = beepTiming;
}

int displayValues(float distanceRear, float distanceFront){
	
	char uartString[50];
	char dispString[50];
	uint16_t dist = 0;	

	distanceRear = distanceRear/3;
	distanceFront = distanceFront/3;
	
	if (distanceRear < distanceFront)
	{
		dist = distanceRear;
	} 
	else
	{
		dist = distanceFront;
	}
	
	if ((distanceFront < 7) | (distanceRear < 7))
	{
		GPIO_write_high(&PORTB, buzzer);
		TIM1_overflow_interrupt_disable();
	} 
	else if((distanceFront > 40) & (distanceRear > 40))
	{
		GPIO_write_low(&PORTB, buzzer);
		TIM1_overflow_interrupt_disable();
	}else{
		TIM1_overflow_interrupt_enable();
	}

	sprintf(uartString,"Front: %0.2lf  ||  Rear: %0.2lf \r\n",distanceFront, distanceRear);
	uart_puts(uartString);
	
	sprintf(dispString,"%0.2f",distanceFront);
	lcd_gotoxy(8, 0);
	lcd_puts("      ");
	lcd_gotoxy(8, 0);
	lcd_puts(dispString);
	
	sprintf(dispString,"%0.2f",distanceRear);
	lcd_gotoxy(8, 1);
	lcd_puts("      ");
	lcd_gotoxy(8, 1);
	lcd_puts(dispString);
	
	if (distanceFront < 7)
	{
		DIODE_update_shift_regs_FRONT(7);
	} else if((distanceFront > 7) & (distanceFront < 10))
	{
		DIODE_update_shift_regs_FRONT(6);
	} else if((distanceFront > 10) & (distanceFront < 15))
	{
		DIODE_update_shift_regs_FRONT(5);
	} else if((distanceFront > 15) & (distanceFront < 20))
	{
		DIODE_update_shift_regs_FRONT(4);
		beepTiming = 52000;
	} else if((distanceFront > 20) & (distanceFront < 25))
	{
		DIODE_update_shift_regs_FRONT(3);
		beepTiming = 46000;
	} else if((distanceFront > 25) & (distanceFront < 30))
	{
		DIODE_update_shift_regs_FRONT(2);
		beepTiming = 40000;
	} else if((distanceFront > 30) & (distanceFront < 35))	
	{
		DIODE_update_shift_regs_FRONT(1);
		beepTiming = 34000;
	} else
	{
		DIODE_update_shift_regs_FRONT(0);
		beepTiming = 28000;
	}
	
	
	if (distanceRear < 5)
	{
		DIODE_update_shift_regs_REAR(7);
	} else if((distanceRear > 5) & (distanceRear < 10))
	{
		DIODE_update_shift_regs_REAR(6);
	} else if((distanceRear > 10) & (distanceRear < 15))
	{
		DIODE_update_shift_regs_REAR(5);
	} else if((distanceRear > 15) & (distanceRear < 20))
	{
		DIODE_update_shift_regs_REAR(4);
	} else if((distanceRear > 20) & (distanceRear < 25))
	{
		DIODE_update_shift_regs_REAR(3);
	} else if((distanceRear > 25) & (distanceRear < 30))
	{
		DIODE_update_shift_regs_REAR(2);
	} else if((distanceRear > 30) & (distanceRear < 35))
	{
		DIODE_update_shift_regs_REAR(1);
	} else 
	{
		DIODE_update_shift_regs_REAR(0);
	}
	
	if ((dist > 7) & (dist < 10))
	{
		beepTiming = 62000;
	}else if ((dist > 10) & (dist < 15))
	{
		beepTiming = 58000;
	}else if ((dist > 15) & (dist < 20))
	{
		beepTiming = 52000;
	}else if ((dist > 20) & (dist < 25))
	{
		beepTiming = 46000;
	}else if ((dist > 25) & (dist < 30))
	{
		beepTiming = 40000;
	}else if ((dist > 30) & (dist < 35))
	{
		beepTiming = 34000;
	}else
	{
		beepTiming = 28000;
	}

	return beepTiming;
	
}