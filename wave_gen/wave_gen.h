// the max freq  = F_CPU / 2
// dont try to opertate oc1a & oc1b at the same time, as tcnt1 gets resetted when it first compares to any
// if you try to operate both oc1a & oc1b at the same time only the one with the hightest freq will operate and the second generator will work on that same freq.
// only intentionally operate both oc1a & oc1b at the saame time if the freq is the exact same for both

#ifndef __WAVE_GEN_H__
#define __WAVE_GEN_H__

#ifndef F_CPU
    #define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <inttypes.h>

// NOTE: added for geany proj plug-in to catch the def and go to the file by pressing on
// lcd.h
extern uint8_t wave_gen;

// use only one which isnt conflicting with the rest of the application
// if icr1 is used in the app use ocr1a for this lib and it works the other way
#define __WAVE_USE_OCR1A
//#define __WAVE_USE_ICR1


#define W0PIN   3
#define W2PIN 7
#define W1APIN 5
#define W1BPIN 4


void waveGenerator0_set(uint32_t freq);
void waveGenerator1A_set(uint32_t freq);
void waveGenerator1B_set(uint32_t freq);
void waveGenerator2_set(uint32_t freq);

void waveGenerator0_stop(void);
void waveGenerator1A_stop(void);
void waveGenerator1B_stop(void);
void waveGenerator2_stop(void);


#endif
