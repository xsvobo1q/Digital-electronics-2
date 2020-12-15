/*
 * ParkingAssistant_HC-SR04.c
 *
 * Created: 18.11.2020 12:46:06
 * Author : Dusek, Svoboda
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

/*          Defines of pins            */
#define sensTrigFront PB2	
#define sensEchoFront PD3

#define sensTrigRear PB3
#define sensEchoRear PD2

#define buzzer PB4

/*          Defines of variables       */
volatile unsigned int beepTiming = 0;
volatile uint8_t trigEnable = 1;
volatile uint8_t averaging = 0;
volatile float distanceFront = 0;
volatile float distanceRear = 0;
volatile float distance = 0;
uint8_t mux = 1;
volatile bool help = true;	// help variable for Echo time measuring
float wdt = 0;				// "watchdog timer" solving no Echo pulse

/*          Declaration of displaying function           */
int displayValues(float distanceRear, float distanceFront);

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

/*    Initialization of bar graphs     */
	DIODE_init();
	
/*    Initialization of UART          */	
	uart_init(UART_BAUD_SELECT(9600, F_CPU));

/*    Pins configuration              */	
	GPIO_config_input_nopull(&DDRD, sensEchoFront);
	GPIO_config_input_nopull(&DDRD, sensEchoRear);
	
	GPIO_config_output(&DDRB, sensTrigFront);
	GPIO_config_output(&DDRB, sensTrigRear);
	
	GPIO_write_low(&PORTB, sensTrigFront);
	GPIO_write_low(&PORTB, sensTrigRear);
	
	GPIO_config_output(&DDRB, buzzer);
	GPIO_write_low(&PORTB, buzzer);
	
/*    Enable overflow from TIMER 1 - buzzer timing    */
	TIM1_overflow_1s();
	TIM1_overflow_interrupt_enable();
	
/*	  Enable overflow from TIMER 0 - measuring of Echo length     */
	TIM0_overflow_16us();
	TIM0_overflow_interrupt_enable();
	
	EIMSK |= (1<<INT1);								// set register to interrupt enable on INT1
	EICRA &= ~(1<<ISC11); EICRA |= (1<<ISC10);		// set register to interrupt when level change is on INT1
	EIMSK |= (1<<INT0);								// set register to interrupt enable on INT0
	EICRA &= ~(1<<ISC01); EICRA |= (1<<ISC00);		// set register to interrupt when level change is on INT0
	
	sei();
	
    while (1) 
    {
		
		/*   Timer solving no Echo pulse - obstacle is too far   */
		if (trigEnable == 0)
		{
			wdt++;
			_delay_ms(1);
			if(wdt > 15)
			{
				if (mux == 1)
				{
					distanceFront = 13000;
				}
				
				if (mux == 2)
				{
					distanceRear = 13000;
				}
				averaging = 0;
	 			trigEnable = 1;
	 			help = true;
				mux++;
				wdt = 0;			
			}
		}else
		{
			wdt = 0;
		}
		
		/*   MULTIPLEXOR   */
		if ((mux == 1) & (trigEnable == 1))
		{	
 			if (averaging >= 5)
 			{
				distanceFront = distanceFront/averaging;
	 			averaging = 0;
 	 			mux++;
				wdt = 0;
 			}else
			{
				trigEnable = 0;
				help = true;
				/* triggering of front sensor */
				_delay_us(50);
				GPIO_write_high(&PORTB, sensTrigFront);
				_delay_us(10);
				GPIO_write_low(&PORTB, sensTrigFront);	 
			 }		
		}
		
		if ((mux == 2) & (trigEnable == 1))
		{	
			if (averaging >= 5)
			{
				distanceRear = distanceRear/averaging;
				averaging = 0;
 				mux++;
				wdt = 0;
			} else
			{
				trigEnable = 0;
				help = true;
				/* triggering of rear sensor */
				_delay_us(50);
				GPIO_write_high(&PORTB, sensTrigRear);
				_delay_us(10);
				GPIO_write_low(&PORTB, sensTrigRear);
			}
		}
		
		if (mux >= 3)
		{
			beepTiming = displayValues(distanceRear, distanceFront);	// function that display values on LCD, UART, LEDs
			distanceFront = 0;												  // and returns value for beep timer
			distanceRear = 0;
			mux = 1;
		}
    }
}

/* ISR for Echo from front sensor */
ISR(INT1_vect)
{
	
	if (help)
	{
		help = false;
	}
	else
	{
 		averaging++;
		trigEnable = 1;
		help = true;
	}
	
}

/* ISR for Echo from rear sensor */
ISR(INT0_vect)
{

	if (help)
	{
		help = false;
	}
	else
	{
		averaging++;
		trigEnable = 1;
		help = true;
	}

}

/*    ISR for measuring Echo pulse   */
ISR(TIMER0_OVF_vect)
{
	if (!help)
	{	
		if (mux == 1)
		{
			distanceFront += 16;
		}else if(mux == 2)
		{
			distanceRear += 16;
		}
	}
}

