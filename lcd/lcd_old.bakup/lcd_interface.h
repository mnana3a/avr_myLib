#ifndef __LCD_INTERFACE_H__
#define __LCD_INTERFACE_H__

#define PORTx A

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


#endif
