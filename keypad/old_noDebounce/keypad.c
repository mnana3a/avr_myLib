#include "keypad.h"
#include "lcd/lcd.h"
#include "rt/rt.h"

static uint8_t keypad[_MAX_KEYS] = {0b11101110, 0b11011110, 0b10111110, 0b01111110,
                                                                        0b11101101, 0b11011101, 0b10111101, 0b01111101,
                                                                        0b11101011, 0b11011011, 0b10111011, 0b01111011,
                                                                        0b11100111, 0b11010111, 0b10110111, 0b01110111,};


// this routine reports the button in 2 port reads
// set coloumns to output and set them to 0 while rows inputs.. when read:
//      if no button is being pressed all row inputs are 1 due to pull ups
//      if a butt is being pressed its row is detected
// set rows as outputs and set them to 0 while coloumns inputs.. when read:
//  the same
int8_t keypad_getKey(void)
{
    uint8_t _key = 0;
    uint8_t counter = 0;

    // check rows
    // missing debounce
    KEYPAD_DDR = 0xf0;
    KEYPAD_PORT = 0x0f;
    _delay_us(3);           /* Insert nop for synchronization*/
    if((_key = (KEYPAD_PIN & 0x0f)) == 0x0f)    return -1;

    // check coloumns
    // missing debounce
    KEYPAD_DDR = 0x0f;
    KEYPAD_PORT = 0xf0;
    _delay_us(3);           /* Insert nop for synchronization*/
    _key |= (KEYPAD_PIN & 0Xf0);

    for(counter = 0 ; counter < 16 ; counter++)
    {
        if(keypad[counter] == _key) break;
    }
    if(counter == 16)   return -1;
    return (int8_t)keypadMap[counter];
}

// this function stops the cpu from doing anything else while waiting for a key press
uint8_t keypad_waitForKey(void)
{
    uint8_t _key = 0, counter = 0;

    KEYPAD_DDR = 0xf0;
    KEYPAD_PORT = 0x0f;
    do{
        _delay_us(3);
        _key = KEYPAD_PIN & 0x0f;
    }while( _key == 0x0f );

    KEYPAD_DDR = 0x0f;
    KEYPAD_PORT = 0xf0;
    _delay_us(3);
    // check coloumns
    // missing debounce
    _key = _key | (KEYPAD_PIN & 0Xf0);

    for(counter = 0 ; counter < 16 ; counter++)
    {
        if(keypad[counter] == _key) break;
    }
    if(counter == 16)   return -1;
    return keypadMap[counter];
}
