//TODO : check if float freq required for time1/timer3

//NOTE : dont operate oc1c with oc1a or oc1b
//NOTE : dont operate oc1c with oc2 as they share the same pin
//NOTE : any module who is common with another and called to function will operate at the same freq its ancestor is operating... if oc1a is on 20hz and called oc1b it will operate on 20 hz regardless of the freq input.. if oc1b isnt called then it wont connect the generator and the pin works as a gpio

// the max freq  = F_CPU / 2

//NOTE : dont try to opertate oc1a & oc1b at the same time, as tcnt1 gets resetted when it first compares to any
// if you try to operate both oc1a & oc1b at the same time only the one with the hightest freq will operate and the second generator will work on that same freq.
// only intentionally operate both oc1a & oc1b at the saame time if the freq is the exact same for both

#ifndef __WAVE_GEN_H__
#define __WAVE_GEN_H__

#ifndef F_CPU
    #define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <inttypes.h>

//NOTE : added for geany proj plug-in to catch the def and go to the file by pressing on
// lcd.h
extern uint8_t wave_gen;

// use only one which isnt conflicting with the rest of the application
// if icr1 is used in the app use ocr1a for this lib and it works the other way
#define __WAVE_USE_ICR1

#ifdef __CAP_USE_ICR1
    #define __WAVE_USE_OCR1A
    #undef __WAVE_USE_ICR1
#endif

#if defined(__AVR_ATmega128__)
    #define __WAVE_USE_ICR3
    #ifdef __CAP_USE_ICR3
        #define __WAVE_USE_OCR3A
        #undef __WAVE_USE_ICR3
    #endif
#endif

#if defined(__AVR_ATmega32__) || defined(__AVR_ATmega16__)
    #define W0DDR DDRB
    #define W1DDR DDRD
    #define W2DDR DDRD
    #define W0PIN   3
    #define W1APIN 5
    #define W1BPIN 4
    #define W2PIN 7
#elif defined(__AVR_ATmega128__)
    #define W0DDR DDRB
    #define W1DDR DDRB
    #define W2DDR DDRB
    #define W3DDR DDRE
    #define W0PIN   4
    #define W1APIN 5
    #define W1BPIN 6
    #define W1CPIN 7
    #define W2PIN 7
    #define W3APIN 3
    #define W3BPIN 4
    #define W3CPIN 5
#endif

#define BUFFER0 5
#define BUFFER1 7

void waveGenerator0_set(uint32_t freq);
void waveGenerator1A_set(uint32_t freq);
void waveGenerator1B_set(uint32_t freq);
void waveGenerator2_set(uint32_t freq);

void waveGenerator0_stop(void);
void waveGenerator1A_stop(void);
void waveGenerator1B_stop(void);
void waveGenerator2_stop(void);

#if defined(__AVR_ATmega128__)
void waveGenerator1C_set(uint32_t freq);
void waveGenerator1C_stop(void);
void waveGenerator3A_set(uint32_t freq);
void waveGenerator3A_stop(void);
void waveGenerator3B_set(uint32_t freq);
void waveGenerator3B_stop(void);
void waveGenerator3C_set(uint32_t freq);
void waveGenerator3C_stop(void);
#endif

#endif
