#include "phasePWM.h"

static uint8_t g_u8Timer1_flag = 0;
static uint8_t g_u8Timer2_flag = 0;

#if defined(__AVR_ATmega128__)
    static uint8_t g_u8Timer3_flag = 0;
#endif


// timer 0
void phasePWM0_init(uint32_t freq)
{
    // this routine works with non-inverted mode only
    uint8_t pre = 0;

    // freq = F_CPU / (510*Prescaler)
    // for default value set the freq argument with the value 0 [61 kHz]
    // freq is limited to a specific set of values depending on the chosen prescaler
    // these next values depend on crystal = 16 Mhz and will be different if another crystal is being used
    // freq = [31372.5 , 3921.5 , 490.2 , 122.5 , 30.6]  for atmega32/16
    // freq = [31372.5 , 3921.5 , 980.4 , 490.2 , 245 , 122.5 , 30.6]  for atmega128
    // select prescaler with the min. possible error margin
    
    // the result is a log curve
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
        if(freq >= AV_01)                          pre = 1;
        else if(freq < AV_01 && freq >= AV_02)     pre = 2;
        else if(freq < AV_02 && freq >= AV_03)     pre = 3;
        else if(freq < AV_03 && freq >= AV_04)     pre = 4;
        else if(freq < AV_04)                      pre = 5;
    #elif defined(__AVR_ATmega128__)
        if(freq >= AV_11)                          pre = 1;
        else if(freq < AV_11 && freq >= AV_12)     pre = 2;
        else if(freq < AV_12 && freq >= AV_13)     pre = 3;
        else if(freq < AV_13 && freq >= AV_14)     pre = 4;
        else if(freq < AV_14 && freq >= AV_15)     pre = 5;
        else if(freq < AV_15 && freq >= AV_16)     pre = 6;
        else if(freq < AV_16)                      pre = 7;
    #endif

    // NOTE: should make TCCR0 = not with | as changing the freq would cause error
    // set the prescaler in the register
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
        TCCR0 = (1<<5) | (1<<6) | pre;
    #endif

    // 0c0 must be set output
    PWM0DDR |= (1<<PWM0PIN);
    TCNT0 = 0;
}

void phasePWM0_duty(uint8_t duty)
{
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR0 |= (1<<5);
    
    // duty = (2 * OCR0) / 510  || OCR0 / 255
    OCR0 = (uint8_t)((uint32_t)((uint32_t)((uint16_t)duty * 255) * DIV100_16) >> 16);
}

void phasePWM0_stop(void)
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

void phasePWM2_init(uint32_t freq)
{
    // this routine works with non-inverted mode only
    uint8_t pre = 0;

    // freq = F_CPU / (256*Prescaler)
    // for default value set the freq argument with the value 0 [61 kHz]
    // freq is limited to a specific set of values depending on the chosen prescaler
    // freq = [62500 , 7812.5 , 976.5 , 244.14 , 61]  for atmega32/16
    // freq = [62500 , 7812.5 , 1953 , 976.5 , 488 , 244.14 , 61]  for atmega128
    // select prescaler with the min. possible error margin

    if (!g_u8Timer2_flag)
    {
        // the result is a log curve
        #if defined(__AVR_ATmega128__)
            if(freq >= AV_01)                          pre = 1;
            else if(freq < AV_01 && freq >= AV_02)     pre = 2;
            else if(freq < AV_02 && freq >= AV_03)     pre = 3;
            else if(freq < AV_03 && freq >= AV_04)     pre = 4;
            else if(freq < AV_04)                      pre = 5;
        #elif defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
            if(freq >= AV_11)                          pre = 1;
            else if(freq < AV_11 && freq >= AV_12)     pre = 2;
            else if(freq < AV_12 && freq >= AV_13)     pre = 3;
            else if(freq < AV_13 && freq >= AV_14)     pre = 4;
            else if(freq < AV_14 && freq >= AV_15)     pre = 5;
            else if(freq < AV_15 && freq >= AV_16)     pre = 6;
            else if(freq < AV_16)                      pre = 7;
        #endif
        
        // NOTE: should make TCCR0 = not with | as changing the freq would cause error
        // set the prescaler in the register
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
            TCCR2 = (1<<5) | (1<<6) | pre;
        #endif

        // 0c2 must be set output
        PWM2DDR |= (1<<PWM2PIN);
        TCNT2 = 0;
        g_u8Timer2_flag = 1; 
    }
    else
    {
        // do nothing as atmega128 oc2 & oc1c share the same pin
    }
}

