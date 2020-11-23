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

#define sensTrigFront PB2
#define sensEchoFront PD3

#define sensTrigRear PB3
#define sensEchoRear PD2

#define buzzer PB4

volatile uint8_t trigEnable = 1;
volatile uint8_t averaging = 0;
volatile float distanceFront = 0;
volatile float distanceRear = 0;
uint8_t mux = 1;

void displayValues(float distance, uint8_t averaging, uint8_t mux);

int main(void)
{
	
	GPIO_config_input_nopull(&DDRD, sensEchoFront);
	GPIO_config_input_nopull(&DDRD, sensEchoRear);
	
	GPIO_config_output(&DDRB, sensTrigFront);
	GPIO_config_output(&DDRB, sensTrigRear);
	
	GPIO_write_low(&PORTB, sensTrigFront);
	GPIO_write_low(&PORTB, sensTrigRear);
	
	GPIO_config_output(&DDRB, buzzer);
	
	EICRA |= ((1<<ISC11) | (1<<ISC10));
	EIMSK |= (1<<INT1);
	
	EICRA |= ((1<<ISC01) | (1<<ISC00));
	EIMSK |= (1<<INT0);
	
	sei();
	
	uart_init(UART_BAUD_SELECT(9600, F_CPU));
	
    /* Replace with your application code */
    while (1) 
    {
		if ((mux == 1) & (trigEnable == 1))
		{
			if (averaging == 10)
			{
				displayValues(distanceFront, averaging, mux);
				distanceFront = 0;
				averaging = 0;
				mux++;
			}
			//_delay_ms(5);
			GPIO_write_high(&PORTB, sensTrigFront);
			_delay_us(10);
			GPIO_write_low(&PORTB, sensTrigFront);
			trigEnable = 0;
		}
		if ((mux == 2) & (trigEnable == 1))
		{
			if (averaging == 10)
			{
				displayValues(distanceRear, averaging, mux);
				distanceRear = 0;
				averaging = 0;
				mux = 1;
			}
			//_delay_ms(5);
			GPIO_write_high(&PORTB, sensTrigRear);
			_delay_us(10);
			GPIO_write_low(&PORTB, sensTrigRear);
			trigEnable = 0;			

		}
		
    }
}

ISR(INT1_vect){
	
	do{
		distanceFront++;
		_delay_us(1);
	}while(GPIO_read(&PIND, sensEchoFront));
	
	averaging++;
	trigEnable = 1;

}

ISR(INT0_vect){
	
	do{
		distanceRear++;
		_delay_us(1);
	}while(GPIO_read(&PIND, sensEchoRear));
	
	averaging++;
	trigEnable = 1;

}

void displayValues(float distance, uint8_t averaging, uint8_t mux){
	
	char uartString[50];
	
	distance = distance*(0.1901);
	distance = distance/averaging;
	if (distance < 10)
	{
		GPIO_write_high(&PORTB, buzzer);
	} 
	else
	{
		GPIO_write_low(&PORTB, buzzer);
	}
	if (mux == 1) sprintf(uartString,"Front: = %0.1f\r\n",distance);
	if (mux == 2) sprintf(uartString,"      Rear: =  %0.1f\r\n",distance);
	uart_puts(uartString);
	
}
