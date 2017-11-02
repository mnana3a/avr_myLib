// NOTE: this driver has a flaw when the calculated prescaler with/without truncation is one of the acutal prescalers values

#include "wave_gen.h"

static uint8_t g_u8Timer1_flag = 0;
static uint8_t g_u8Timer2_flag = 0;

#if defined(__AVR_ATmega128__)
    static uint8_t g_u8Timer3_flag = 0;
#endif

    
void waveGenerator0_set(uint32_t freq)
{
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    // wave freq = FOSC / 2*(OCR0 + 1) * prescaler
    // least possible prescaler is better
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
        if(freq >= (uint32_t)(F_CPU >> 1))
            {prescaler = 1; pre = 1;}
        else if (freq < (uint32_t)(F_CPU >> 1) && freq >= (uint32_t)(F_CPU >> 9))
            {prescaler = 1; pre = 1;}
        else if (freq <= (uint32_t)(F_CPU >> 4) && freq >= (uint32_t)(F_CPU >> 12))
            {prescaler = 8; pre = 2;}
        else if (freq <= (uint32_t)(F_CPU >> 7) && freq >= (uint32_t)(F_CPU >> 15))
            {prescaler = 64; pre = 3;}
        else if (freq <= (uint32_t)(F_CPU >> 9) && freq >= (uint32_t)(F_CPU >> 17))
            {prescaler = 256; pre = 4;}
        else if (freq <= (uint32_t)(F_CPU >> 11) && freq >= (uint32_t)(F_CPU >> 19))
            {prescaler = 1024; pre = 5;}
            
    #elif defined(__AVR_ATmega128__)
        if(freq >= (uint32_t)(F_CPU >> 1))
            {prescaler = 1; pre = 1;}
        else if (freq < (uint32_t)(F_CPU >> 1) && freq >= (uint32_t)(F_CPU >> 9))
            {prescaler = 1; pre = 1;}
        else if (freq <= (uint32_t)(F_CPU >> 4) && freq >= (uint32_t)(F_CPU >> 12))
            {prescaler = 8; pre = 2;}
        else if (freq <= (uint32_t)(F_CPU >> 6) && freq >= (uint32_t)(F_CPU >> 14))
            {prescaler = 32; pre = 3;}
        else if (freq <= (uint32_t)(F_CPU >> 7) && freq >= (uint32_t)(F_CPU >> 15))
            {prescaler = 64; pre = 4;}
        else if (freq <= (uint32_t)(F_CPU >> 8) && freq >= (uint32_t)(F_CPU >> 16))
            {prescaler = 128; pre = 5;}
        else if (freq <= (uint32_t)(F_CPU >> 9) && freq >= (uint32_t)(F_CPU >> 17))
            {prescaler = 256; pre = 6;}
        else if (freq <= (uint32_t)(F_CPU >> 11) && freq >= (uint32_t)(F_CPU >> 19))
            {prescaler = 1024; pre = 7;}
    #endif
    
    // TODO: change the software division to something else for more performance at initalization time
    OCR0 = (F_CPU / (2ul * prescaler * freq)) - 1ul;
    // set the prescaler in the register
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
        TCCR0 = (1<<3) | (1<<4) | pre;
    #endif
    
    // 0c0 must be set output
    W0DDR |= (1<<W0PIN);
    TCNT0 = 0;
}

void waveGenerator0_stop(void)
{
    TCNT0 = 0;
    // oc2 disconnected and normal port operation
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
        TCCR0 &= ~(1<<4);       
        TCCR0 &= ~(1<<5);
    #endif
}

/*******************************************************************************/