void phasePWM2_duty(uint8_t duty)
{
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR2 |= (1<<5);

    OCR2 = (uint8_t)((uint32_t)((uint32_t)((uint16_t)duty * 255) * DIV100_16) >> 16);
}

void phasePWM2_stop(void)
{
    // notice this way you stop timer 0 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR0 = 0;

    // this way you dont stop the timer so you wont need to reinit
    OCR2 = 0;
    TCNT2 = 0;
    // oc2 pin diconnected, normal port operation
    TCCR2 &= ~(1<<4);
    TCCR2 &= ~(1<<5);
    g_u8Timer2_flag = 0; 
}
/************************************************************************/
// timer 1
// OC1A

// OPTION: make the freq float
void phasePWM1A_init(uint32_t freq)
{
    // control OC1A only
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    uint8_t pre = 0;
    
    /* freq can be
            top = 0xff  --> f = 31372.5 : 30.6 hz       with specific values depending on pre
            top = 0x1ff --> f = 15655.57 : 15.3 hz  with specific values depending on pre
            top = 0x3ff --> f = 7820.13 : 7.6 hz    with specific values depending on pre
            top = ICR1 || OCR1A
                    pre = 1    -->  f = 8 M    : 122  hz
                    pre = 1024 -->  f = 7812.5 : 0.119 hz
    */
    
    // freq = F_CPU / (2 * Prescaler * top)
    if (!g_u8Timer1_flag)
    {
        // BUG: this freq will not be accurate as the min. resolution of the generator is 2 bits (0x0003) which yields 4 Mhz max freq and may get away with 8 Mhz

        if(freq >= (uint32_t)(F_CPU >> 1))
            {prescaler = 1; pre = 1;}
        else if (freq < (uint32_t)(F_CPU >> 1) && freq >= (uint32_t)((F_CPU >> 17)+1ul))
            {prescaler = 1; pre = 1;}
        else if (freq < (uint32_t)(F_CPU >> 4) && freq >= (uint32_t)((F_CPU >> 20)+1ul))
            {prescaler = 8; pre = 2;}
        else if (freq < (uint32_t)(F_CPU >> 7) && freq >= (uint32_t)((F_CPU >> 23)+1ul))
            {prescaler = 64; pre = 3;}
        // FIXME: requires float freq value to be able to generate freq lower than 1 hz
        else if (freq < (uint32_t)(F_CPU >> 9) && freq >= (uint32_t)1)
            {prescaler = 256; pre = 4;}
        else if (freq < 1ul)
            {prescaler = 1024; pre = 5;}


        // TODO: change the software division to something else for more performance at initalization time
        #if defined( __PWM_USE_ICR1) || defined (__PWM_USE_OCR1A)
            ICR1 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / (2 * prescaler));
        #elif defined (__PWM_USE_OCR1A)
            // cant be used to define top value here as it is used to define duty cycle
            //OCR1A = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
        #endif

        // set the prescaler in the register
        // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
        //        that disabled me from using both OC1A & OC1B atm..
        //        now i can use them both but they must have the same freq
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<4) | pre;
            
            #if defined(__PWM_USE_ICR1) || defined(__PWM_USE_OCR1A)
                TCCR1A &= ~(1<<0);
            #elif defined(__PWM_USE_OCR1A)
                // cant be used with this generator as OCR1A defines top value already
                //TCCR1A |= (1<<0);
            #endif
        #endif

        // oc1a must be set output
        PWM1DDR |= (1<<PWM1APIN);
        TCNT1H = 0;
        TCNT1L = 0;
        // set this global flag to indicate oc1a is running with a specifi prescaler
        g_u8Timer1_flag = 1;
    }
    else
    {
        // do nothing as it will mess with oc1b output

        // set the prescaler in the register
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<4);
        #endif
        // 0c1a must be set output
        PWM1DDR |= (1<<PWM1APIN);
    }
}

