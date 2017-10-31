#include "fastPWM.h"

static uint8_t g_u8Timer1_flag = 0;
static uint8_t g_u8Timer2_flag = 0;

#if defined(__AVR_ATmega128__)
    static uint8_t g_u8Timer3_flag = 0;
#endif


// timer 0
void fastPWM0_init(uint32_t freq)
{
    // this routine works with non-inverted mode only

    uint8_t pre = 0;

    // freq = F_CPU / (256*Prescaler)
    // for default value set the freq argument with the value 0 [61 kHz]
    // freq is limited to a specific set of values depending on the chosen prescaler
    // these next values depend on crystal = 16 Mhz and will be different if another crystal is being used
    // freq = [62500 , 7812.5 , 976.5 , 244.14 , 61]  for atmega32/16
    // freq = [62500 , 7812.5 , 1953 , 976.5 , 488 , 244.14 , 61]  for atmega128
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
    
    // NOTE : should make TCCR0 = not with | as changing the freq would cause error
    // set the prescaler in the register
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
        TCCR0 = (1<<3) | (1<<5) | (1<<6) | pre;
    #endif

    // 0c0 must be set output
    PWM0DDR |= (1<<PWM0PIN);
    TCNT0 = 0;
}

void fastPWM0_duty(uint8_t duty)
{
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR0 |= (1<<5);

    // else uses a way to do division without using software division method
    if (duty <= 0)   fastPWM0_stop();
    else if (duty >= 100) OCR0 = 255;
    // next lose " - 1" in the end as without it we get better results due to truncation
    else  OCR0 = (uint8_t)((uint32_t)((uint32_t)((uint16_t)duty * 256) * DIV100_16) >> 16);
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
        
        // NOTE : should make TCCR0 = not with | as changing the freq would cause error
        // set the prescaler in the register
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
            TCCR2 = (1<<3) | (1<<5) | (1<<6) | pre;
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

void fastPWM2_duty(uint8_t duty)
{
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR2 |= (1<<5);
    
    // else uses a way to do division without using software division method
    if (duty <= 0)   fastPWM2_stop();
    else if (duty >= 100) OCR2 = 255;
    // next lose " - 1" in the end as without it we get better results due to truncation
    else  OCR2 = (uint8_t)((uint32_t)((uint32_t)((uint16_t)duty * 256) * DIV100_16) >> 16);
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
    g_u8Timer2_flag = 0; 
}

/************************************************************************/
// timer 1
// OC1A

// OPTION : make the freq float
void fastPWM1A_init(uint32_t freq)
{
    // control OC1A only
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    /* freq can be
            top = 0xff  --> f = 62500 : 61 hz   with specific values depending on pre
            top = 0x1ff --> f = 31250 : 30 hz   with specific values depending on pre
            top = 0x3ff --> f = 15625 : 15 hz   with specific values depending on pre
            top = ICR1 || OCR1A
                    pre = 1    -->  f = 16 M  : 244  hz
                    pre = 1024 -->  f = 15625 : 0.23 hz
    */
    // freq = F_CPU / ((top + 1)*Prescaler)
    if (!g_u8Timer1_flag)
    {
        // BUG : this freq will not be accurate as the min. resolution of the generator is 2 bits (0x0003) which yields 4 Mhz max freq and may get away with 8 Mhz
        if(freq >= (uint32_t)F_CPU)
            {prescaler = 1; pre = 1;}
        if(freq >= (uint32_t)(F_CPU >> 16) && freq < F_CPU)
            {prescaler = 1; pre = 1;}
        else if(freq >= (uint32_t)(F_CPU >> 19) && freq <= (uint32_t)(F_CPU >> 3))
            {prescaler = 8; pre = 2;}  
        else if(freq >= (uint32_t)(F_CPU >> 22) && freq <= (uint32_t)(F_CPU >> 6))
            {prescaler = 64; pre = 3;}
        else if(freq >= 1ul && freq <= (uint32_t)(F_CPU >> 8))
            {prescaler = 256; pre = 4;} 
        else if(freq < 1)
            {prescaler = 1024; pre = 5;} 

        //TODO : change the software division to something else for more performance at initalization time
        #if defined( __PWM_USE_ICR1) || defined (__PWM_USE_OCR1A)
            ICR1 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
        #elif defined (__PWM_USE_OCR1A)
            // cant be used to define top value here as it is used to define duty cycle
            //OCR1A = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
        #endif
        
        // set the prescaler in the register & mode 14
        // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
        //        that disabled me from using both OC1A & OC1B atm..
        //        now i can use them both but they must have the same freq
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<3) | (1<<4) | pre;
            
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
            TCCR1B |= (1<<3) | (1<<4);
        #endif
        // 0c1a must be set output
        PWM1DDR |= (1<<PWM1APIN);
    }
}

