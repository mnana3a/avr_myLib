// this lib uses the internal pull ups for easier interface
// this lib method uses ack (feedback) from the main s/w to move on to the next fetch
// i.e, key read --> when read by main clear(ack) --> wait for relase --> read next key
//      key read --> nack --> keep it until realse, if released then pressed before ack it is lost and the new value is presented.

// to use this lib:
// int8_t *key = 0;
// task_keypad(){key = keypad_getKey();}
// task_lcd(){if(*key){........; *key = 0;}}

#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#ifndef F_CPU
    #define F_CPU 16000000UL
#endif
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

// NOTE: added for geany proj plug-in to catch the def and go to the file by pressing on
// lcd.h
extern uint8_t keypad;

#define __ASCII
//#define __NUMERICAL
#define _MAX_KEYS 16


#define FLAG_ON 255
#define FLAG_OFF 0


// this is the scheduling time for button checking
#define KEYPAD_T 25

enum STATE {RELEASED, MAYBE_RELEASED, PUSHED, MAYBE_PUSHED};
extern enum STATE _kstate;
int8_t g_u8Key;


int8_t keypad_waitForKey(void);
int8_t keypad_key(void);            // un-debounced
int8_t *keypad_getKey(void);        // debounced

#endif
