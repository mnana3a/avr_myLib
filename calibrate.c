// this function can be used to calibrate the internal rc of a microcontroller
// notice this function is written with atmega169 in mind so it needs
// modification in order to work with other microcontrollers

/* funciton name : OSCCAL_calibration
 * returns       : None
 * Parameters    : None
 * Purpose       : Calibrate the Internal OSCCAL Byte, using external watch osc as a reference
*/

// timer 2 works in asyn op. and timer 1 works with internal rc

void OSCCAL_calibration(void)
{
    unsigned char calibrate = 0;
    int temp = 0;
    unsigned char tempL = 0;
    
    CKLPR |= (1<<CLKPCE);       // to enable the prescaler in atmega169
    CLKPR |= (1<<CLKPS1) | (1<<CLKPS0); // set prescaler to 8 so the rc = 8 / 8 = 1 MHz
    TIMSK2 = 0;                 // disable interrupts related to timer2
    ASSR |= (1<<AS2);           // enable timer2 asynch op.
    OCR2A = 200;                // timer2 compare register
    TCCR1B |= (1<<CS10);        // start timer1 with no prescaling
    TCCR2A |= (1<<CS20);        // start timer2 with no prescaling

    // wait for TCN2UB and TCR2UB busy flag, required for timer2 asynch op.
    while((ASSR & 0X01) | (ASSR & 0X04));
    _delay_ms(1000);            // wait for external osc to stabilise

    while(!calibrate){
        cli();
        TIFR1 = 0XFF;
        TIFR2 = 0XFF;           // clear the flag bits in both timer1 & timer2
        
        TCNT1H = 0;
        TCNT1L = 0;
        TCNT2 = 0;              // clear the counting in both timers
    
        while(!(TIFR2 & (1<<OCF2A));    // wait for timer2 compare flag
        TCCR1B = 0;             // stop timer1
        sei();

        if((TIFR1 & (1<<TOV1))){
            temp = 0xffff;
        }
        else{
            tempL = TCNT1L;
            temp = TCNT1H << 8;
            temp |= tempL;
        }

        // 1 Mhz == 32,768
        // x     == 201
        // x = (201 * 1 Mhz)/32,768 = 6134
        if(temp > 6250)     OSCCAL--;       // fast
        else if(temp < 6120)    OSCCAL++;   // slow
        else    calibrate = 1;      // internal rc is correct
        
        TCCR1B |= (1<<CS10);     // start timer1 for the next if not correct
    }
}
