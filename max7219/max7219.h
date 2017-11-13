// NOTE: max7219 ss is active high, max7221 ss is active low
// USER: this works only with common-cathode segments

#ifndef __max7219_H
#define __max7219_H

// tag this if you will use the on-board spi module otherwise will use the bit-bang
//#define __USE_SPI

#ifndef F_CPU
    #define F_CPU 16000000UL
#endif

#include <inttypes.h>
#include <avr/io.h>

extern uint8_t max7219;

#ifdef __USE_SPI
    #define SPI_MODE_0
    #define SPI_SS_ACTIVE_HIGH
    #include "spi/spi.h"
#else
    #define MAX7219_PORT    PORTD
    #define MAX7219_DDR     DDRD
    #define MAX7219_CS      PD0
    #define MAX7219_CLK     PD1
    #define MAX7219_DIN     PD2
#endif

#define NOOP   0x00
#define DIGIT0 0x01
#define DIGIT1 0x02
#define DIGIT2 0x03
#define DIGIT3 0x04
#define DIGIT4 0x05
#define DIGIT5 0x06
#define DIGIT6 0x07
#define DIGIT7 0x08
#define DECODEMODE  0x09
#define INTENSITY   0x0A
#define SCANLIMIT   0x0B
#define SHUTDOWN    0x0C
#define DISPLAYTEST 0x0F

void max7219_init(uint8_t digitsNum);
void max7219_put(uint8_t channel, uint8_t _data);
static void max7219_setReg(uint8_t mode, uint8_t value);
void max7219_testOff(void);
void max7219_testOn(void);
#ifndef __USE_SPI
static void max7219_byte(uint8_t data);
#endif

#endif