// NOTE: replaced float duty cycle used for accuracy into an int but removed " - 1" to get the same result i get from float calculations
void phasePWM1A_duty(uint8_t duty)
{
    // duty cycle = OCR1x / top
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR1A |= (1<<7);
    
    OCR1A = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * ICR1) * DIV100_16) >> 16);
}

void phasePWM1A_stop(void)
{
    // notice this way you stop timer 0 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR0 = 0;

    // this way you dont stop the timer so you wont need to reinit
    OCR1A = 0;
    TCNT1 = 0;
    // oc1A pin diconnected, normal port operation
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
        TCCR1A &= ~(1<<7);
        TCCR1A &= ~(1<<6);
    #endif
    g_u8Timer1_flag = 0;
}

/**************************************************************************/
// timer 1
// OC1B

// OPTION: make the freq float
void phasePWM1B_init(uint32_t freq)
{
    // control OC1ABonly
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    uint8_t pre = 0;
    
    /* freq can be
            top = 0xff  --> f = 31372.5 : 30.6 hz       with specific values depending on pre
            top = 0x1ff --> f = 15655.57 : 15.3 hz  with specific values depending on pre
            top = 0x3ff --> f = 7820.13 : 7.6 hz    with specific values depending on pre
            top = ICR1 || OCR1A
                    pre = 1    -->  f = 8 M    : 122  hz
                    pre = 1024 -->  f = 7812.5 : 0.119 hz
    */
    
    // freq = F_CPU / (2 * Prescaler * top)
    if (!g_u8Timer1_flag)
    {
        // BUG: this freq will not be accurate as the min. resolution of the generator is 2 bits (0x0003) which yields 4 Mhz max freq and may get away with 8 Mhz

        if(freq >= (uint32_t)(F_CPU >> 1))
            {prescaler = 1; pre = 1;}
        else if (freq < (uint32_t)(F_CPU >> 1) && freq >= (uint32_t)((F_CPU >> 17)+1ul))
            {prescaler = 1; pre = 1;}
        else if (freq < (uint32_t)(F_CPU >> 4) && freq >= (uint32_t)((F_CPU >> 20)+1ul))
            {prescaler = 8; pre = 2;}
        else if (freq < (uint32_t)(F_CPU >> 7) && freq >= (uint32_t)((F_CPU >> 23)+1ul))
            {prescaler = 64; pre = 3;}
        // FIXME: requires float freq value to be able to generate freq lower than 1 hz
        else if (freq < (uint32_t)(F_CPU >> 9) && freq >= (uint32_t)1)
            {prescaler = 256; pre = 4;}
        else if (freq < 1ul)
            {prescaler = 1024; pre = 5;}


        // TODO: change the software division to something else for more performance at initalization time
        #if defined( __PWM_USE_ICR1)
            ICR1 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / (2 * prescaler));
        #elif defined (__PWM_USE_OCR1A)
            OCR1A = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / (2 * prescaler));
        #endif

        // set the prescaler in the register
        // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
        //        that disabled me from using both OC1A & OC1B atm..
        //        now i can use them both but they must have the same freq
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<4) | pre;
            
            #if defined(__PWM_USE_ICR1)
                TCCR1A &= ~(1<<0);
            #elif defined(__PWM_USE_OCR1A)
                TCCR1A |= (1<<0);
            #endif
        #endif

        // oc1a must be set output
        PWM1DDR |= (1<<PWM1BPIN);
        TCNT1H = 0;
        TCNT1L = 0;
        // set this global flag to indicate oc1a is running with a specifi prescaler
        g_u8Timer1_flag = 1;
    }
    else
    {
        // do nothing as it will mess with oc1b output

        // set the prescaler in the register
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<4);
        #endif
        // 0c1a must be set output
        PWM1DDR |= (1<<PWM1BPIN);
    }
}

// NOTE: replaced float duty cycle used for accuracy into an int but removed " - 1" to get the same result i get from float calculations
void phasePWM1B_duty(uint8_t duty)
{
    // duty cycle = OCR1x / top
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR1A |= (1<<5);

    #if defined(__PWM_USE_ICR1)
        OCR1B = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * ICR1) * DIV100_16) >> 16);
    #elif defined(__PWM_USE_OCR1A)
        OCR1B = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * OCR1A) * DIV100_16) >> 16);
    #endif
}

