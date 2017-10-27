// MOD: added auto clear to the button flag state so no need to clear it urself in the code... but may cause event loss as before you record it and clean the flag later yourself but now unless the event occur while u r pressing the button you will miss it, but it is meant to be this way
// this is a state machine for the button to avoid debouncing
// it is considered as a task that should run every 30 msec

// not always a good idea to use this with buttons as it takes 30:60 msec to detect a press 
// sometimes you wont need to dounce a push but to debounce the release of the button

//                                                      OLD
// when using this lib :
// set the pin and change it here but must set the input in the driver
// uint8_t *state;
// state = buttonState();
// if(*state){//do somehting;   *state = 0;}
// MAKE SURE to clear the flag after using it
// this lib is mainly meant for rt usage, it uses rt.h for scheduling but can be used seperately

// it uses a pointer to the global var so i dont have to kno the var name in the driver program just a pointer to it and modify the var usig the pointer
#ifndef __BPUT_H__
#define __BPUT_H__

#include <inttypes.h>
#include <avr/io.h>
#include "rt.h"
#ifndef F_CPU
    #define F_CPU 16000000UL
#endif

// NOTE: added for geany proj plug-in to catch the def and go to the file by pressing on
// lcd.h
extern uint8_t pbut;

// this is the scheduling time for button checking
#define BUT_T 30

// change according to the used pin
#define BUT_PIN 0
#define BUT_PORT PINB

extern uint8_t bFlag;

enum STATE {RELEASED, MAYBE_RELEASED, PUSHED, MAYBE_PUSHED};
extern enum STATE _state;

uint8_t *pbut_state(void);

#endif
