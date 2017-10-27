#ifndef __LCD_INTERFACE_H__
#define __LCD_INTERFACE_H__


#define PORTx A
#define __USE_CUSTOM_CHAR

// max number of custom characters is 8 [0:7]
// to modify a character just comment one of the last two in this matrix and define your own using the form:
//      const unsigned char Character1[8] PROGMEM = { 0x00, 0x0A, 0x15, 0x11, 0x0A, 0x04, 0x00, 0x00 };
//      lcd_defineChar(Character1, 7);


// to display a custom character after defining it first thing in main... lcd_data(0) or its location in CGRAM
#ifdef __USE_CUSTOM_CHAR
    const uint8_t lcdCustomChar[] PROGMEM=//define 8 custom LCD chars
    {
        0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, // 0. 0/5 full progress block
        0x00, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x00, // 1. 1/5 full progress block
        0x00, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, // 2. 2/5 full progress block
        0x00, 0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x1F, 0x00, // 3. 3/5 full progress block
        0x00, 0x1F, 0x1E, 0x1E, 0x1E, 0x1E, 0x1F, 0x00, // 4. 4/5 full progress block
        0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, // 5. 5/5 full progress block
        0x03, 0x07, 0x0F, 0x1F, 0x0F, 0x07, 0x03, 0x00, // 6. rewind arrow
        //0x18, 0x1C, 0x1E, 0x1F, 0x1E, 0x1C, 0x18, 0x00  // 7. fast-forward arrow
    };
#endif

#define _CONCAT(a,b) a##b
#define PORT(x) _CONCAT(PORT,x)
#define PIN(x) _CONCAT(PIN,x)
#define DDR(x) _CONCAT(DDR,x)

#define LCD_DDR DDR(PORTx)    // high nibble of data pins are connected to the high nibble of the port
#define LCD_PORT PORT(PORTx)
#define LCD_PIN PIN(PORTx)
#define LCD_RS 0        // RS pin on b0
#define LCD_RW 1        // RW pin on b1  or can be hardwired to gnd
#define LCD_EN 2        // enable pin on b2

// tag this if you want the string to show sequentially
//#define __LCD_DISP_SEQ

#endif