void phasePWM1B_stop(void)
{
    // notice this way you stop timer 0 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR0 = 0;

    OCR1B = 0;
    TCNT1 = 0;
    // oc1b pin diconnected, normal port operation
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
        TCCR1A &= ~(1<<5);
        TCCR1A &= ~(1<<4);
    #endif
    g_u8Timer1_flag = 0;
}

/*********************************************************************************/

// dont operate it with oc1a or oc1b
// dont operate it with oc2 as they share the same pin
#if defined(__AVR_ATmega128__)
// OPTION: make the freq float
void phasePWM1C_init(uint32_t freq)
{
    // control OC1C only
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    /* freq can be
            top = 0xff  --> f = 31372.5 : 30.6 hz       with specific values depending on pre
            top = 0x1ff --> f = 15655.57 : 15.3 hz  with specific values depending on pre
            top = 0x3ff --> f = 7820.13 : 7.6 hz    with specific values depending on pre
            top = ICR1 || OCR1A
                    pre = 1    -->  f = 8 M    : 122  hz
                    pre = 1024 -->  f = 7812.5 : 0.119 hz
    */
    
    // freq = F_CPU / (2 * Prescaler * top)
    if (!g_u8Timer1_flag)
    {
        // BUG: this freq will not be accurate as the min. resolution of the generator is 2 bits (0x0003) which yields 4 Mhz max freq and may get away with 8 Mhz
        if(freq >= (uint32_t)(F_CPU >> 1))
            {prescaler = 1; pre = 1;}
        else if (freq < (uint32_t)(F_CPU >> 1) && freq >= (uint32_t)((F_CPU >> 17)+1ul))
            {prescaler = 1; pre = 1;}
        else if (freq < (uint32_t)(F_CPU >> 4) && freq >= (uint32_t)((F_CPU >> 20)+1ul))
            {prescaler = 8; pre = 2;}
        else if (freq < (uint32_t)(F_CPU >> 7) && freq >= (uint32_t)((F_CPU >> 23)+1ul))
            {prescaler = 64; pre = 3;}
        // FIXME: requires float freq value to be able to generate freq lower than 1 hz
        else if (freq < (uint32_t)(F_CPU >> 9) && freq >= (uint32_t)1)
            {prescaler = 256; pre = 4;}
        else if (freq < 1ul)
            {prescaler = 1024; pre = 5;}

        // TODO: change the software division to something else for more performance at initalization time
        #if defined( __PWM_USE_ICR1)
            ICR1 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / (2 * prescaler));
        #elif defined (__PWM_USE_OCR1A)
            OCR1A = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / (2 * prescaler));
        #endif
        
        // set the prescaler in the register & mode 14
        // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
        //        that disabled me from using both OC1A & OC1B atm..
        //        now i can use them both but they must have the same freq
        TCCR1A |= (1<<1) | (1<<3);
        TCCR1B |= (1<<4) | pre;
        
        #if defined(__PWM_USE_ICR1)
            TCCR1A &= ~(1<<0);
        #elif defined(__PWM_USE_OCR1A)
            TCCR1A |= (1<<0);
        #endif

        // oc1a must be set output
        PWM1DDR |= (1<<PWM1CPIN);
        TCNT1H = 0;
        TCNT1L = 0;
        // set this global flag to indicate oc1a is running with a specifi prescaler
        g_u8Timer1_flag = 1;
    }
    else if (g_u8Timer1_flag && !g_u8Timer2_flag)
    {
        // do nothing as it will mess with oc1b output

        // set the prescaler in the register
        TCCR1A |= (1<<1) | (1<<3);
        TCCR1B |= (1<<4);
        // 0c1c must be set output
        PWM1DDR |= (1<<PWM1CPIN);
    }
    else
    {
        // they share same pin dont do anything
    }
}

// NOTE: replaced float duty cycle used for accuracy into an int but removed " - 1" to get the same result i get from float calculations
void phasePWM1C_duty(uint8_t duty)
{
    // duty cycle = OCR1x / top
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR1A |= (1<<3);

    #if defined(__PWM_USE_ICR1)
        OCR1C = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * ICR1) * DIV100_16) >> 16);
    #elif defined(__PWM_USE_OCR1A)
        OCR1C = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * OCR1A) * DIV100_16) >> 16);
    #endif
}