// NOTE : replaced float duty cycle used for accuracy into an int but removed " - 1" to get the same result i get from float calculations
void fastPWM1A_duty(uint8_t duty)
{
    // duty cycle = (OCR1x + 1) / (top + 1)
    // so when pwmstop occurs it can call back pwm function without init again
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
        TCCR1A |= (1<<7);
    #endif
    
    // else uses a way to do division without using software division method
    if (duty <= 0)   fastPWM1A_stop();
    else if (duty >= 100) OCR1A = ICR1;
    // next lose " - 1" in the end as without it we get better results due to truncation
    else  OCR1A = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * ICR1+1) * DIV100_16) >> 16);
}

void fastPWM1A_stop(void)
{
    // notice this way you stop timer 1 from all opertaions and when you come back you have to reinitialize the timer again
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

// OPTION : make the freq float
void fastPWM1B_init(uint32_t freq)
{
    // control OC1B only
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    /* freq can be
            top = 0xff  --> f = 62500 : 61 hz   with specific values depending on pre
            top = 0x1ff --> f = 31250 : 30 hz   with specific values depending on pre
            top = 0x3ff --> f = 15625 : 15 hz   with specific values depending on pre
            top = ICR1 || OCR1A
                    pre = 1    -->  f = 16 M  : 244  hz
                    pre = 1024 -->  f = 15625 : 0.23 hz
    */
    // freq = F_CPU / ((top + 1)*Prescaler)
    if (!g_u8Timer1_flag)
    {
        // BUG : this freq will not be accurate as the min. resolution of the generator is 2 bits (0x0003) which yields 4 Mhz max freq and may get away with 8 Mhz
        if(freq >= (uint32_t)F_CPU)
            {prescaler = 1; pre = 1;}
        if(freq >= (uint32_t)(F_CPU >> 16) && freq < F_CPU)
            {prescaler = 1; pre = 1;}
        else if(freq >= (uint32_t)(F_CPU >> 19) && freq <= (uint32_t)(F_CPU >> 3))
            {prescaler = 8; pre = 2;}  
        else if(freq >= (uint32_t)(F_CPU >> 22) && freq <= (uint32_t)(F_CPU >> 6))
            {prescaler = 64; pre = 3;}
        else if(freq >= 1ul && freq <= (uint32_t)(F_CPU >> 8))
            {prescaler = 256; pre = 4;} 
        else if(freq < 1)
            {prescaler = 1024; pre = 5;} 

        //TODO : change the software division to something else for more performance at initalization time
        #if defined( __PWM_USE_ICR1)
            ICR1 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
        #elif defined (__PWM_USE_OCR1A)
            OCR1A = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
        #endif
        
        // set the prescaler in the register & mode 14
        // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
        //        that disabled me from using both OC1A & OC1B atm..
        //        now i can use them both but they must have the same freq
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<3) | (1<<4) | pre;
            
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
            TCCR1B |= (1<<3) | (1<<4);
        #endif
        // 0c1a must be set output
        PWM1DDR |= (1<<PWM1BPIN);
    }
}

// NOTE : replaced float duty cycle used for accuracy into an int but removed " - 1" to get the same result i get from float calculations
void fastPWM1B_duty(uint8_t duty)
{
    // duty cycle = (OCR1x + 1) / (top + 1)
    // so when pwmstop occurs it can call back pwm function without init again
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
        TCCR1A |= (1<<5);
    #endif
    
    // else uses a way to do division without using software division method
    if (duty <= 0)   fastPWM1B_stop();
    else if (duty >= 100)
    {
        #if defined(__PWM_USE_ICR1)
            OCR1B = ICR1;
        #elif defined(__PWM_USE_OCR1A)
            OCR1B = OCR1A;
        #endif
    } 
    // next lose " - 1" in the end as without it we get better results due to truncation
    else
    {
        #if defined(__PWM_USE_ICR1)
            OCR1B = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * ICR1+1) * DIV100_16) >> 16);
        #elif defined(__PWM_USE_OCR1A)
            OCR1B = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * OCR1A+1) * DIV100_16) >> 16);
        #endif
    }  
}

