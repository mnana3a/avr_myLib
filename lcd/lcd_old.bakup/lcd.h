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

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/pgmspace.h>


#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_LINE1 0X80
#define LCD_LINE2 0XC0
#define LCD_CURSOR_OFF 0X0C
#define LCD_RIGHT 0x18
#define LCD_LEFT 0x1C
#define LCD_CURSOR_BLINK_ON 0x0F
#define LCD_CURSOR_BLINK_OFF 0X0E
#define LCD_4BIT 0x28
#define LCD_SHIFT_CURSOR_RIGHT 0X06
#define LCD_DISP_OFF 0X08

#define LCD_CLR             0	//DB0: clear display
#define LCD_HOME            1	//DB1: return to home position
#define LCD_ENTRY_MODE      2	//DB2: set entry mode
#define LCD_ENTRY_INC       1	//DB1: increment
#define LCD_ENTRY_SHIFT     0	//DB2: shift
#define LCD_ON_CTRL         3	//DB3: turn lcd/cursor on
#define LCD_ON_DISPLAY      2	//DB2: turn display on
#define LCD_ON_CURSOR       1	//DB1: turn cursor on
#define LCD_ON_BLINK        0	//DB0: blinking cursor
#define LCD_MOVE            4	//DB4: move cursor/display
#define LCD_MOVE_DISP       3	//DB3: move display (0-> move cursor)
#define LCD_MOVE_RIGHT      2	//DB2: move right (0-> left)
#define LCD_FUNCTION        5	//DB5: function set
#define LCD_FUNCTION_8BIT   4	//DB4: set 8BIT mode (0->4BIT mode)
#define LCD_FUNCTION_2LINES 3	//DB3: two lines (0->one line)
#define LCD_FUNCTION_10DOTS 2	//DB2: 5x10 font (0->5x7 font)
#define LCD_CGRAM           6	//DB6: set CG RAM address
#define LCD_DDRAM           7	//DB7: set DD RAM address
// reading:
#define LCD_BUSY            7	//DB7: LCD is busy
#define LCD_LINES			2	//visible lines
#define LCD_LINE_LENGTH		16	//line length (in characters)
// cursor position to DDRAM mapping
#define LCD_LINE0_DDRAMADDR		0x00
#define LCD_LINE1_DDRAMADDR		0x40
#define LCD_LINE2_DDRAMADDR		0x14
#define LCD_LINE3_DDRAMADDR		0x54
// progress bar defines
#define PROGRESSPIXELS_PER_CHAR	6


void lcd_init(void);
void lcd_command(uint8_t command);
void lcd_data(uint8_t data);
void lcd_goxy(uint8_t x, uint8_t y);
void lcd_disp(unsigned char *string);
void lcd_busy(void);
void lcd_int(int32_t value, int8_t field_length);
void lcd_float(float value, int8_t field_length);

#endif // LCD_H_INCLUDED

