#ifndef __RTC_H__
#define __RTC_H__

#ifndef F_CPU
    #define F_CPU 16000000UL
#endif
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// NOTE: added for geany proj plug-in to catch the def and go to the file by pressing on
// rtc.h
extern uint8_t rtc;

#if defined(__AVR_ATmega32__) || defined(__AVR_ATmega16__)
    #define PC7 TOSC2
    #define PC6 TOSC1
#elif defined(__AVR_ATmega128__)
    #define PG3 TOSC2
    #define PG4 TOSC1
#endif

void rtc_init(void);
void rtc_callbackFn(void (*ptrToRTC_ISR)(void));

#endif