void fastPWM1B_stop(void)
{
    // notice this way you stop timer 1 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR0 = 0;

    // this way you dont stop the timer so you wont need to reinit
    OCR1B = 0;
    TCNT1 = 0;
    // oc1A pin diconnected, normal port operation
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
// OPTION : make the freq float
void fastPWM1C_init(uint32_t freq)
{
    // control OC1C only
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    /* freq can be
            top = 0xff  --> f = 62500 : 61 hz   with specific values depending on pre
            top = 0x1ff --> f = 31250 : 30 hz   with specific values depending on pre
            top = 0x3ff --> f = 15625 : 15 hz   with specific values depending on pre
            top = ICR1 || OCR1A
                    pre = 1    -->  f = 16 M  : 244  hz
                    pre = 1024 -->  f = 15625 : 0.23 hz
    */
    // freq = F_CPU / ((top + 1)*Prescaler)
    if (!g_u8Timer1_flag)
    {
        // BUG : this freq will not be accurate as the min. resolution of the generator is 2 bits (0x0003) which yields 4 Mhz max freq and may get away with 8 Mhz
        if(freq >= (uint32_t)F_CPU)
            {prescaler = 1; pre = 1;}
        if(freq >= (uint32_t)(F_CPU >> 16) && freq < F_CPU)
            {prescaler = 1; pre = 1;}
        else if(freq >= (uint32_t)(F_CPU >> 19) && freq <= (uint32_t)(F_CPU >> 3))
            {prescaler = 8; pre = 2;}  
        else if(freq >= (uint32_t)(F_CPU >> 22) && freq <= (uint32_t)(F_CPU >> 6))
            {prescaler = 64; pre = 3;}
        else if(freq >= 1ul && freq <= (uint32_t)(F_CPU >> 8))
            {prescaler = 256; pre = 4;} 
        else if(freq < 1)
            {prescaler = 1024; pre = 5;} 

        //TODO : change the software division to something else for more performance at initalization time
        #if defined( __PWM_USE_ICR1)
            ICR1 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
        #elif defined (__PWM_USE_OCR1A)
            OCR1A = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
        #endif
        
        // set the prescaler in the register & mode 14
        // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
        //        that disabled me from using both OC1A & OC1B atm..
        //        now i can use them both but they must have the same freq
        TCCR1A |= (1<<1) | (1<<3);
        TCCR1B |= (1<<3) | (1<<4) | pre;
        
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
        TCCR1B |= (1<<3) | (1<<4);
        // 0c1a must be set output
        PWM1DDR |= (1<<PWM1CPIN);
    }
    else
    {
        // they share same pin dont do anything
    }
}

// NOTE : replaced float duty cycle used for accuracy into an int but removed " - 1" to get the same result i get from float calculations
void fastPWM1C_duty(uint8_t duty)
{
    // duty cycle = (OCR1x + 1) / (top + 1)
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR1A |= (1<<3);
    
    // else uses a way to do division without using software division method
    if (duty <= 0)   fastPWM1C_stop();
    else if (duty >= 100)
    {
        #if defined(__PWM_USE_ICR1)
            OCR1C = ICR1;
        #elif defined(__PWM_USE_OCR1A)
            OCR1C = OCR1A;
        #endif
    } 
    // next lose " - 1" in the end as without it we get better results due to truncation
    else
    {
        #if defined(__PWM_USE_ICR1)
            OCR1C = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * ICR1+1) * DIV100_16) >> 16);
        #elif defined(__PWM_USE_OCR1A)
            OCR1C = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * OCR1A+1) * DIV100_16) >> 16);
        #endif
    }  
}

