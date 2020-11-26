

/*

 * lcd_i2cmaster_test.c

 *

 * Created: 03/06/2014 11:07:15

 *  Author: David Prentice

 */ 


#include <avr/io.h>
#define F_CPU	16000000
#include <util/delay.h>

#include "lcd.h"

extern void lcd_backlight(char on);    //not in lcd.h

int main(void)

{

    lcd_init(LCD_ON_DISPLAY);

	lcd_backlight(0);

	_delay_ms(500);

	lcd_backlight(1);

	_delay_ms(500);

	while(1)

    {

        lcd_clrscr();

		

		lcd_gotoxy(1, 1);

		lcd_puts("Second Line");

		_delay_ms(1000);
		lcd_gotoxy(1, 0);

		lcd_puts("Hello World");

    }

}