void phasePWM1C_stop(void)
{
    // notice this way you stop timer 0 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR0 = 0;

    OCR1C = 0;
    TCNT1 = 0;
    // oc1c pin diconnected, normal port operation
    TCCR1A &= ~(1<<3);
    TCCR1A &= ~(1<<2);
    
    g_u8Timer1_flag = 0;
    g_u8Timer2_flag = 0;
}

/*******************************************************************************/

// timer 3
// OC3A

// OPTION: make the freq float
void phasePWM3A_init(uint32_t freq)
{
    // control OC3A only
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    uint8_t pre = 0;
    
    /* freq can be
            top = 0xff  --> f = 31372.5 : 30.6 hz       with specific values depending on pre
            top = 0x1ff --> f = 15655.57 : 15.3 hz  with specific values depending on pre
            top = 0x3ff --> f = 7820.13 : 7.6 hz    with specific values depending on pre
            top = ICR3 || OCR3A
                    pre = 1    -->  f = 8 M    : 122  hz
                    pre = 1024 -->  f = 7812.5 : 0.119 hz
    */
    
    // freq = F_CPU / (2 * Prescaler * top)
    if (!g_u8Timer3_flag)
    {
        // BUG: this freq will not be accurate as the min. resolution of the generator is 2 bits (0x0003) which yields 4 Mhz max freq and may get away with 8 Mhz

        if(freq >= (uint32_t)(F_CPU >> 1))
            {prescaler = 1; pre = 1;}
        else if (freq < (uint32_t)(F_CPU >> 1) && freq >= (uint32_t)((F_CPU >> 17)+1ul))
            {prescaler = 1; pre = 1;}
        else if (freq < (uint32_t)(F_CPU >> 4) && freq >= (uint32_t)((F_CPU >> 20)+1ul))
            {prescaler = 8; pre = 2;}
        else if (freq < (uint32_t)(F_CPU >> 7) && freq >= (uint32_t)((F_CPU >> 23)+1ul))
            {prescaler = 64; pre = 3;}
        // FIXME: requires float freq value to be able to generate freq lower than 1 hz
        else if (freq < (uint32_t)(F_CPU >> 9) && freq >= (uint32_t)1)
            {prescaler = 256; pre = 4;}
        else if (freq < 1ul)
            {prescaler = 1024; pre = 5;}


        // TODO: change the software division to something else for more performance at initalization time
        #if defined( __PWM_USE_ICR3) || defined (__PWM_USE_OCR3A)
            ICR3 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / (2 * prescaler));
        #elif defined (__PWM_USE_OCR3A)
            // cant be used to define top value here as it is used to define duty cycle
            //OCR3A = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
        #endif

        // set the prescaler in the register
        // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
        //        that disabled me from using both OC1A & OC1B atm..
        //        now i can use them both but they must have the same freq

        TCCR3A |= (1<<1) | (1<<7);
        TCCR3B |= (1<<4) | pre;
        
        #if defined(__PWM_USE_ICR3) || defined(__PWM_USE_OCR3A)
            TCCR3A &= ~(1<<0);
        #elif defined(__PWM_USE_OCR3A)
            // cant be used with this generator as OCR3A defines top value already
            //TCCR3A |= (1<<0);
        #endif

        // oc3a must be set output
        PWM3DDR |= (1<<PWM3APIN);
        TCNT3H = 0;
        TCNT3L = 0;
        // set this global flag to indicate oc3a is running with a specific prescaler
        g_u8Timer3_flag = 1;
    }
    else
    {
        // do nothing as it will mess with oc3b & c output

        // set the prescaler in the register
        TCCR3A |= (1<<1) | (1<<7);
        TCCR3B |= (1<<4);
        // oc3a must be set output
        PWM3DDR |= (1<<PWM3APIN);
    }
}

// NOTE: replaced float duty cycle used for accuracy into an int but removed " - 1" to get the same result i get from float calculations
void phasePWM3A_duty(uint8_t duty)
{
    // duty cycle = OCR3x / top
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR3A |= (1<<7);
    
    OCR3A = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * ICR3) * DIV100_16) >> 16);
}

