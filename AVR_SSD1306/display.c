/*
 * display.c
 *
 * Created: 08/07/2023 22:50:44
 *  Author: milos
 */ 

#include <string.h>
#include "display.h"
#include "i2c.h"
#include "font.h"

void Display_Init(display_t* display) {
	//uint8_t data1[] = {
		//0xAE,       // display off
		//0xD5, 0x80, // clock div and osc freq
		//0xA8, 63    // multiplex ratio
	//};
	//I2C_WriteCommandMultiple(0x78, DISPLAY_COMMAND_CMD, data1, sizeof(data1));
		//
	//uint8_t data2[] = {
		//0xD3, 0,    // display offset
		//0x40        // start line - 0
	//};
	//I2C_WriteCommandMultiple(0x78, DISPLAY_COMMAND_CMD, data2, sizeof(data2));
		//
	//uint8_t data3[] = {
		//0x20, 0,    // addressing mode: 0 - "horizonal" XD
		//0xA0 | 0x1, // segment remap
		//0xC8        // scan dir - normal
	//};
	//I2C_WriteCommandMultiple(0x78, DISPLAY_COMMAND_CMD, data3, sizeof(data3));
		//
	//uint8_t data4[] = {
		//0xDA, 0x12, // pin config
		//0x81, 0xCF, // contrast
		//0xD9, 0xF1  // pre-charge period
	//};
	//I2C_WriteCommandMultiple(0x78, DISPLAY_COMMAND_CMD, data4, sizeof(data4));
		//
	//uint8_t data5[] = {
		//0xDB, 0x40, // vcomh
		//0xA4,       // display from ram
		//0xA6,       // normal display, non inverted
		//0x2E,       // 
		//0xAF
	//};
	//I2C_WriteCommandMultiple(0x78, DISPLAY_COMMAND_CMD, data5, sizeof(data5));
	
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0xae},	      1);     // Display OFF/ON: off (POR = 0xae)
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0xa4},       1);     // Set Entire Display OFF/ON: off (POR = 0xa4)
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0xd5, 0x50}, 2);     // Divide Ratio/Oscillator FrequencyData Set: divide ratio = 1 (POR = 1), Oscillator Frequency = +/- 0% (POR = +/- 0%)
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0xa8, 0x3f}, 2);     // Multiplex Ratio Data Set: 64 (POR = 0x3f, 64)
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0xd3, 0x00}, 2);     // Display OffsetData Set: 0 (POR = 0x00)
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0x40},       1);     // Set Display Start Line: 0
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0xad, 0x8b}, 2);     // DC-DC ON/OFF Mode Set: Built-in DC-DC is used, Normal Display (POR = 0x8b)
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0xd9, 0x22}, 2);     // Dis-charge/Pre-charge PeriodData Set: pre-charge 2 DCLKs, dis-charge 2 DCLKs (POR = 0x22, pre-charge 2 DCLKs, dis-charge 2 DCLKs)
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0xdb, 0x35}, 2);     // VCOM Deselect LevelData Set: 0,770V (POR = 0x35, 0,770 V)
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0x32},       1);	  // Set Pump voltage value: 8,0 V (POR = 0x32, 8,0 V)
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0x81, 0xff}, 2);     // Contrast Data Register Set: 255 (large) (POR = 0x80)
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0xa6},       1);	  // Set Normal/Reverse Display: normal (POR = 0xa6)
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0xda, 0x12}, 2);	  // com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5)
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0xa6},       1);	  // none inverted normal display mode
	I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0xaf},       1);     // Display on
}

void Display_DrawPixel(display_t* display, uint8_t x, uint8_t y) {
	uint8_t page = x / 8;
	display->framebuf[page][y] |= 1 << (x - (page * 8));
}

void Display_ClearPixel(display_t* display, uint8_t x, uint8_t y) {
	uint8_t page = x / 8;
	display->framebuf[page][y] &= 1 << (x - (page * 8));
}

