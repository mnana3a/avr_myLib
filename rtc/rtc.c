// USER: to use this in the source code : write the function to be done each 1 second and assign that function to the fn pointer RTC_ISR

#include "rtc.h"

// this routine uses timer 2 overflow interrupt after 256 ticks i.e, 1 second
// if the timer 2 is 16-bit (on some chips) then the prescaler can be set to 1 and the count to 32768
// so in the source code add
// ISR(TIMER2_OVF_vect)  to add the code to be done each 1 second

static void (*RTC_ISR)(void) = (void *) 0;

void rtc_init(void)
{
    // delay for the crystal to stablize
    for (uint8_t i = 0; i <= 255;)
    {
        i++;
    }
    TIMSK = 0;
    ASSR |= (1<<3);     // ASYNCH OP
    #if defined(__AVR_ATmega32__) || defined(__AVR_ATmega16__)
        TCCR2 |= (1<<0) | (1<<2);   // must set the prescaler to 128
        TCNT2 = 0;
    #elif defined(__AVR_ATmega128__)
        TCCR0 |= (1<<0) | (1<<2);   // must set the prescaler to 128
        TCNT0 = 0;
    #endif
    while((ASSR & (1<<0)) && (ASSR & (1<<2)));    // WAIT FOR THE BUSY FLAGS
    TIFR = 0XFF;
    #if defined(__AVR_ATmega32__) || defined(__AVR_ATmega16__)
        TIMSK |= (1<<6);      // ENABLE TIMER2 OV int
    #elif defined(__AVR_ATmega128__)
        TIMSK |= (1<<0);      // ENABLE TIMER0 OV int
    #endif
    sei();
}

// this isr executes once each second
#if defined(__AVR_ATmega32__) || defined(__AVR_ATmega16__)
ISR(TIMER2_OVF_vect)
{
    RTC_ISR();
}

#elif defined(__AVR_ATmega128__)
ISR(TIMER0_OVF_vect)
{
    RTC_ISR();
}

#endif

// use it at run time to run a user-defined function when transmission compeletes by interrupt
void rtc_callbackFn(void (*ptrToRTC_ISR)(void))
{
    RTC_ISR = ptrToRTC_ISR;
}