void fastPWM1C_stop(void)
{
    // notice this way you stop timer 1 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR0 = 0;

    // this way you dont stop the timer so you wont need to reinit
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

// OPTION : make the freq float
void fastPWM3A_init(uint32_t freq)
{
    // control OC3A only
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    /* freq can be
            top = 0xff  --> f = 62500 : 61 hz   with specific values depending on pre
            top = 0x1ff --> f = 31250 : 30 hz   with specific values depending on pre
            top = 0x3ff --> f = 15625 : 15 hz   with specific values depending on pre
            top = ICR1 || OCR1A
                    pre = 1    -->  f = 16 M  : 244  hz
                    pre = 1024 -->  f = 15625 : 0.23 hz
    */
    // freq = F_CPU / ((top + 1)*Prescaler)
    if (!g_u8Timer3_flag)
    {
        // BUG : this freq will not be accurate as the min. resolution of the generator is 2 bits (0x0003) which yields 4 Mhz max freq and may get away with 8 Mhz
        if(freq >= (uint32_t)F_CPU)
            {prescaler = 1; pre = 1;}
        if(freq >= (uint32_t)(F_CPU >> 16) && freq < F_CPU)
            {prescaler = 1; pre = 1;}
        else if(freq >= (uint32_t)(F_CPU >> 19) && freq <= (uint32_t)(F_CPU >> 3))
            {prescaler = 8; pre = 2;}  
        else if(freq >= (uint32_t)(F_CPU >> 22) && freq <= (uint32_t)(F_CPU >> 6))
            {prescaler = 64; pre = 3;}
        else if(freq >= 1ul && freq <= (uint32_t)(F_CPU >> 8))
            {prescaler = 256; pre = 4;} 
        else if(freq < 1)
            {prescaler = 1024; pre = 5;} 

        //TODO : change the software division to something else for more performance at initalization time
        #if defined( __PWM_USE_ICR3) || defined (__PWM_USE_OCR3A)
            ICR3 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
        #elif defined (__PWM_USE_OCR3A)
            // cant be used to define top value here as it is used to define duty cycle
            //OCR3A = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
        #endif
        
        // set the prescaler in the register & mode 14
        // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
        //        that disabled me from using both OC1A & OC1B atm..
        //        now i can use them both but they must have the same freq
        TCCR3A |= (1<<1) | (1<<7);
        TCCR3B |= (1<<3) | (1<<4) | pre;
        
        #if defined(__PWM_USE_ICR3) || defined(__PWM_USE_OCR3A)
            TCCR3A &= ~(1<<0);
        #elif defined(__PWM_USE_OCR3A)
            // cant be used with this generator as OCR3A defines top value already
            //TCCR3A |= (1<<0);
        #endif

        // oc1a must be set output
        PWM3DDR |= (1<<PWM3APIN);
        TCNT3H = 0;
        TCNT3L = 0;
        // set this global flag to indicate oc1a is running with a specifi prescaler
        g_u8Timer3_flag = 1;
    }
    else
    {
        // do nothing as it will mess with oc1b output

        // set the prescaler in the register
        TCCR3A |= (1<<1) | (1<<7);
        TCCR3B |= (1<<3) | (1<<4);
        // oc3a must be set output
        PWM3DDR |= (1<<PWM3APIN);
    }
}

// NOTE : replaced float duty cycle used for accuracy into an int but removed " - 1" to get the same result i get from float calculations
void fastPWM3A_duty(uint8_t duty)
{
    // duty cycle = (OCR3x + 1) / (top + 1)
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR3A |= (1<<7);
    
    // else uses a way to do division without using software division method
    if (duty <= 0)   fastPWM3A_stop();
    else if (duty >= 100) OCR3A = ICR3;
    // next lose " - 1" in the end as without it we get better results due to truncation
    else  OCR3A = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * ICR3+1) * DIV100_16) >> 16);
}

void fastPWM3A_stop(void)
{
    // notice this way you stop timer 1 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR3 = 0;

    // this way you dont stop the timer so you wont need to reinit
    OCR3A = 0;
    TCNT3 = 0;
    // oc3A pin diconnected, normal port operation
    TCCR1A &= ~(1<<7);
    TCCR3A &= ~(1<<6);
    g_u8Timer3_flag = 0;
}

/**************************************************************************/
// timer 3
// OC3B