void waveGenerator2_set(uint32_t freq)
{
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    // wave freq = FOSC / 2*(OCR2 + 1) * prescaler
    // least possible prescaler is better
    if (!g_u8Timer2_flag)
    {
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
            if(freq >= (uint32_t)(F_CPU >> 1))
            {prescaler = 1; pre = 1;}
            else if (freq < (uint32_t)(F_CPU >> 1) && freq >= (uint32_t)(F_CPU >> 9))
                {prescaler = 1; pre = 1;}
            else if (freq <= (uint32_t)(F_CPU >> 4) && freq >= (uint32_t)(F_CPU >> 12))
                {prescaler = 8; pre = 2;}
            else if (freq <= (uint32_t)(F_CPU >> 6) && freq >= (uint32_t)(F_CPU >> 14))
                {prescaler = 32; pre = 3;}
            else if (freq <= (uint32_t)(F_CPU >> 7) && freq >= (uint32_t)(F_CPU >> 15))
                {prescaler = 64; pre = 4;}
            else if (freq <= (uint32_t)(F_CPU >> 8) && freq >= (uint32_t)(F_CPU >> 16))
                {prescaler = 128; pre = 5;}
            else if (freq <= (uint32_t)(F_CPU >> 9) && freq >= (uint32_t)(F_CPU >> 17))
                {prescaler = 256; pre = 6;}
            else if (freq <= (uint32_t)(F_CPU >> 11) && freq >= (uint32_t)(F_CPU >> 19))
                {prescaler = 1024; pre = 7;}
            
        #elif defined(__AVR_ATmega128__)
            if(freq >= (uint32_t)(F_CPU >> 1))
            {prescaler = 1; pre = 1;}
            else if (freq < (uint32_t)(F_CPU >> 1) && freq >= (uint32_t)(F_CPU >> 9))
                {prescaler = 1; pre = 1;}
            else if (freq <= (uint32_t)(F_CPU >> 4) && freq >= (uint32_t)(F_CPU >> 12))
                {prescaler = 8; pre = 2;}
            else if (freq <= (uint32_t)(F_CPU >> 7) && freq >= (uint32_t)(F_CPU >> 15))
                {prescaler = 64; pre = 3;}
            else if (freq <= (uint32_t)(F_CPU >> 9) && freq >= (uint32_t)(F_CPU >> 17))
                {prescaler = 256; pre = 4;}
            else if (freq <= (uint32_t)(F_CPU >> 11) && freq >= (uint32_t)(F_CPU >> 19))
                {prescaler = 1024; pre = 5;}
            
        #endif

        // TODO: change the software division to something else for more performance at initalization time
        OCR2 = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        // set the prescaler in the register
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
            TCCR2 = (1<<3) | (1<<4) | pre;
        #endif
        
        // 0c2 must be set output
        W2DDR |= (1<<W2PIN);
        TCNT2 = 0;
        g_u8Timer2_flag = 1; 
    }
    else
    {
        // do nothing as atmega128 oc2 & oc1c share the same pin
    }
}

void waveGenerator2_stop(void)
{
    TCNT2 = 0;
    // oc2 disconnected and normal port operation
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
        TCCR2 &= ~(1<<4);       
        TCCR2 &= ~(1<<5);
    #endif
    g_u8Timer2_flag = 0; 
}

/*******************************************************************************/

void waveGenerator1A_set(uint32_t freq)
{
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    // wave freq = FOSC / 2*(OCR1A + 1) * prescaler
    // least possible prescaler is better

    // set the prescaler in the register
    // generator b left unchanged here as it maybe used in pwm mode rather than disabling it here
    // so |= is required here
    // mode 4 or 12 is used depending on the definition in the header file
    if (!g_u8Timer1_flag)
    {
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
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
        #endif

        // TODO: change the software division to something else for more performance at initalization time
        #if defined(__WAVE_USE_OCR1A)
            OCR1A = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #elif defined(__WAVE_USE_ICR1)
            ICR1 = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #endif
        
        // set the prescaler in the register
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
            TCCR1A |= (1<<6);
            TCCR1A &= ~(1<<0);
            TCCR1A &= ~(1<<1);
            #if defined(__WAVE_USE_OCR1A)
                TCCR1B = (1<<3) | pre;
            #elif defined(__WAVE_USE_ICR1)
                TCCR1B = (1<<3) | (1<<4) | pre;
            #endif
            
        #endif
        
        // 0c1a must be set output
        W1DDR |= (1<<W1APIN);
        TCNT1 = 0;
        // set this global flag to indicate oc1a is running with a specifi prescaler
        g_u8Timer1_flag = 1;
    }
    else
    {
        // do nothing as it will mess with oc1b output

        // set the prescaler in the register
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
            TCCR1A |= (1<<6);
            TCCR1A &= ~(1<<0);
            TCCR1A &= ~(1<<1);
        #endif
        // 0c3c must be set output
        W1DDR |= (1<<W1APIN);
    }
}

void waveGenerator1A_stop(void)
{
    TCNT1 = 0;
    // oc1a disconnected and normal port operation
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
        TCCR1A &= ~(1<<7);      
        TCCR1A &= ~(1<<6);
    #endif
    // reset the flag to enable oc1b to run with a different prescaler
    g_u8Timer1_flag = 0;
}

/*******************************************************************************/

