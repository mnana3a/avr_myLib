#include "rtc.h"

// this routine uses timer 2 overflow interrupt after 256 ticks i.e, 1 second
// so in the source code add
// ISR(TIMER2_OVF_vect)  to add the code to be done each 1 second
// or modify according to the required
void init_RTC(void)
{
  TIMSK = 0;
  ASSR |= (1<<3);     // ASYNCH OP
  TCNT2 = 0;
  TCCR2 |= (1<<0) | (1<<2);   // must set the prescaler to 128
  while((ASSR & (1<<0)) && (ASSR & (1<<2)));    // WAIT FOR THE BUSY FLAGS
  TIFR = 0XFF;
  TIMSK |= (1<<6);      // ENABLE TIMER2 OV int
    sei();
}