// OPTION : make the freq float
void fastPWM3B_init(uint32_t freq)
{
    // control OC3B only
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    /* freq can be
            top = 0xff  --> f = 62500 : 61 hz   with specific values depending on pre
            top = 0x1ff --> f = 31250 : 30 hz   with specific values depending on pre
            top = 0x3ff --> f = 15625 : 15 hz   with specific values depending on pre
            top = ICR1 || OCR1A
                    pre = 1    -->  f = 16 M  : 244  hz
                    pre = 1024 -->  f = 15625 : 0.23 hz
    */
    // freq = F_CPU / ((top + 1)*Prescaler)
    if (!g_u8Timer3_flag)
    {
        // BUG : this freq will not be accurate as the min. resolution of the generator is 2 bits (0x0003) which yields 4 Mhz max freq and may get away with 8 Mhz
        if(freq >= (uint32_t)F_CPU)
            {prescaler = 1; pre = 1;}
        if(freq >= (uint32_t)(F_CPU >> 16) && freq < F_CPU)
            {prescaler = 1; pre = 1;}
        else if(freq >= (uint32_t)(F_CPU >> 19) && freq <= (uint32_t)(F_CPU >> 3))
            {prescaler = 8; pre = 2;}  
        else if(freq >= (uint32_t)(F_CPU >> 22) && freq <= (uint32_t)(F_CPU >> 6))
            {prescaler = 64; pre = 3;}
        else if(freq >= 1ul && freq <= (uint32_t)(F_CPU >> 8))
            {prescaler = 256; pre = 4;} 
        else if(freq < 1)
            {prescaler = 1024; pre = 5;} 

        //TODO : change the software division to something else for more performance at initalization time
        #if defined( __PWM_USE_ICR3)
            ICR3 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
        #elif defined (__PWM_USE_OCR3A)
            OCR3A = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
        #endif
        
        // set the prescaler in the register & mode 14
        // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
        //        that disabled me from using both OC1A & OC1B atm..
        //        now i can use them both but they must have the same freq
        TCCR3A |= (1<<1) | (1<<5);
        TCCR3B |= (1<<3) | (1<<4) | pre;
        
        #if defined(__PWM_USE_ICR3)
            TCCR3A &= ~(1<<0);
        #elif defined(__PWM_USE_OCR3A)
            TCCR3A |= (1<<0);
        #endif

        // oc3a must be set output
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
        TCCR3B |= (1<<3) | (1<<4);
        // oc3a must be set output
        PWM3DDR |= (1<<PWM3BPIN);
    }
}

// NOTE : replaced float duty cycle used for accuracy into an int but removed " - 1" to get the same result i get from float calculations
void fastPWM3B_duty(uint8_t duty)
{
    // duty cycle = (OCR3x + 1) / (top + 1)
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR3A |= (1<<5);
    
    // else uses a way to do division without using software division method
    if (duty <= 0)   fastPWM3B_stop();
    else if (duty >= 100)
    {
        #if defined(__PWM_USE_ICR3)
            OCR3B = ICR3;
        #elif defined(__PWM_USE_OCR3A)
            OCR3B = OCR3A;
        #endif
    } 
    // next lose " - 1" in the end as without it we get better results due to truncation
    else
    {
        #if defined(__PWM_USE_ICR3)
            OCR3B = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * ICR3+1) * DIV100_16) >> 16);
        #elif defined(__PWM_USE_OCR3A)
            OCR3B = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * OCR3A+1) * DIV100_16) >> 16);
        #endif
    }  
}

void fastPWM3B_stop(void)
{
    // notice this way you stop timer 1 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR3 = 0;

    // this way you dont stop the timer so you wont need to reinit
    OCR3B = 0;
    TCNT3 = 0;
    // oc1A pin diconnected, normal port operation
    TCCR3A &= ~(1<<5);
    TCCR3A &= ~(1<<4);
    g_u8Timer3_flag = 0;
}

/*********************************************************************************/

// timer 3
// OC3C