void waveGenerator1B_set(uint32_t freq)
{
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    // wave freq = FOSC / 2*(OCR1A + 1) * prescaler
    // least possible prescaler is better

    // set the prescaler in the register
    // generator b left unchanged here as it maybe used in pwm mode rather than disabling it here
    // so |= is required here
    // mode 4 or 12 is used depending on the definition in the header file
    if (!g_u8Timer1_flag)
    {
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
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
        #endif

        // TODO: change the software division to something else for more performance at initalization time
        #if defined(__WAVE_USE_OCR1A)
            OCR1A = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #elif defined(__WAVE_USE_ICR1)
            ICR1 = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #endif
        
        // set the prescaler in the register
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
            TCCR1A |= (1<<4);
            TCCR1A &= ~(1<<0);
            TCCR1A &= ~(1<<1);
            #if defined(__WAVE_USE_OCR1A)
                TCCR1B = (1<<3) | pre;
            #elif defined(__WAVE_USE_ICR1)
                TCCR1B = (1<<3) | (1<<4) | pre;
            #endif
            
        #endif
        
        // 0c1a must be set output
        W1DDR |= (1<<W1BPIN);
        TCNT1 = 0;
        // set this global flag to indicate oc1a is running with a specifi prescaler
        g_u8Timer1_flag = 1;
    }
    else
    {
        // do nothing as it will mess with oc1a output
        // set the prescaler in the register
        #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
            TCCR1A |= (1<<4);
            TCCR1A &= ~(1<<0);
            TCCR1A &= ~(1<<1);
        #endif
        // 0c3c must be set output
        W1DDR |= (1<<W1BPIN);
    }
    
}

void waveGenerator1B_stop(void)
{
    TCNT1 = 0;
    // oc1b disconnected and normal port operation
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega128__)
        TCCR1A &= ~(1<<5);      
        TCCR1A &= ~(1<<4);
    #endif
    // reset the flag to indicate oc1a can use it
    g_u8Timer1_flag = 0;
}

/*******************************************************************************/

// dont operate it with oc1a or oc1b
// dont operate it with oc2 as they share the same pin
#if defined(__AVR_ATmega128__)
void waveGenerator1C_set(uint32_t freq)
{
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    // wave freq = FOSC / 2*(OCR1A + 1) * prescaler
    // least possible prescaler is better

    // set the prescaler in the register
    // generator b left unchanged here as it maybe used in pwm mode rather than disabling it here
    // so |= is required here
    // mode 4 or 12 is used depending on the definition in the header file
    if (!g_u8Timer1_flag && !g_u8Timer2_flag)
    {
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
        #if defined(__WAVE_USE_OCR1A)
            OCR1A = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #elif defined(__WAVE_USE_ICR1)
            ICR1 = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #endif
        
        // set the prescaler in the register
        TCCR1A |= (1<<2);
        TCCR1A &= ~(1<<0);
        TCCR1A &= ~(1<<1);
        #if defined(__WAVE_USE_OCR1A)
            TCCR1B = (1<<3) | pre;
        #elif defined(__WAVE_USE_ICR1)
            TCCR1B = (1<<3) | (1<<4) | pre;
        #endif
        
        // 0c1c must be set output
        W1DDR |= (1<<W1CPIN);
        TCNT1 = 0;
        // set this global flag to indicate oc1a is running with a specifi prescaler
        g_u8Timer1_flag = 1;
        g_u8Timer2_flag = 1;
    }
    else if (g_u8Timer1_flag && !g_u8Timer2_flag)
    {
        // do nothing as it will mess with oc1a output

        // set the prescaler in the register
        TCCR1A |= (1<<2);
        TCCR1A &= ~(1<<0);
        TCCR1A &= ~(1<<1);
        // 0c3c must be set output
        W1DDR |= (1<<W1CPIN);
    }
    else
    {
        // they share same pin dont do anything
    }
    
}

void waveGenerator1C_stop(void)
{
    TCNT1 = 0;
    // oc1b disconnected and normal port operation
    TCCR1A &= ~(1<<2);      
    TCCR1A &= ~(1<<3);
    // reset the flag to indicate oc1a/oc1b can use it
    g_u8Timer1_flag = 0;
    g_u8Timer2_flag = 0;
}

/*******************************************************************************/

void waveGenerator3A_set(uint32_t freq)
{
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    // wave freq = FOSC / 2*(OCR3A + 1) * prescaler
    // least possible prescaler is better

    // set the prescaler in the register
    // generator b left unchanged here as it maybe used in pwm mode rather than disabling it here
    // so |= is required here
    // mode 4 or 12 is used depending on the definition in the header file
    if (!g_u8Timer3_flag)
    {
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
        #if defined(__WAVE_USE_OCR3A)
            OCR3A = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #elif defined(__WAVE_USE_ICR3)
            ICR3 = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #endif
        
        // set the prescaler in the register
        TCCR3A |= (1<<6);
        TCCR3A &= ~(1<<0);
        TCCR3A &= ~(1<<1);
        #if defined(__WAVE_USE_OCR3A)
            TCCR3B = (1<<3) | pre;
        #elif defined(__WAVE_USE_ICR3)
            TCCR3B = (1<<3) | (1<<4) | pre;
        #endif
        
        // 0c1a must be set output
        W3DDR |= (1<<W3APIN);
        TCNT3 = 0;
        // set this global flag to indicate oc3a is running with a specifi prescaler
        g_u8Timer3_flag = 1;
    }
    else
    {
        // do nothing as it will mess with oc1b output

        // set the prescaler in the register
        TCCR3A |= (1<<6);
        TCCR3A &= ~(1<<0);
        TCCR3A &= ~(1<<1);
        // 0c3c must be set output
        W3DDR |= (1<<W3APIN);
    }
}

