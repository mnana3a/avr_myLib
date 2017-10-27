#include "rt.h"

volatile unsigned long int __time = 0;  // value of msec since last reset ( running cpu time)

void rt_init(void)
{
    TIMSK |= (1<<1);    // timer 0 compare mode interrupt enable
    OCR0 = 249; // timer0 interrupts each 1 msec
    TCCR0 |= (1<<3) | (1<<0) | (1<<1);
    sei();
}

ISR(TIMER0_COMP_vect)
{
    __time++;
}
