#include "keypad.h"
#include "keypad_map.h"

int8_t g_u8Key = 0;
enum STATE _kstate = RELEASED;


static uint8_t _keypad[_MAX_KEYS] = {0b11101110, 0b11011110, 0b10111110, 0b01111110,
                                                                        0b11101101, 0b11011101, 0b10111101, 0b01111101,
                                                                        0b11101011, 0b11011011, 0b10111011, 0b01111011,
                                                                        0b11100111, 0b11010111, 0b10110111, 0b01110111,};


// this routine reports the button in 2 port reads
// set coloumns to output and set them to 0 while rows inputs.. when read:
//      if no button is being pressed all row inputs are 1 due to pull ups
//      if a butt is being pressed its row is detected
// set rows as outputs and set them to 0 while coloumns inputs.. when read:
//  the same

// un-debounced
int8_t keypad_key(void)
{
    uint8_t _key = 0;
    uint8_t counter = 0;

    // check rows
    KEYPAD_DDR = 0xf0;
    KEYPAD_PORT = 0x0f;
    _delay_us(5);           /* Insert nop for synchronization and time const of parasitic cap*/
    if((_key = (KEYPAD_PIN & 0x0f)) == 0x0f)    return -1;

    // check coloumns
    KEYPAD_DDR = 0x0f;
    KEYPAD_PORT = 0xf0;
    _delay_us(5);           /* Insert nop for synchronization and time const of parasitic cap*/
    _key |= (KEYPAD_PIN & 0Xf0);

    for(counter = 0 ; counter < 16 ; counter++)
    {
        if(_keypad[counter] == _key) break;
    }
    if(counter == 16)   return -2;
    return (int8_t)keypadMap[counter];
}

// de-bounced version
int8_t *keypad_getKey(void)
{
    static uint8_t _kpFlag = 0;
    int8_t __key = 0;

    __key = keypad_key();
    switch(_kstate){
        case RELEASED:
            if(__key != -1 && __key != -2)  _kstate = MAYBE_PUSHED;
            else if(__key == -2)
            {
                    // do nothing, keep the previous state
            }
            else    _kstate = RELEASED;
        break;

        case MAYBE_PUSHED:
            if(__key != -1 && __key != -2){
                _kstate = PUSHED;
                _kpFlag = FLAG_ON;
            }
            else if(__key == -2)
            {
                    // do nothing, keep the previous state
            }
            else    _kstate = RELEASED;
        break;

        case PUSHED:
            if(__key != -1 && __key != -2)
            {
                _kstate = PUSHED;
            }
            else if(__key == -2)
            {
                    // do nothing, keep the previous state
            }   
            else _kstate = MAYBE_RELEASED;
        break;

        case MAYBE_RELEASED:
            if(__key != -1 && __key != -2){
                _kstate = PUSHED;
            }
            else if(__key == -2)
            {       
                // do nothing, keep the previous state
            }
            else
            {   
                _kstate = RELEASED;
                //_kpFlag = 0;
            }
            break;
    }

    if(_kpFlag == FLAG_ON){
        g_u8Key = __key;
        _kpFlag = FLAG_OFF;
    }   
    return &g_u8Key;
}


// this function stops the cpu from doing anything else while waiting for a key press
// un-debounced
int8_t keypad_waitForKey(void)
{
    uint8_t _key = 0, counter = 0;

    KEYPAD_DDR = 0xf0;
    KEYPAD_PORT = 0x0f;
    do{
        _delay_us(5);
        _key = KEYPAD_PIN & 0x0f;
    }while( _key == 0x0f );

    // check coloumns
    KEYPAD_DDR = 0x0f;
    KEYPAD_PORT = 0xf0;
    _delay_us(5);
    _key = _key | (KEYPAD_PIN & 0Xf0);

    for(counter = 0 ; counter < 16 ; counter++)
    {
        if(_keypad[counter] == _key) break;
    }
    if(counter == 16)   return -1;
    return keypadMap[counter];
}
