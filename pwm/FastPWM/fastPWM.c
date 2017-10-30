#include "fastPWM.h"

// timer 0
void fastPWM0_init(uint32_t freq)
{
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    uint8_t pre = 0;
    // freq is limited to a specific set of values depending on the chosen prescaler
    // freq = [62500 , 7812.5 , 976.5 , 244.14 , 61]  for atmega32/16
    // freq = [62500 , 7812.5 , 1953 , 976.5 , 488 , 244.14 , 61]  for atmega128
    // select prescaler with the min. possible error margin
    
    // freq = F_CPU / (256*Prescaler)
    // for default value set the freq argument with the value 0 [61 kHz]
    if(freq <= 240){
        prescaler = 1024;
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
            pre = 5;
        #elif defined(__AVR_ATmega128__)
            pre = 7;
        #endif
    }
    else
    {
        prescaler = F_CPU / (freq * 256);
        // the result is a log curve
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
            if(prescaler < 1)   pre = 1;
            else if(prescaler >= 1 && prescaler <= 2)   pre = 1;
            else if(prescaler > 2 && prescaler <= 14)   pre = 2;
            else if(prescaler > 14 && prescaler <= 102) pre = 3;
            else if(prescaler > 102 && prescaler <= 152)    pre = 4;
            else if(prescaler > 152)   pre = 5;
        #elif defined(__AVR_ATmega128__)
            if(prescaler < 1)   pre = 1;
            else if(prescaler >= 1 && prescaler <= 2)   pre = 1;
            else if(prescaler > 2 && prescaler <= 12)   pre = 2;
            else if(prescaler > 12 && prescaler <= 42) pre = 3;
            else if(prescaler > 42 && prescaler <= 85)    pre = 4;
            else if(prescaler > 85 && prescaler <= 170)   pre = 5;
            else if(prescaler > 192 && prescaler <= 409)   pre = 6;
            else if(prescaler > 409)   pre = 7;
        #endif
    }
    // NOTE: should make TCCR0 = not with | as changing the freq would cause error
    // set the prescaler in the register
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
        TCCR0 = (1<<3) | (1<<5) | (1<<6) | pre;
    #endif

    // 0c0 must be set output
    W0DDR |= (1<<W0PIN);
    TCNT0 = 0;
}

void fastPWM0_duty(uint8_t duty)
{
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR0 |= (1<<5);
    if(duty == 0)   fastPWM0_stop();
    else    OCR0 = ((duty * 256) / 100) - 1;
}

void fastPWM0_stop(void)
{
    // notice this way you stop timer 0 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR0 = 0;

    // this way you dont stop the timer so you wont need to reinit
    OCR0 = 0;
    TCNT0 = 0;
    // oc0 pin diconnected, normal port operation
    TCCR0 &= ~(1<<4);
    TCCR0 &= ~(1<<5);
}

/***********************************************************************/
// timer 2

void fastPWM2_init(uint32_t freq)
{
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    uint8_t pre = 0;
    // freq is limited to a specific set of values depending on the chosen prescaler
    // freq = [62500 , 7812.5 , 976.5 , 244.14 , 61]  for atmega32/16
    // freq = [62500 , 7812.5 , 1953 , 976.5 , 488 , 244.14 , 61]  for atmega128
    // select prescaler with the min. possible error margin
    
    // freq = F_CPU / (256*Prescaler)
    // for default value set the freq argument with the value 0 [61 kHz]
    if(freq <= 240){
        prescaler = 1024;
        #if defined(__AVR_ATmega128__)
            pre = 5;
        #elif defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
            pre = 7;
        #endif
    }
    else
    {
        prescaler = F_CPU / (freq * 256);
        // the result is a log curve
        #if defined(__AVR_ATmega128__)
            if(prescaler < 1)   pre = 1;
            else if(prescaler >= 1 && prescaler <= 2)   pre = 1;
            else if(prescaler > 2 && prescaler <= 14)   pre = 2;
            else if(prescaler > 14 && prescaler <= 102) pre = 3;
            else if(prescaler > 102 && prescaler <= 152)    pre = 4;
            else if(prescaler > 152)   pre = 5;
        #elif defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
            if(prescaler < 1)   pre = 1;
            else if(prescaler >= 1 && prescaler <= 2)   pre = 1;
            else if(prescaler > 2 && prescaler <= 12)   pre = 2;
            else if(prescaler > 12 && prescaler <= 42) pre = 3;
            else if(prescaler > 42 && prescaler <= 85)    pre = 4;
            else if(prescaler > 85 && prescaler <= 170)   pre = 5;
            else if(prescaler > 192 && prescaler <= 409)   pre = 6;
            else if(prescaler > 409)   pre = 7;
        #endif
    }
    // NOTE: should make TCCR0 = not with | as changing the freq would cause error
    // set the prescaler in the register
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
        TCCR2 = (1<<3) | (1<<5) | (1<<6) | pre;
    #endif

    // 0c2 must be set output
    W2DDR |= (1<<W2PIN);
    TCNT2 = 0;
}

