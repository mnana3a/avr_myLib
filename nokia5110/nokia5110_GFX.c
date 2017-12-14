#include "nokia5110_GFX.h"

// setLine draws a line from x0,y0 to x1,y1 with the set color.
// This function was grabbed from the SparkFun ColorLCDShield
// library.
void nokia5110_set_line(uint8_t y0, uint8_t x0, uint8_t y1, uint8_t x1, uint8_t bw)
{
    int8_t dy, dx, stepx, stepy;

    dy = y1 - y0;
    dx = x1 - x0;

    if (dy < 0)
    {
        dy = -dy;
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }

    if (dx < 0)
    {
        dx = -dx;
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }

    dy <<= 1; // dy is now 2*dy
    dx <<= 1; // dx is now 2*dx
    nokia5110_pixel(y0, x0, bw); // Draw the first pixel.

    if (dx > dy)
    {
        int8_t fraction = dy - (dx >> 1);
        while (x0 != x1)
        {
          if (fraction >= 0)
          {
            y0 += stepy;
            fraction -= dx;
          }
          x0 += stepx;
          fraction += dy;
          nokia5110_pixel(y0, x0, bw);
        }
    }
    else
    {
        int fraction = dx - (dy >> 1);
        while (y0 != y1)
        {
          if (fraction >= 0)
          {
            x0 += stepx;
            fraction -= dy;
          }
          y0 += stepy;
          fraction += dx;
          nokia5110_pixel(y0, x0, bw);
        }
    }
}

// this will draw a rectangle from x0,y0 top-left corner to
// a x1,y1 bottom-right corner. Can be filled with the fill
// parameter, and colored with bw.
// This function was grabbed from the SparkFun ColorLCDShield
// library.
void nokia5110_set_rect(uint8_t y0, uint8_t x0, uint8_t y1, uint8_t x1, uint8_t fill, uint8_t bw)
{
  // check if the rectangle is to be filled
  if (fill == 1)
  {
    int xDiff;

    if(x0 > x1)
      xDiff = x0 - x1; //Find the difference between the x vars
    else
      xDiff = x1 - x0;

    while(xDiff > 0)
    {
      nokia5110_set_line(y0, x0, y1, x0, bw);

      if(x0 > x1)
        x0--;
      else
        x0++;

      xDiff--;
    }
  }
  else
  {
    // best way to draw an unfilled rectangle is to draw four lines
    nokia5110_set_line(y0, x0, y0, x1, bw);
    nokia5110_set_line(y1, x0, y1, x1, bw);
    nokia5110_set_line(y0, x0, y1, x0, bw);
    nokia5110_set_line(y0, x1, y1, x1, bw);
  }
}

// setCircle draws a circle centered around x0,y0 with a defined
// radius. The circle can be black or white. And have a line
// thickness ranging from 1 to the radius of the circle.
// This function was grabbed from the SparkFun ColorLCDShield
// library.
void nokia5110_set_circle (uint8_t y0, uint8_t x0, uint8_t radius, uint8_t bw, uint8_t lineThickness)
{
    for(uint8_t r = 0; r < lineThickness; r++)
    {
        int8_t fl = 1 - radius;
        int8_t ddF_x = 0;
        int8_t ddF_y = -2 * radius;
        int8_t x = 0;
        int8_t y = radius;

        nokia5110_pixel(y0 + radius, x0, bw);
        nokia5110_pixel(y0 - radius, x0, bw);
        nokia5110_pixel(y0, x0 + radius, bw);
        nokia5110_pixel(y0, x0 - radius, bw);

        while(x < y)
        {
            if (fl >= 0)
            {
                y--;
                ddF_y += 2;
                fl += ddF_y;
            }
            x++;
            ddF_x += 2;
            fl += ddF_x;

            nokia5110_pixel(y0 + y, x0 + x, bw);
            nokia5110_pixel(y0 + y, x0 - x, bw);
            nokia5110_pixel(y0 - y, x0 + x, bw);
            nokia5110_pixel(y0 - y, x0 - x, bw);
            nokia5110_pixel(y0 + x, x0 + y, bw);
            nokia5110_pixel(y0 + x, x0 - y, bw);
            nokia5110_pixel(y0 - x, x0 + y, bw);
            nokia5110_pixel(y0 - x, x0 - y, bw);
        }
        radius--;
    }
}

void nokia5110_set_filledCircle(uint8_t y0, uint8_t x0, uint8_t r, uint8_t color)
{
    int8_t f = 1 - r;
    int8_t ddF_x = 1;
    int8_t ddF_y = -2 * r;
    int8_t x = 0;
    int8_t y = r;

    for (uint8_t i = y0 - r; i <= y0 + r; i++) {
        nokia5110_pixel(i, x0, color);
    }

    while (x < y) {
        if (f >= 0) {
          y--;
          ddF_y += 2;
          f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        for (uint8_t i = y0 - y; i <= y0 + y; i++) {
            nokia5110_pixel(i, x0+x, color);
            nokia5110_pixel(i, x0-x, color);
        } 
        for (uint8_t i = y0 - x; i <= y0 + x; i++) {
            nokia5110_pixel(i, x0+y, color);
            nokia5110_pixel(i, x0-y, color);
        }    
    }
}


void nokia5110_set_circleHelper(int16_t y0, int16_t x0, int16_t r, uint8_t cornername, uint16_t color)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      nokia5110_pixel(y0 + y, x0 + x, color);
      nokia5110_pixel(y0 + x, x0 + y, color);
    } 
    if (cornername & 0x2) {
      nokia5110_pixel(y0 - y, x0 + x, color);
      nokia5110_pixel(y0 - x, x0 + y, color);
    }
    if (cornername & 0x8) {
      nokia5110_pixel(y0 + x, x0 - y, color);
      nokia5110_pixel(y0 + y, x0 - x, color);
    }
    if (cornername & 0x1) {
      nokia5110_pixel(y0 - x, x0 - y, color);
      nokia5110_pixel(y0 - y, x0 - x, color);
    }
  }
}


// TODO: mod these functions to suite if needed "grabbed from adafruit gfx lib

/*
// draw a rounded rectangle!
void Adafruit_GFX::drawRoundRect(int16_t x, int16_t y, int16_t w,
  int16_t h, int16_t r, uint16_t color) {
  // smarter version
  drawFastHLine(x+r  , y    , w-2*r, color); // Top
  drawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
  drawFastVLine(  x    , y+r  , h-2*r, color); // Left
  drawFastVLine(  x+w-1, y+r  , h-2*r, color); // Right
  // draw four corners
  drawCircleHelper(x+r    , y+r    , r, 1, color);
  drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

// fill a rounded rectangle!
void Adafruit_GFX::fillRoundRect(int16_t x, int16_t y, int16_t w,
                 int16_t h, int16_t r, uint16_t color) {
  // smarter version
  fillRect(x+r, y, w-2*r, h, color);

  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

// draw a triangle!
void Adafruit_GFX::drawTriangle(int16_t x0, int16_t y0,
                int16_t x1, int16_t y1, 
                int16_t x2, int16_t y2, uint16_t color) {
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

// fill a triangle!
void Adafruit_GFX::fillTriangle ( int16_t x0, int16_t y0,
                  int16_t x1, int16_t y1, 
                  int16_t x2, int16_t y2, uint16_t color) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    drawFastHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1,
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }
}*/
