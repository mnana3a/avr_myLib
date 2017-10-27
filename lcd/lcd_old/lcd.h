// 4 bit mode on one port ---> PORTB

// this header and functions are for 2x16 lcd and other versions require some modifications

/* development:
        - define more command to be used
        - more control over the shifting and cursor moving
        - if the first line is full move automatically to the 2nd line
        - add support to read strings from flash mem. using prgspace.h functions
        - add support for float numbers										(done)
*/
/* HW connections:
   	  high nibble of lcd to high nibble of the chosen port
   	  rs - rw - e to the first three pins of the same chosen port
*/
#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

#define _CONCAT(a,b) a##b
#define PORT(x) _CONCAT(PORT,x)
#define PIN(x) _CONCAT(PIN,x)
#define DDR(x) _CONCAT(DDR,x)

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LCD_DDR DDR(A)    // high nibble of data pins are connected to the high nibble of the port
#define LCD_PORT PORT(A)
#define LCD_PIN PIN(A)
#define LCD_RS 0        // RS pin on b0
#define LCD_RW 1        // RW pin on b1  or can be hardwired to gnd
#define LCD_EN 2        // enable pin on b2

#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_LINE1 0X80
#define LCD_LINE2 0XC0
#define LCD_CURSOR_OFF 0X0C
#define LCD_RIGHT 0x18
#define LCD_LEFT 0x1C


void lcd_init(void);
void lcd_command(uint8_t command);
void lcd_data(uint8_t data);
void lcd_goxy(uint8_t x, uint8_t y);
void lcd_disp(unsigned char *string);
void lcd_busy(void);
void lcd_int(int32_t value, int8_t field_length);
void lcd_float(float value, int8_t field_length);

#endif // LCD_H_INCLUDED

