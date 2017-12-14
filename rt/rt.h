/*
*   Function : creates a timer Interrupt every 1 msec that is used for time purposes
*   Notes    : it uses TIMER0 CTC Mode for operation
*/

#ifndef __RT_H
#define __RT_H

#ifndef F_CPU
    #define F_CPU 16000000UL
#endif
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// NOTE: added for geany proj plug-in to catch the def and go to the file by pressing on
// lcd.h
extern uint8_t rt;

#define MILLIS __time
#define MILLIS() __time

#ifndef __GNUC__
#define __inline__ inline
#endif

extern volatile unsigned long int __time;   // value of msec since last reset ( running cpu time)

void rt_init(void);
extern inline unsigned long millis(void) __attribute__((always_inline));

extern inline unsigned long millis(void)
{
    return __time;
}

#endif
