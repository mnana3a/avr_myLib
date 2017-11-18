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
