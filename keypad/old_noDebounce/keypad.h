// this lib uses the internal pull ups for easier interface
// this lib requires only one button to be pressed otherwise it will not operate as if nothing is being pressed

#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define __ASCII
//#define __NUMERICAL
#define _MAX_KEYS 16

#define _CONCAT(a,b) a##b
#define PORT(x) _CONCAT(PORT,x)
#define PIN(x) _CONCAT(PIN,x)
#define DDR(x) _CONCAT(DDR,x)

#define KEYPAD_DDR  DDR(C)
#define KEYPAD_PORT  PORT(C)
#define KEYPAD_PIN  PIN(C)

#define ROW1 0
#define COL1 4

#ifdef __ASCII
static unsigned char keypadMap[_MAX_KEYS] = {'1','2','3','A', 
                                             '4','5','6','B', 
                                             '7','8','9','C', 
                                             '*','0','#','D'};
#elif defined (__NUMERICAL)
static unsigned char keypadMap[_MAX_KEYS] = {1, 2, 3, 0x0a, 
                                             4, 5, 6, 0x0b, 
                                             7, 8, 9, 0x0c, 
                                             0x0e, 0, 0x0f, 0x0d};
#endif

uint8_t keypad_waitForKey(void);
int8_t keypad_getKey(void);

#endif
