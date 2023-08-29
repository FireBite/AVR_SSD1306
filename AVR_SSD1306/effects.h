/*
 * effects.h
 *
 * Created: 10/07/2023 07:10:22
 *  Author: milos
 */ 


#ifndef EFFECTS_H_
#define EFFECTS_H_

#include "stdlib.h"
#include "display.h"
#include "math.h"
#include <util/delay.h>

void demo_lines(display_t* display);
void demo_bounce(display_t* display, uint8_t bounces, uint8_t isPersistent);
void demo_text(display_t* display);
void demo_circle(display_t* display, uint8_t n);
void demo_randomLines(display_t* display, uint8_t n);

#endif /* EFFECTS_H_ */