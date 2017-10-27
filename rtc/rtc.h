#ifndef __RTC_H__
#define __RTC_H__

#ifndef F_CPU
    #define F_CPU 16000000UL
#endif
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// NOTE: added for geany proj plug-in to catch the def and go to the file by pressing on
// lcd.h
extern uint8_t rtc;

void init_RTC(void);

#endif
