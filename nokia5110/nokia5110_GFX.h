#ifndef __NOKIA_5110_H
#define __NOKIA_5110_H

#include "inttypes.h"
#include "avr/io.h"
#include "nokia5110.h"

void nokia5110_set_line(uint8_t y0, uint8_t x0, uint8_t y1, uint8_t x1, uint8_t bw);
// top-left corner to bottom-right corner
void nokia5110_set_rect(uint8_t y0, uint8_t x0, uint8_t y1, uint8_t x1, uint8_t fill, uint8_t bw);
void nokia5110_set_circle (uint8_t y0, uint8_t x0, uint8_t radius, uint8_t bw, uint8_t lineThickness);
void nokia5110_set_filledCircle(uint8_t y0, uint8_t x0, uint8_t r, uint8_t color);
void nokia5110_set_circleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);

#endif