// Based on bresenham's line algorithm
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void Display_DrawLine(display_t* display, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	int16_t dx = abs(x1 - x0);
	int16_t sx = x0 < x1 ? 1 : -1;
	
	int16_t dy = abs(y1 - y0);
	int16_t sy = y0 < y1 ? 1 : -1;
	
	int16_t err = (dx>dy ? dx : -dy)/2;
	int16_t err_last;

	while(1) {
		Display_DrawPixel(display, x0, y0);
		
		if (x0==x1 && y0==y1)
			break;
		
		err_last = err;
		
		if (err_last > -dx) {
			err -= dy;
			x0 += sx;
		}
		
		if (err_last < dy) {
			err += dx;
			y0 += sy;
		}
	}
}

void Display_DrawRect(display_t* display, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	// Draw border
	Display_DrawLine(display, x0, y0, x0, y1);
	Display_DrawLine(display, x1, y1, x0, y1);
	Display_DrawLine(display, x1, y1, x1, y0);
	Display_DrawLine(display, x0, y0, x1, y0);
}

void Display_DrawFillRect(display_t* display, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	for (uint8_t x = 0; x < x1 - x0; x++) {
		for (uint8_t y = 0; y < y1 - y0; y++) {
			Display_DrawPixel(display, x0 + x, y0 + y);
		}
	}
}

void Display_DrawString(display_t* display, char* text, uint8_t x, uint8_t y) {
	uint8_t page = x / 8;
	uint8_t x_page = x - page * 8;
	uint8_t* glyph;
	
	// Check for vertical overflow
	if (x + 7 > DISPLAY_HEIGHT - 1)
		return;
	
	// Use faster algorithm if drawn on the page boundaries
	uint8_t isAligned = x_page == 0;
	
	while ((*text) != '\0') {
		// Draw glyph column by column
		for (uint8_t offset = 0; offset < 5; offset++) {
			// Check for horizontal overflow
			if (y > DISPLAY_WIDTH - 1)
				return;
			
			// Get glyph for current letter
			glyph = font[(*text) - 48];
				
			if (isAligned) {
				// Copy font column directly to the frame buffer
				display->framebuf[page][y] |= glyph[offset];
			} else {
				// Split font column into two pages
				display->framebuf[page]    [y] |= glyph[offset] << x_page;
				display->framebuf[page + 1][y] |= glyph[offset] >> (8 - x_page);
			}
			
			y++;
		}
		
		// Add space between glyphs
		y += 2;
		
		// Check for horizontal overflow
		if (y > DISPLAY_WIDTH - 1)
			return;
		
		text++;
	}
}

void Display_PageShift(display_t* display, uint8_t page, int8_t dir) {
	// Iterate through page and do cyclic shift to all columns in a single page
	if (dir == 1) {
		uint8_t temp = display->framebuf[page][127];
		
		for (uint8_t i = 127; i > 0; i--) {
			display->framebuf[page][i] = display->framebuf[page][i-1];
		}
	
		display->framebuf[page][0] = temp;
	} else {
		uint8_t temp = display->framebuf[page][0];
		
		for (uint8_t i = 0; i < 127; i++) {
			display->framebuf[page][i] = display->framebuf[page][i+1];
		}
	
		display->framebuf[page][127] = temp;
	}
}

void Display_SetAll(display_t* display) {
	memset(display->framebuf, 0xFF, DISPLAY_HEIGHT / 8 * DISPLAY_WIDTH);	
}

void Display_Clear(display_t* display) {
	memset(display->framebuf, 0x00, DISPLAY_HEIGHT / 8 * DISPLAY_WIDTH);
}

void Display_Flush(display_t* display) {
	for (uint8_t page = 0; page < 8; page++) {
		// Reset RAM?
		I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0x40}, 1);
		
		// Reset? column and set page address
		I2C_WriteCommandMultiple(0x78, DISPLAY_CMD, (uint8_t[]) {0x10, 0x02, 0xB0 + page}, 3);
			
		// Write out one page chunk from framebuffer
		I2C_WriteCommandMultiple(0x78, DISPLAY_DATA, display->framebuf[page], DISPLAY_WIDTH);
	}
}

