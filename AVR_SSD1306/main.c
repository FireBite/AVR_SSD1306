/*
 * AVR_SSD1306.c
 *
 * Created: 08/07/2023 22:49:26
 * Author : milos
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "display.h"
#include "i2c.h"
#include "effects.h"

int main(void) {
	display_t display;
	
	I2C_Init();
	Display_Init(&display);
	
	Display_Clear(&display);
	Display_Flush(&display);
	
	_delay_ms(2000);
    
	while (1) {
		demo_bounce(&display, 9, 0);
		demo_text(&display);
		demo_randomLines(&display, 175);
		demo_circle(&display, 75);
		demo_lines(&display);
		demo_bounce(&display, 9, 1);
	}
}