void phasePWM3A_stop(void)
{
    // notice this way you stop timer 3 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR3 = 0;

    // this way you dont stop the timer so you wont need to reinit
    OCR3A = 0;
    TCNT3 = 0;
    // oc3A pin diconnected, normal port operation)
    TCCR3A &= ~(1<<7);
    TCCR3A &= ~(1<<6);
    g_u8Timer3_flag = 0;
}

/**************************************************************************/
// timer 3
// OC3B

// OPTION: make the freq float
void phasePWM3B_init(uint32_t freq)
{
    // control OC3B only
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    uint8_t pre = 0;
    
    /* freq can be
            top = 0xff  --> f = 31372.5 : 30.6 hz       with specific values depending on pre
            top = 0x1ff --> f = 15655.57 : 15.3 hz  with specific values depending on pre
            top = 0x3ff --> f = 7820.13 : 7.6 hz    with specific values depending on pre
            top = ICR3 || OCR3A
                    pre = 1    -->  f = 8 M    : 122  hz
                    pre = 1024 -->  f = 7812.5 : 0.119 hz
    */
    
    // freq = F_CPU / (2 * Prescaler * top)
    if (!g_u8Timer3_flag)
    {
        // BUG: this freq will not be accurate as the min. resolution of the generator is 2 bits (0x0003) which yields 4 Mhz max freq and may get away with 8 Mhz

        if(freq >= (uint32_t)(F_CPU >> 1))
            {prescaler = 1; pre = 1;}
        else if (freq < (uint32_t)(F_CPU >> 1) && freq >= (uint32_t)((F_CPU >> 17)+1ul))
            {prescaler = 1; pre = 1;}
        else if (freq < (uint32_t)(F_CPU >> 4) && freq >= (uint32_t)((F_CPU >> 20)+1ul))
            {prescaler = 8; pre = 2;}
        else if (freq < (uint32_t)(F_CPU >> 7) && freq >= (uint32_t)((F_CPU >> 23)+1ul))
            {prescaler = 64; pre = 3;}
        // FIXME: requires float freq value to be able to generate freq lower than 1 hz
        else if (freq < (uint32_t)(F_CPU >> 9) && freq >= (uint32_t)1)
            {prescaler = 256; pre = 4;}
        else if (freq < 1ul)
            {prescaler = 1024; pre = 5;}


        // TODO: change the software division to something else for more performance at initalization time
        #if defined( __PWM_USE_ICR3)
            ICR3 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / (2 * prescaler));
        #elif defined (__PWM_USE_OCR3A)
            OCR3A = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / (2 * prescaler));
        #endif

        // set the prescaler in the register
        // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
        //        that disabled me from using both OC1A & OC1B atm..
        //        now i can use them both but they must have the same freq

        TCCR3A |= (1<<1) | (1<<5);
        TCCR3B |= (1<<4) | pre;
        
        #if defined(__PWM_USE_ICR3)
            TCCR3A &= ~(1<<0);
        #elif defined(__PWM_USE_OCR3A)
            TCCR3A |= (1<<0);
        #endif

        // oc3b must be set output
        PWM3DDR |= (1<<PWM3BPIN);
        TCNT3H = 0;
        TCNT3L = 0;
        // set this global flag to indicate oc1a is running with a specifi prescaler
        g_u8Timer3_flag = 1;
    }
    else
    {
        // do nothing as it will mess with oc1b output

        // set the prescaler in the register
        TCCR3A |= (1<<1) | (1<<5);
        TCCR3B |= (1<<4);
        // 0c3b must be set output
        PWM3DDR |= (1<<PWM3BPIN);
    }
}

void phasePWM3B_duty(uint8_t duty)
{
    // duty cycle = OCR3x / top
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR3A |= (1<<5);

    #if defined(__PWM_USE_ICR3)
        OCR3B = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * ICR3) * DIV100_16) >> 16);
    #elif defined(__PWM_USE_OCR3A)
        OCR3B = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * OCR3A) * DIV100_16) >> 16);
    #endif
}

void phasePWM3B_stop(void)
{
    // notice this way you stop timer 3 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR3 = 0;

    OCR3B = 0;
    TCNT3 = 0;
    // oc3b pin diconnected, normal port operation
    TCCR3A &= ~(1<<5);
    TCCR3A &= ~(1<<4);
    g_u8Timer3_flag = 0;
}

