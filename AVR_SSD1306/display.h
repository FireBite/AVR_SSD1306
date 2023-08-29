/*
 * display.h
 *
 * Created: 08/07/2023 22:50:32
 *  Author: milos
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

#define DISPLAY_WIDTH  128
#define DISPLAY_HEIGHT 64

#define DISPLAY_CMD  (0 << 6)
#define DISPLAY_DATA (1 << 6)

typedef struct {
	uint8_t framebuf[DISPLAY_HEIGHT / 8][DISPLAY_WIDTH];
} display_t;

void Display_Init(display_t* display);

void Display_DrawPixel(display_t* display, uint8_t x, uint8_t y);
void Display_ClearPixel(display_t* display, uint8_t x, uint8_t y);

void Display_DrawLine(display_t* display, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void Display_DrawRect(display_t* display, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void Display_DrawFillRect(display_t* display, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

void Display_DrawString(display_t* display, char* text, uint8_t x, uint8_t y);

void Display_PageShift(display_t* display, uint8_t page, int8_t dir);

void Display_SetAll(display_t* display);
void Display_Clear(display_t* display);
void Display_Flush(display_t* display);

#endif /* DISPLAY_H_ */