void fastPWM2_duty(uint8_t duty)
{
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR2 |= (1<<5);
    if(duty == 0)   fastPWM2_stop();
    else    OCR2 = ((duty * 256) / 100) - 1;
}

void fastPWM2_stop(void)
{
    // notice this way you stop timer 0 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR0 = 0;

    // this way you dont stop the timer so you wont need to reinit
    OCR2 = 0;
    TCNT2 = 0;
    // oc2 pin diconnected, normal port operation
    TCCR2 &= ~(1<<4);
    TCCR2 &= ~(1<<5);
}

/************************************************************************/
// timer 1
// OC1A

//FIXME : make the freq float
//FIXME : change the hardcoded freq numbers into F_CPU/2... freq dependent numbers
void fastPWM1A_init(uint32_t freq)
{
    // control OC1A only
    // this routine works with non-inverted mode only
    // and using ICR1 for top value only
    uint16_t prescaler = 0;
    uint8_t pre = 0;
    float ffreq = 0.0;
    /* freq can be
            top = 0xff  --> f = 62500 : 61 hz   with specific values depending on pre
            top = 0x1ff --> f = 31250 : 30 hz   with specific values depending on pre
            top = 0x3ff --> f = 15625 : 15 hz   with specific values depending on pre
            top = ICR1 || OCR1A
                    pre = 1    -->  f = 16 M  : 244  hz
                    pre = 1024 -->  f = 15625 : 0.23 hz
    */
    // freq = F_CPU / ((top + 1)*Prescaler)

    // set top reg. with the required value for that freq.
    if(freq == 0){
        prescaler = 1024;
        ffreq = 0.23;
        #if defined(__PWM_USE_ICR1)
            ICR1 = (uint16_t)((float)((float)F_CPU / ffreq) / prescaler) - 1;
        #elif defined(__PWM_USE_OCR1A)
            OCR1A = (uint16_t)((float)((float)F_CPU / ffreq) / prescaler) - 1;
        #endif
    }
    else
    {
        // change the 16M to F_CPU
        if(freq >= 244 && freq <= 16000000) prescaler = 1;
        else if(freq >= 30 && freq <= 2000000)  prescaler = 8;
        else if(freq >= 4 && freq <= 250000)    prescaler = 64;
        else if(freq >= 1 && freq <= 62500) prescaler = 256;
        else if(freq >= 0 && freq <= 15625) prescaler = 1024;

        ICR1 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
    }

    // set the prescaler in the register & mode 14
    // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
    //        that disabled me from using both OC1A & OC1B atm..
    //        now i can use them both but they must have the same freq
    switch(prescaler)
    {
        case 1:
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<0) | (1<<3) | (1<<4);
            TCCR1B &= ~(1<<1);
            TCCR1B &= ~(1<<2);
            break;
        case 8:
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<1) | (1<<3) | (1<<4);
            TCCR1B &= ~(1<<0);
            TCCR1B &= ~(1<<2);
            break;
        case 64:
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<0) | (1<<1) | (1<<3) | (1<<4);
            TCCR1B &= ~(1<<2);
            break;
        case 256:
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<2) | (1<<3) | (1<<4);
            TCCR1B &= ~(1<<0);
            TCCR1B &= ~(1<<1);
            break;
        case 1024:
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<0) | (1<<2) | (1<<3) | (1<<4);
            TCCR1B &= ~(1<<1);
            break;
        default:
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<0) | (1<<3) | (1<<4);
            TCCR1B &= ~(1<<1);
            TCCR1B &= ~(1<<2);
            break;
    }

    // pd5 must be set output ( oc0 )
    DDRD |= (1<<PWM1A);
    // init TCNT0 to 0
    TCNT1H = 0;
    TCNT1L = 0;
}