/*********************************************************************************/

// OC3C
// OPTION: make the freq float
void phasePWM3C_init(uint32_t freq)
{
    // control OC3C only
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    /* freq can be
            top = 0xff  --> f = 31372.5 : 30.6 hz       with specific values depending on pre
            top = 0x1ff --> f = 15655.57 : 15.3 hz  with specific values depending on pre
            top = 0x3ff --> f = 7820.13 : 7.6 hz    with specific values depending on pre
            top = ICR3 || OCR3A
                    pre = 1    -->  f = 8 M    : 122  hz
                    pre = 1024 -->  f = 7812.5 : 0.119 hz
    */
    
    // freq = F_CPU / (2 * Prescaler * top)
    if (!g_u8Timer3_flag)
    {
        // BUG: this freq will not be accurate as the min. resolution of the generator is 2 bits (0x0003) which yields 4 Mhz max freq and may get away with 8 Mhz
        if(freq >= (uint32_t)(F_CPU >> 1))
            {prescaler = 1; pre = 1;}
        else if (freq < (uint32_t)(F_CPU >> 1) && freq >= (uint32_t)((F_CPU >> 17)+1ul))
            {prescaler = 1; pre = 1;}
        else if (freq < (uint32_t)(F_CPU >> 4) && freq >= (uint32_t)((F_CPU >> 20)+1ul))
            {prescaler = 8; pre = 2;}
        else if (freq < (uint32_t)(F_CPU >> 7) && freq >= (uint32_t)((F_CPU >> 23)+1ul))
            {prescaler = 64; pre = 3;}
        // FIXME: requires float freq value to be able to generate freq lower than 1 hz
        else if (freq < (uint32_t)(F_CPU >> 9) && freq >= (uint32_t)1)
            {prescaler = 256; pre = 4;}
        else if (freq < 1ul)
            {prescaler = 1024; pre = 5;}

        // TODO: change the software division to something else for more performance at initalization time
        #if defined( __PWM_USE_ICR3)
            ICR3 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / (2 * prescaler));
        #elif defined (__PWM_USE_OCR3A)
            OCR3A = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / (2 * prescaler));
        #endif
        
        // set the prescaler in the register & mode 14
        // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
        //        that disabled me from using both OC1A & OC1B atm..
        //        now i can use them both but they must have the same freq
        TCCR3A |= (1<<1) | (1<<3);
        TCCR3B |= (1<<4) | pre;
        
        #if defined(__PWM_USE_ICR3)
            TCCR3A &= ~(1<<0);
        #elif defined(__PWM_USE_OCR3A)
            TCCR3A |= (1<<0);
        #endif

        // oc3c must be set output
        PWM3DDR |= (1<<PWM3CPIN);
        TCNT3H = 0;
        TCNT3L = 0;
        // set this global flag to indicate oc1a is running with a specifi prescaler
        g_u8Timer3_flag = 1;
    }
    else
    {
        // do nothing as it will mess with oc1b output

        // set the prescaler in the register
        TCCR3A |= (1<<1) | (1<<3);
        TCCR3B |= (1<<4);
        // oc3c must be set output
        PWM3DDR |= (1<<PWM3CPIN);
    }
}

// NOTE: replaced float duty cycle used for accuracy into an int but removed " - 1" to get the same result i get from float calculations
void phasePWM3C_duty(uint8_t duty)
{
    // duty cycle = OCR3x / top
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR3A |= (1<<3);

    #if defined(__PWM_USE_ICR3)
        OCR3C = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * ICR3) * DIV100_16) >> 16);
    #elif defined(__PWM_USE_OCR3A)
        OCR3C = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * OCR3A) * DIV100_16) >> 16);
    #endif
}

void phasePWM3C_stop(void)
{
    // notice this way you stop timer 0 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR3 = 0;

    OCR3C = 0;
    TCNT3 = 0;
    // oc1c pin diconnected, normal port operation
    TCCR3A &= ~(1<<3);
    TCCR3A &= ~(1<<2);
    
    g_u8Timer3_flag = 0;
}

#endif
