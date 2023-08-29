/*
 * effects.c
 *
 * Created: 10/07/2023 07:10:35
 *  Author: milos
 */ 

#include "effects.h"

void demo_lines(display_t* display) {
	uint8_t N = 15;
	uint8_t M = 15;
	
	Display_Clear(display);
	
	for (uint8_t i = 0; i < N; i++) {
		Display_DrawLine(display, 0, 0, DISPLAY_HEIGHT - 1, (uint16_t)(DISPLAY_WIDTH - 1) * i/ N);
		Display_Flush(display);
		_delay_ms(50);
	}
	
	for (uint8_t i = M; i > 0; i--) {
		Display_DrawLine(display, 0, 0, (uint16_t)(DISPLAY_HEIGHT - 1) * i / M, DISPLAY_WIDTH - 1);
		Display_Flush(display);
		_delay_ms(50);
	}
	
	for (uint8_t i = N; i > 0; i--) {
		Display_DrawLine(display, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1, (uint16_t)(DISPLAY_WIDTH - 1) * i/ N);
		Display_Flush(display);
		_delay_ms(50);
	}
	
	for (uint8_t i = 0; i < M; i++) {
		Display_DrawLine(display, 0, DISPLAY_WIDTH - 1, (uint16_t)(DISPLAY_HEIGHT - 1) * i / M, 0);
		Display_Flush(display);
		_delay_ms(50);
	}
	
	_delay_ms(2000);
}

void demo_bounce(display_t* display, uint8_t bounces, uint8_t isPersistent) {
	int8_t x_dir = 1;
	int8_t y_dir = 1;
	uint8_t x0, x1, y0, y1;
	
	if (isPersistent) {
		x0 = 8;
		x1 = 11;
		y0 = 50;
		y1 = 53;
		} else {
		x0 = 10;
		x1 = x0 + 16;
		y0 = 50;
		y1 = y0 + 29;
	}
	
	Display_Clear(display);
	
	while (bounces) {
		// Detect bouncing object hitting the edge and inverse movement direction
		if ((x_dir == 1 && x1 == DISPLAY_HEIGHT - 1) || (x_dir == -1 && x0 == 0)) {
			x_dir *= -1;
			bounces--;
		}
		
		if ((y_dir == 1 && y1 == DISPLAY_WIDTH - 1) || (y_dir == -1 && y0 == 0)) {
			y_dir *= -1;
			bounces--;
		}
		
		// Move object
		x0 += x_dir;
		x1 += x_dir;
		y0 += y_dir;
		y1 += y_dir;
		
		// Draw
		if (!isPersistent) { // Bouncing rect
			Display_Clear(display);
			Display_DrawString(display, (char[]) {bounces + 48, 0}, x0 + 4, y0 + 12);
		}
		
		Display_DrawRect(display, x0, y0, x1, y1);
		
		Display_Flush(display);
		_delay_ms(5);
	}
}

void demo_text(display_t* display) {
	Display_Clear(display);
	
	Display_DrawString(display, "001001111001010001", 0,  0);
	Display_DrawString(display, "011000100010101010", 16, 0);
	Display_DrawString(display, "110100010101001001", 32, 0);
	Display_DrawString(display, "001011110010100011", 48, 0);	
	
	uint8_t n = 127;
	while(n--) {
		Display_PageShift(display, 0,  1);
		Display_PageShift(display, 0,  1);
		
		Display_PageShift(display, 2, -1);
		
		Display_PageShift(display, 4,  1);
		
		Display_PageShift(display, 6, -1);
		Display_PageShift(display, 6, -1);
		
		Display_Flush(display);
		_delay_ms(10);
	}
}

void demo_circle(display_t* display, uint8_t n) {
	uint8_t x_center = 31;
	uint8_t y_center = 63;
	uint8_t radius = 28;
	uint8_t x0, x1, y0, y1;
	
	Display_Clear(display);
	
	while(n--) {
		double pha = 3.14 * ((double)rand() / RAND_MAX);
		uint8_t p = (uint8_t)(radius * sin(pha));
		uint8_t q = (uint8_t)(radius * cos(pha));
		
		if (rand() > RAND_MAX / 2) {
			x0 =  p + x_center;
			x1 = -p + x_center;
			y0 =  q + y_center;
			y1 = -q + y_center;
			} else {
			x0 = -p + x_center;
			x1 =  p + x_center;
			y0 = -q + y_center;
			y1 =  q + y_center;
		}
		
		Display_DrawLine(display, x0, y0, x1, y1);
		
		Display_Flush(display);
		_delay_ms(20);
	}
}

void demo_randomLines(display_t* display, uint8_t n) {
	uint8_t x_center = 31;
	uint8_t y_center = 63;
	uint8_t radius = 28;
	uint8_t x0, x1, y0, y1;
	
	Display_Clear(display);
	
	while(n--) {
		double pha = 3.14 * ((double)rand() / RAND_MAX);
		uint8_t p = (uint8_t)(radius * sin(pha));
		uint8_t q = (uint8_t)(radius * cos(pha));
		
		if (rand() > RAND_MAX / 2) {
			x0 =  p + x_center;
			x1 = -p + x_center;
			y0 =  q + y_center;
			y1 = -q + y_center;
			} else {
			x0 = -p + x_center;
			x1 =  p + x_center;
			y0 = -q + y_center;
			y1 =  q + y_center;
		}
		
		for (uint8_t i = 0; i < 8; i++) {
			Display_PageShift(display, i, 1);
		}
		
		Display_DrawLine(display, x0, y0, x1, y1);
		
		Display_Flush(display);
		_delay_ms(20);
	}
}