// it is fat better to make the duty cycle input a float for better accuracy
void fastPWM1A_duty(float duty)
{
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR1A |= (1<<7);
    // duty cycle = (OCR1x + 1) / (top + 1)
    if(duty <= 0)   fastPWM1A_stop();
    else    OCR1A = (uint16_t)((float)((float)((float)duty * (float)(ICR1 + 1)) / 100) - 1);
    // old : the uint32_t cast is a must here as : duty * ICR1 value cant be stored in 16 bits
    // new : made it float for better accuracy
}

void fastPWM1A_stop(void)
{
    // notice this way you stop timer 0 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR0 = 0;

    // this way you dont stop the timer so you wont need to reinit
    OCR1A = 0;
    TCNT1 = 0;
    // oc1A pin diconnected, normal port operation
    TCCR1A &= ~(1<<7);
    TCCR1A &= ~(1<<6);
}

/**************************************************************************/
// timer 1
// OC1B

//FIXME : make the freq float
void fastPWM1B_init(uint32_t freq)
{
    // control OC1B only
    // this routine works with non-inverted mode only
    // and using ICR1 for top value only
    uint16_t prescaler = 0;
    float ffreq = 0.0;
    /* freq can be
            top = 0xff  --> f = 62500 : 61 hz       with specific values depending on pre
            top = 0x1ff --> f = 31250 : 30 hz   with specific values depending on pre
            top = 0x3ff --> f = 15625 : 15 hz   with specific values depending on pre
            top = ICR1 || OCR1A
                    pre = 1    -->  f = 16 M  : 244  hz
                    pre = 1024 -->  f = 15625 : 0.23 hz
    */
    // freq = F_CPU / ((top + 1)*Prescaler)

    // set ICR1 with the required value for that freq.
    if(freq == 0){
        prescaler = 1024;
        ffreq = 0.23;

        ICR1 = (uint16_t)((float)((float)F_CPU / ffreq) / prescaler) - 1;
    }
    else
    {
        if(freq >= 244 && freq <= 16000000) prescaler = 1;
        else if(freq >= 30 && freq <= 2000000)  prescaler = 8;
        else if(freq >= 4 && freq <= 250000)    prescaler = 64;
        else if(freq >= 1 && freq <= 62500) prescaler = 256;
        else if(freq >= 0 && freq <= 15625) prescaler = 1024;

        ICR1 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
    }

    // set the prescaler in the register & mode 14
    // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
    //        that disabled me from using both OC1A & OC1B atm..
    //        now i can use them both but they must have the same freq
    switch(prescaler)
    {
        case 1:
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<0) | (1<<3) | (1<<4);
            TCCR1B &= ~(1<<1);
            TCCR1B &= ~(1<<2);
            break;
        case 8:
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<1) | (1<<3) | (1<<4);
            TCCR1B &= ~(1<<0);
            TCCR1B &= ~(1<<2);
            break;
        case 64:
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<0) | (1<<1) | (1<<3) | (1<<4);
            TCCR1B &= ~(1<<2);
            break;
        case 256:
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<2) | (1<<3) | (1<<4);
            TCCR1B &= ~(1<<0);
            TCCR1B &= ~(1<<1);
            break;
        case 1024:
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<0) | (1<<2) | (1<<3) | (1<<4);
            TCCR1B &= ~(1<<1);
            break;
        default:
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<0) | (1<<3) | (1<<4);
            TCCR1B &= ~(1<<1);
            TCCR1B &= ~(1<<2);
            break;
    }

    // pd5 must be set output ( oc0 )
    DDRD |= (1<<PWM1B);
    // init TCNT0 to 0
    TCNT1H = 0;
    TCNT1L = 0;
}

void fastPWM1B_duty(float duty)
{
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR1B |= (1<<5);
    // duty cycle = (OCR1x + 1) / (top + 1)
    if(duty <= 0)   fastPWM1B_stop();
    else    OCR1B = (uint16_t)((float)((float)((float)duty * (float)(ICR1 + 1)) / 100) - 1);
    // old : the uint32_t cast is a must here as : duty * ICR1 value cant be stored in 16 bits
    // new : made it float for better accuracy
}

void fastPWM1B_stop(void)
{
    // notice this way you stop timer 0 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR0 = 0;

    // this way you dont stop the timer so you wont need to reinit
    OCR1B = 0;
    TCNT1 = 0;
    // oc1B pin diconnected, normal port operation
    TCCR1A &= ~(1<<5);
    TCCR1A &= ~(1<<4);
}
