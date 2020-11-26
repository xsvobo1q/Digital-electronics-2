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
volatile double distanceFront = 0;
volatile double distanceRear = 0;
uint8_t mux = 1;

void displayValues(float distance, float distanceFront, uint8_t averaging, uint8_t mux);

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
 			if (averaging == 4) 
 			{
// 				displayValues(distanceFront, averaging, mux);
// 				distanceFront = 0;
 				averaging = 0;
 				mux++;
 			}
			_delay_us(50);
			GPIO_write_high(&PORTB, sensTrigFront);
			_delay_us(10);
			GPIO_write_low(&PORTB, sensTrigFront);
			trigEnable = 0;
		}
		if ((mux == 2) & (trigEnable == 1))
		{
 			if (averaging == 4)	mux++;
//			{
// 				displayValues(distanceRear, averaging, mux);
// 				distanceRear = 0;
// 				averaging = 0;
				
 //			}
			_delay_us(50);
			GPIO_write_high(&PORTB, sensTrigRear);
			_delay_us(10);
			GPIO_write_low(&PORTB, sensTrigRear);
			trigEnable = 0;			

		}
		if (mux == 3)
		{
			displayValues(distanceRear, distanceFront, averaging, mux);
			distanceFront = 0;
			distanceRear = 0;
			averaging = 0;
			mux = 1;
		}
		
    }
}

ISR(INT1_vect){
	
	do{
		distanceFront++;
		//_delay_us(1);
	}while(GPIO_read(&PIND, sensEchoFront));
	
	averaging++;
	trigEnable = 1;

}

ISR(INT0_vect){
	
	do{
		distanceRear++;
		//_delay_us(1);
	}while(GPIO_read(&PIND, sensEchoRear));
	
	averaging++;
	trigEnable = 1;

}

void displayValues(float distanceRear, float distanceFront, uint8_t averaging, uint8_t mux){
	
	_delay_ms(10);
	
	char uartString[50];
	
	distanceRear = distanceRear/averaging;
	distanceFront = distanceFront/averaging;
	distanceRear = distanceRear*(0.18692);
	distanceFront = distanceFront*(0.18692);
	
	if ((distanceFront < 10) | (distanceRear < 10))
	{
		GPIO_write_high(&PORTB, buzzer);
	} 
	else
	{
		GPIO_write_low(&PORTB, buzzer);
	}
	
	sprintf(uartString,"Front: %0.1f      ||      Rear: %0.1f \r\n",distanceFront, distanceRear);

	uart_puts(uartString);
}