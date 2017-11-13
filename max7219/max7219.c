// NOTE: first byte (MSB) contains the command control bits, the 2nd (LSB) is the data to be displayed

#include "max7219.h"

void max7219_init(uint8_t digitsNum)
{
    // enableDecoderOnDigits  holds the bits for digits to be enabled to work with the internal decoder ( binary form )
    //      if set to 0 no digit has the decoder
    //      if set to 255 all digits works with the decoder

    // digitsNum  is a "dicemal number" that can vary between 1 : 8 depending on the number of digits connected to the chip

    #ifdef __USE_SPI
        spi_init();
    #else
        MAX7219_DDR |= (1 << MAX7219_CLK);  // Set CLK port as output
        MAX7219_DDR |= (1 << MAX7219_CS);   // Set CS port as output
        MAX7219_DDR |= (1 << MAX7219_DIN);  // Set DIN port as output
        MAX7219_PORT &= ~(1 << MAX7219_CLK);   // Set CLK to LOW
        MAX7219_PORT &= ~(1 << MAX7219_CS);   // Set CS to LOW
    #endif

    max7219_setReg(DECODEMODE, 0xFF);
    max7219_setReg(SCANLIMIT, digitsNum-1);
    max7219_setReg(INTENSITY, 8);
    max7219_setReg(SHUTDOWN, 1);
    max7219_testOff();
}

void max7219_put(uint8_t channel, uint8_t _data)
{
    // if decoder is enabled
    //      channel can be 0x01:0x08
    //      data can be a binary number

    // if decoder is disabled
    //      channel can be 0x01:0x08
    //      data can be what segments to turn on/off b7=dp , b6 = a ..... b0 = g

    if (channel >= 1 && channel <= 8)
    {
        max7219_setReg(channel, _data);
    }
}

static void max7219_setReg(uint8_t mode, uint8_t value)
{
    #ifdef __USE_SPI
        spi_setSS();
        spi_put(mode);
        spi_put(value);
        spi_clearSS();
        // next step is a MUST so the last digit data latches
        spi_setSS();
        spi_clearSS();
    #else
        MAX7219_PORT |= (1 << MAX7219_CS);        // Set CS to HIGH
        max7219_byte(mode);
        max7219_byte(value);
        MAX7219_PORT &= ~(1 << MAX7219_CS);       // Set CS to LOW
        MAX7219_PORT &= ~(1 << MAX7219_CLK);      // Set CLK to LOW
        // next step is a MUST so the last digit data latches
        MAX7219_PORT |= (1 << MAX7219_CS);        // Set CS to HIGH
        MAX7219_PORT &= ~(1 << MAX7219_CS);       // Set CS to LOW
    #endif
}

void max7219_testOff(void)
{
    max7219_setReg(DISPLAYTEST, 0x00);
}

void max7219_testOn(void)
{
    max7219_setReg(DISPLAYTEST, 0x01);
}

#ifndef __USE_SPI
static void max7219_byte(uint8_t data)
{
    for(uint8_t i = 8; i >= 1; i--) {
        MAX7219_PORT &= ~(1 << MAX7219_CLK);   // Set CLK to LOW
        if (data & 0x80)                        // Mask the MSB of the data
            MAX7219_PORT |= (1 << MAX7219_DIN);    // Set DIN to HIGH
        else
            MAX7219_PORT &= ~(1 << MAX7219_DIN);   // Set DIN to LOW
        MAX7219_PORT |= (1 << MAX7219_CLK);        // Set CLK to HIGH
        data <<= 1;
    }
}
#endif