void waveGenerator3A_stop(void)
{
    TCNT3 = 0;
    // oc1a disconnected and normal port operation
    TCCR3A &= ~(1<<7);      
    TCCR3A &= ~(1<<6);
    // reset the flag to enable oc1b to run with a different prescaler
    g_u8Timer3_flag = 0;
}

/*******************************************************************************/

void waveGenerator3B_set(uint32_t freq)
{
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    // wave freq = FOSC / 2*(OCR3A + 1) * prescaler
    // least possible prescaler is better

    // set the prescaler in the register
    // generator b left unchanged here as it maybe used in pwm mode rather than disabling it here
    // so |= is required here
    // mode 4 or 12 is used depending on the definition in the header file
    if (!g_u8Timer3_flag)
    {
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
        #if defined(__WAVE_USE_OCR3A)
            OCR3A = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #elif defined(__WAVE_USE_ICR3)
            ICR3 = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #endif
        
        // set the prescaler in the register
        TCCR3A |= (1<<4);
        TCCR3A &= ~(1<<0);
        TCCR3A &= ~(1<<1);
        #if defined(__WAVE_USE_OCR3A)
            TCCR3B = (1<<3) | pre;
        #elif defined(__WAVE_USE_ICR3)
            TCCR3B = (1<<3) | (1<<4) | pre;
        #endif
        
        // 0c1a must be set output
        W3DDR |= (1<<W3BPIN);
        TCNT3 = 0;
        // set this global flag to indicate oc3a is running with a specifi prescaler
        g_u8Timer3_flag = 1;
    }
    else
    {
        // do nothing as it will mess with oc3a output

        // set the prescaler in the register
        TCCR3A |= (1<<4);
        TCCR3A &= ~(1<<0);
        TCCR3A &= ~(1<<1);
        // 0c3c must be set output
        W3DDR |= (1<<W3BPIN);
    }
    
}

void waveGenerator3B_stop(void)
{
    TCNT3 = 0;
    // oc1b disconnected and normal port operation
    TCCR3A &= ~(1<<5);      
    TCCR3A &= ~(1<<4);
    // reset the flag to indicate oc1a can use it
    g_u8Timer3_flag = 0;
}

/*******************************************************************************/

void waveGenerator3C_set(uint32_t freq)
{
    uint16_t prescaler = 0;
    uint8_t pre = 0;

    // wave freq = FOSC / 2*(OCR3A + 1) * prescaler
    // least possible prescaler is better

    // set the prescaler in the register
    // generator b left unchanged here as it maybe used in pwm mode rather than disabling it here
    // so |= is required here
    // mode 4 or 12 is used depending on the definition in the header file
    if (!g_u8Timer3_flag)
    {
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
        #if defined(__WAVE_USE_OCR3A)
            OCR3A = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #elif defined(__WAVE_USE_ICR3)
            ICR3 = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #endif
        
        // set the prescaler in the register
        TCCR3A |= (1<<2);
        TCCR3A &= ~(1<<0);
        TCCR3A &= ~(1<<1);
        #if defined(__WAVE_USE_OCR3A)
            TCCR3B = (1<<3) | pre;
        #elif defined(__WAVE_USE_ICR3)
            TCCR3B = (1<<3) | (1<<4) | pre;
        #endif
        
        // 0c3c must be set output
        W3DDR |= (1<<W3CPIN);
        TCNT3 = 0;
        // set this global flag to indicate oc1a is running with a specifi prescaler
        g_u8Timer3_flag = 1;
    }
    else
    {
        // do nothing as it will mess with oc3a output
        
        // set the prescaler in the register
        TCCR3A |= (1<<2);
        TCCR3A &= ~(1<<0);
        TCCR3A &= ~(1<<1);
        // 0c3c must be set output
        W3DDR |= (1<<W3CPIN);
    }
    
}

void waveGenerator3C_stop(void)
{
    TCNT3 = 0;
    // oc1b disconnected and normal port operation
    TCCR3A &= ~(1<<2);      
    TCCR3A &= ~(1<<3);
    // reset the flag to indicate oc1a/oc1b can use it
    g_u8Timer3_flag = 0;
}

#endif