/*    ISR for timer, that turning buzzer off/on   */
ISR(TIMER1_OVF_vect)
{
	GPIO_toggle(&PORTB, buzzer);
	TCNT1 = beepTiming;
}

int displayValues(float distanceRear, float distanceFront){
	
	/* Help chars for displaying */	
	char uartString[50];		
	char dispString[50];
	float dist = 0;	
	
	/* Calculating distance in cm from time in [us] and speed of sound in [cm/us] */
	distanceRear = (distanceRear/2) * 0.0343;
	distanceFront = (distanceFront/2) * 0.0343;
	
	/* Logic for buzzer */
	if (distanceRear < distanceFront)
	{
		dist = distanceRear;
	} 
	else
	{
		dist = distanceFront;
	}
	
	if ((distanceFront <= 10) | (distanceRear <= 10))
	{
		TIM1_overflow_interrupt_disable();
		GPIO_write_high(&PORTB, buzzer);
	} 
	else if((distanceFront > 50) & (distanceRear > 50))
	{
		TIM1_overflow_interrupt_disable();
		GPIO_write_low(&PORTB, buzzer);
	}
	else
	{
		TIM1_overflow_interrupt_enable();
	}


	// displaying values on display and uart
	
	if (distanceFront > 200)
	{
	 	lcd_gotoxy(7, 0);
	 	lcd_puts("       ");
	 	lcd_gotoxy(7, 0);
	 	lcd_puts(" > 200");
		uart_puts(" Front: > 200cm   ||   ");
	}else
	{
		sprintf(dispString,"%0.1f",distanceFront);
	 	lcd_gotoxy(7, 0);
	 	lcd_puts("       ");
	 	lcd_gotoxy(7, 0);
	 	lcd_puts(dispString);
		sprintf(uartString, "Front: %0.1lf   ||   ", distanceFront);
		uart_puts(uartString);
	}

	if (distanceRear > 200)
	{
	 	lcd_gotoxy(7, 1);
	 	lcd_puts("       ");
	 	lcd_gotoxy(7, 1);
	 	lcd_puts(" > 200");
		uart_puts(" Rear: > 200cm\r\n");
	} 
	else
	{
		sprintf(dispString,"%0.1f",distanceRear);
		lcd_gotoxy(7, 1);
		lcd_puts("       ");
		lcd_gotoxy(7, 1);
		lcd_puts(dispString);
		sprintf(uartString, "Rear: %0.1lf \r\n", distanceRear);
		uart_puts(uartString);
	}

	
	
	//displaying the distance of the front sensor on bar graph
	//and distance dependent audio signal
	
	if (distanceFront <= 10)
	{
		DIODE_FRONT(8);
	}
	else if((distanceFront > 10) & (distanceFront <= 15))
	{
		DIODE_FRONT(7);
	}
	else if((distanceFront > 15) & (distanceFront <= 20))
	{
		DIODE_FRONT(6);
	}
	else if((distanceFront > 20) & (distanceFront <= 25))
	{
		DIODE_FRONT(5);
	}
	else if((distanceFront > 25) & (distanceFront <= 30))
	{
		DIODE_FRONT(4);
	}
	else if((distanceFront > 30) & (distanceFront <= 35))
	{
		DIODE_FRONT(3);
	}
	else if((distanceFront > 35) & (distanceFront <= 40))
	{
		DIODE_FRONT(2);
	}
	else if((distanceFront > 40) & (distanceFront <= 50))	
	{
		DIODE_FRONT(1);
	}
	else
	{
		DIODE_FRONT(0);
	}
	
	
	//displaying the distance of the rear sensor on bar graph
	
	if (distanceRear <= 10)
	{
		DIODE_REAR(8);
	}
	else if ((distanceRear > 10) & (distanceRear <= 15))
	{
		DIODE_REAR(7);
	}
	else if((distanceRear > 15) & (distanceRear <= 20))
	{
		DIODE_REAR(6);
	}
	else if((distanceRear > 20) & (distanceRear <= 25))
	{
		DIODE_REAR(5);
	} 
	else if((distanceRear > 25) & (distanceRear <= 30))
	{
		DIODE_REAR(4);
	} 
	else if((distanceRear > 30) & (distanceRear <= 35))
	{
		DIODE_REAR(3);
	} 
	else if((distanceRear > 35) & (distanceRear <= 40))
	{
		DIODE_REAR(2);
	} 
	else if((distanceRear > 40) & (distanceRear <= 50))
	{
		DIODE_REAR(1);
	} 
	else 
	{
		DIODE_REAR(0);
	}
	
	//distance dependent audio signal
	
	if ((dist > 10) & (dist <= 15))
	{
		beepTiming = 62000;
	}
	else if ((dist > 15) & (dist <= 20))
	{
		beepTiming = 58000;
	}
	else if ((dist > 20) & (dist <= 25))
	{
		beepTiming = 52000;
	}
	else if ((dist > 25) & (dist <= 30))
	{
		beepTiming = 46000;
	}
	else if ((dist > 35) & (dist <= 40))
	{
		beepTiming = 40000;
	}
	else if ((dist > 40) & (dist <= 50))
	{
		beepTiming = 34000;
	}

	return beepTiming;
	
}
