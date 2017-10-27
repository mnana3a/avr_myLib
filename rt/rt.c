#include "rt.h"

volatile unsigned long int __time = 0;  // value of msec since last reset ( running cpu time)

void rt_init(void)
{
    TIMSK |= (1<<1);    // timer 0 compare mode interrupt enable
    OCR0 = 249; // timer0 interrupts each 1 msec
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
        TCCR0 |= (1<<3) | (1<<0) | (1<<1);
    #elif defined(__AVR_ATmega128__)
        TCCR0 |= (1<<3) | (1<<2);
    #endif
    sei();
}

ISR(TIMER0_COMP_vect)
{
    __time++;
}