// dont operate it with oc1a or oc1b
// dont operate it with oc2 as they share the same pin
// OPTION : make the freq float
void fastPWM3C_init(uint32_t freq)
{
    // control OC1C only
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    /* freq can be
            top = 0xff  --> f = 62500 : 61 hz   with specific values depending on pre
            top = 0x1ff --> f = 31250 : 30 hz   with specific values depending on pre
            top = 0x3ff --> f = 15625 : 15 hz   with specific values depending on pre
            top = ICR1 || OCR1A
                    pre = 1    -->  f = 16 M  : 244  hz
                    pre = 1024 -->  f = 15625 : 0.23 hz
    */
    // freq = F_CPU / ((top + 1)*Prescaler)
    if (!g_u8Timer3_flag)
    {
        // BUG : this freq will not be accurate as the min. resolution of the generator is 2 bits (0x0003) which yields 4 Mhz max freq and may get away with 8 Mhz
        if(freq >= (uint32_t)F_CPU)
            {prescaler = 1; pre = 1;}
        if(freq >= (uint32_t)(F_CPU >> 16) && freq < F_CPU)
            {prescaler = 1; pre = 1;}
        else if(freq >= (uint32_t)(F_CPU >> 19) && freq <= (uint32_t)(F_CPU >> 3))
            {prescaler = 8; pre = 2;}  
        else if(freq >= (uint32_t)(F_CPU >> 22) && freq <= (uint32_t)(F_CPU >> 6))
            {prescaler = 64; pre = 3;}
        else if(freq >= 1ul && freq <= (uint32_t)(F_CPU >> 8))
            {prescaler = 256; pre = 4;} 
        else if(freq < 1)
            {prescaler = 1024; pre = 5;} 

        //TODO : change the software division to something else for more performance at initalization time
        #if defined( __PWM_USE_ICR3)
            ICR3 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
        #elif defined (__PWM_USE_OCR3A)
            OCR3A = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / prescaler) - 1;
        #endif
        
        // set the prescaler in the register & mode 14
        // edit : made it using bitwise instead of TCCR3A = (1<<1 | (1<<7)
        //        that disabled me from using both OC3A & OC3B atm..
        //        now i can use them both but they must have the same freq
        TCCR3A |= (1<<1) | (1<<3);
        TCCR3B |= (1<<3) | (1<<4) | pre;
        
        #if defined(__PWM_USE_ICR3)
            TCCR3A &= ~(1<<0);
        #elif defined(__PWM_USE_OCR3A)
            TCCR3A |= (1<<0);
        #endif

        // oc3a must be set output
        PWM3DDR |= (1<<PWM3CPIN);
        TCNT3H = 0;
        TCNT3L = 0;
        // set this global flag to indicate oc3a is running with a specifi prescaler
        g_u8Timer3_flag = 1;
    }
    else
    {
        // do nothing as it will mess with oc3b output

        // set the prescaler in the register
        TCCR3A |= (1<<1) | (1<<3);
        TCCR3B |= (1<<3) | (1<<4);
        // 0c1a must be set output
        PWM3DDR |= (1<<PWM3CPIN);
    }
}

// NOTE : replaced float duty cycle used for accuracy into an int but removed " - 1" to get the same result i get from float calculations
void fastPWM3C_duty(uint8_t duty)
{
    // duty cycle = (OCR3x + 1) / (top + 1)
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR3A |= (1<<3);
    
    // else uses a way to do division without using software division method
    if (duty <= 0)   fastPWM3C_stop();
    else if (duty >= 100)
    {
        #if defined(__PWM_USE_ICR3)
            OCR3C = ICR3;
        #elif defined(__PWM_USE_OCR3A)
            OCR3C = OCR3A;
        #endif
    } 
    // next lose " - 1" in the end as without it we get better results due to truncation
    else
    {
        #if defined(__PWM_USE_ICR3)
            OCR3C = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * ICR3+1) * DIV100_16) >> 16);
        #elif defined(__PWM_USE_OCR3A)
            OCR3C = (uint16_t)((uint32_t)((uint32_t)((uint32_t)duty * OCR3A+1) * DIV100_16) >> 16);
        #endif
    }  
}

void fastPWM3C_stop(void)
{
    // notice this way you stop timer 1 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR3 = 0;

    // this way you dont stop the timer so you wont need to reinit
    OCR3C = 0;
    TCNT3 = 0;
    // oc1c pin diconnected, normal port operation
    TCCR3A &= ~(1<<3);
    TCCR3A &= ~(1<<2);

    g_u8Timer3_flag = 0;
}


#endif
