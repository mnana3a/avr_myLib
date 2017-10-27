#include "wave_gen.h"

void waveGenerator0_set(uint32_t freq)
{
    uint16_t prescaler = 0;
    // wave freq = FOSC / 2*(OCR0 + 1) * prescaler
    if(freq <= 30){
        prescaler = 1024;
        OCR0 = 255;
    }
    else{
        if(freq <= (F_CPU/2) && freq >= (F_CPU/512))    prescaler = 1;
        else if(freq <= (F_CPU/16) && freq >= (F_CPU/4096)) prescaler = 8;
        else if(freq <= (F_CPU/128) && freq >= (F_CPU/32768))   prescaler = 64;
        else if(freq <= (F_CPU/512) && freq >= (F_CPU/131072))  prescaler = 256;
        else if(freq <= (F_CPU/2048) && freq >= (F_CPU/524288)) prescaler = 1024;
    
        OCR0 = (F_CPU / (2ul * prescaler * freq)) - 1ul;
    }
    // set the prescaler in the register
    switch(prescaler)
    {
        case 1:
            TCCR0 |= (1<<0) | (1<<3) | (1<<4);
            TCCR0 &= ~(1<<1);
            TCCR0 &= ~(1<<2);
            break;
        case 8:
            TCCR0 |= (1<<1) | (1<<3) | (1<<4);
            TCCR0 &= ~(1<<0);
            TCCR0 &= ~(1<<2);
            break;
        case 64:
            TCCR0 |= (1<<0) | (1<<1) | (1<<3) | (1<<4);
            TCCR0 &= ~(1<<2);
            break;
        case 256:
            TCCR0 |= (1<<2) | (1<<3) | (1<<4);
            TCCR0 &= ~(1<<0);
            TCCR0 &= ~(1<<1);
            break;
        case 1024:
            TCCR0 |= (1<<0) | (1<<2) | (1<<3) | (1<<4);
            TCCR0 &= ~(1<<1);
            break;
        default:
            
            break;
    }
    // pb3 must be set output ( oc0 )
    DDRB |= (1<<W0PIN);
    // init TCNT0 to 0
    TCNT0 = 0;
}

void waveGenerator0_stop(void)
{
    TCNT0 = 0;
    // oc2 disconnected and normal port operation
    TCCR0 &= ~(1<<4);       
    TCCR0 &= ~(1<<5);   
}

/*******************************************************************************/

void waveGenerator2_set(uint32_t freq)
{
    uint16_t prescaler = 0;
    // wave freq = FOSC / 2*(OCR2 + 1) * prescaler
    if(freq <= 30){
        prescaler = 1024;
        OCR2 = 255;
    }
    else{
        if(freq <= (F_CPU/2) && freq >= (F_CPU/512))    prescaler = 1;
        else if(freq <= (F_CPU/16) && freq >= (F_CPU/4096)) prescaler = 8;
        else if(freq <= (F_CPU/64) && freq >= (F_CPU/16384))    prescaler = 32;
        else if(freq <= (F_CPU/128) && freq >= (F_CPU/32768))   prescaler = 64;
        else if(freq <= (F_CPU/256) && freq >= (F_CPU/65536))   prescaler = 128;
        else if(freq <= (F_CPU/512) && freq >= (F_CPU/131072))  prescaler = 256;
        else if(freq <= (F_CPU/2048) && freq >= (F_CPU/524288)) prescaler = 1024;
        else if(freq > (F_CPU/2))   prescaler = 1;
    
        OCR2 = (F_CPU / (2ul * prescaler * freq)) - 1ul;
    }
    // set the prescaler in the register
    switch(prescaler)
    {
        case 1:
            TCCR2 |= (1<<0) | (1<<3) | (1<<4);
            TCCR2 &= ~(1<<1);
            TCCR2 &= ~(1<<2);
            break;
        case 8:
            TCCR2 |= (1<<1) | (1<<3) | (1<<4);
            TCCR2 &= ~(1<<0);
            TCCR2 &= ~(1<<2);
            break;
        case 32:
            TCCR2 |= (1<<0) | (1<<1) | (1<<3) | (1<<4);
            TCCR2 &= ~(1<<2);
            break;
        case 64:
            TCCR2 |= (1<<2) | (1<<3) | (1<<4);
            TCCR2 &= ~(1<<0);
            TCCR2 &= ~(1<<1);
            break;
        case 128:
            TCCR2 |= (1<<0) | (1<<2) | (1<<3) | (1<<4);
            TCCR2 &= ~(1<<1);
            break;
        case 256:
            TCCR2 |= (1<<1) | (1<<2) | (1<<3) | (1<<4);
            TCCR2 &= ~(1<<0);
            break;
        case 1024:
            TCCR2 |= (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4);
            break;
        default:
            
            break;
    }
    // pD7 must be set output ( oc2 )
    DDRD |= (1<<W2PIN);
    // init TCNT0 to 0
    TCNT2 = 0;
}

void waveGenerator2_stop(void)
{
    TCNT2 = 0;
    // oc2 disconnected and normal port operation
    TCCR2 &= ~(1<<4);       
    TCCR2 &= ~(1<<5);   
}

/*******************************************************************************/

void waveGenerator1A_set(uint32_t freq)
{
    uint16_t prescaler = 0;
    // wave freq = FOSC / 2*(OCR1A + 1) * prescaler
    if(freq <= 0){
        prescaler = 1024;
        #if defined(__WAVE_USE_OCR1A)
        OCR1A = 65535;
        #elif defined(__WAVE_USE_ICR1)
        ICR1 = 65535;
        #endif
    }
    else{
        if(freq <= (F_CPU/2) && freq >= (F_CPU/131072)) prescaler = 1;
        else if(freq <= (F_CPU/16) && freq >= (F_CPU/1048576))  prescaler = 8;
        else if(freq <= (F_CPU/128) && freq >= (F_CPU/8388608)) prescaler = 64;
        else if(freq <= (F_CPU/512) && freq >= (F_CPU/33554432))    prescaler = 256;
        else if(freq <= (F_CPU/2048) && freq >= (F_CPU/134217728))  prescaler = 1024;
        else if(freq > (F_CPU/2))   prescaler = 1;
        #if defined(__WAVE_USE_OCR1A)
        OCR1A = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #elif defined(__WAVE_USE_ICR1)
        ICR1 = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #endif
    }
    // set the prescaler in the register
    // generator b left unchanged here as it maybe used in pwm mode rather than disabling it here
    // so |= is required here
    // mode 4 or 12 is used depending on the definition in the header file
    switch(prescaler)
    {
        case 1:
            TCCR1A |= (1<<6);
            TCCR1A &= ~(1<<0);
            TCCR1A &= ~(1<<1);
            TCCR1B |= (1<<3);
            #if defined(__WAVE_USE_OCR1A)
            TCCR1B &= ~(1<<4);
            #elif defined(__WAVE_USE_ICR1)
            TCCR1B |= (1<<4);
            #endif
            TCCR1B |= (1<<0);
            TCCR1B &= ~(1<<1);
            TCCR1B &= ~(1<<2);
            break;
        case 8:
            TCCR1A |= (1<<6);
            TCCR1A &= ~(1<<0);
            TCCR1A &= ~(1<<1);
            TCCR1B |= (1<<3);
            #if defined(__WAVE_USE_OCR1A)
            TCCR1B &= ~(1<<4);
            #elif defined(__WAVE_USE_ICR1)
            TCCR1B |= (1<<4);
            #endif
            TCCR1B |= (1<<1);
            TCCR1B &= ~(1<<0);
            TCCR1B &= ~(1<<2);
            break;
        case 64:
            TCCR1A |= (1<<6);
            TCCR1A &= ~(1<<0);
            TCCR1A &= ~(1<<1);
            TCCR1B |= (1<<3);
            #if defined(__WAVE_USE_OCR1A)
            TCCR1B &= ~(1<<4);
            #elif defined(__WAVE_USE_ICR1)
            TCCR1B |= (1<<4);
            #endif
            TCCR1B |= (1<<0) | (1<<1);
            TCCR1B &= ~(1<<2);
            break;
        case 256:
            TCCR1A |= (1<<6);
            TCCR1A &= ~(1<<0);
            TCCR1A &= ~(1<<1);
            TCCR1B |= (1<<3);
            #if defined(__WAVE_USE_OCR1A)
            TCCR1B &= ~(1<<4);
            #elif defined(__WAVE_USE_ICR1)
            TCCR1B |= (1<<4);
            #endif
            TCCR1B |= (1<<2);
            TCCR1B &= ~(1<<0);
            TCCR1B &= ~(1<<1);
            break;
        case 1024:
            TCCR1A |= (1<<6);
            TCCR1A &= ~(1<<0);
            TCCR1A &= ~(1<<1);
            TCCR1B |= (1<<3);
            #if defined(__WAVE_USE_OCR1A)
            TCCR1B &= ~(1<<4);
            #elif defined(__WAVE_USE_ICR1)
            TCCR1B |= (1<<4);
            #endif
            TCCR1B |= (1<<0) | (1<<2);
            TCCR1B &= ~(1<<1);
            break;
        default:
            
            break;
    }
    // pD5 must be set output ( oc1a )
    DDRD |= (1<<W1APIN);
    // init TCNT1 to 0
    TCNT1 = 0;
}

void waveGenerator1A_stop(void)
{
    TCNT1 = 0;
    // oc1a disconnected and normal port operation
    TCCR1A &= ~(1<<7);      
    TCCR1A &= ~(1<<6);  
}

/*******************************************************************************/

void waveGenerator1B_set(uint32_t freq)
{
    uint16_t prescaler = 0;
    // wave freq = FOSC / 2*(OCR1B + 1) * prescaler
    if(freq <= 0){
        prescaler = 1024;
        #if defined(__WAVE_USE_OCR1A)
        OCR1A = 65535;
        #elif defined(__WAVE_USE_ICR1)
        ICR1 = 65535;
        #endif
    }
    else{
        if(freq <= (F_CPU/2) && freq >= (F_CPU/131072)) prescaler = 1;
        else if(freq <= (F_CPU/16) && freq >= (F_CPU/1048576))  prescaler = 8;
        else if(freq <= (F_CPU/128) && freq >= (F_CPU/8388608)) prescaler = 64;
        else if(freq <= (F_CPU/512) && freq >= (F_CPU/33554432))    prescaler = 256;
        else if(freq <= (F_CPU/2048) && freq >= (F_CPU/134217728))  prescaler = 1024;
        else if(freq > (F_CPU/2))   prescaler = 1;
        #if defined(__WAVE_USE_OCR1A)
        OCR1A = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #elif defined(__WAVE_USE_ICR1)
        ICR1 = (F_CPU / (2ul * prescaler * freq)) - 1ul;
        #endif
    }
    // set the prescaler in the register
    // generator b left unchanged here as it maybe used in pwm mode rather than disabling it here
    // so |= is required here
// mode 4 or 12 is used depending on the definition in the header file
    switch(prescaler)
    {
        case 1:
            TCCR1A |= (1<<4);
            TCCR1A &= ~(1<<0);
            TCCR1A &= ~(1<<1);
            TCCR1B |= (1<<3);
            #if defined(__WAVE_USE_OCR1A)
            TCCR1B &= ~(1<<4);
            #elif defined(__WAVE_USE_ICR1)
            TCCR1B |= (1<<4);
            #endif
            TCCR1B |= (1<<0);
            TCCR1B &= ~(1<<1);
            TCCR1B &= ~(1<<2);
            break;
        case 8:
            TCCR1A |= (1<<4);
            TCCR1A &= ~(1<<0);
            TCCR1A &= ~(1<<1);
            TCCR1B |= (1<<3);
            #if defined(__WAVE_USE_OCR1A)
            TCCR1B &= ~(1<<4);
            #elif defined(__WAVE_USE_ICR1)
            TCCR1B |= (1<<4);
            #endif
            TCCR1B |= (1<<1);
            TCCR1B &= ~(1<<0);
            TCCR1B &= ~(1<<2);
            break;
        case 64:
            TCCR1A |= (1<<4);
            TCCR1A &= ~(1<<0);
            TCCR1A &= ~(1<<1);
            TCCR1B |= (1<<3);
            #if defined(__WAVE_USE_OCR1A)
            TCCR1B &= ~(1<<4);
            #elif defined(__WAVE_USE_ICR1)
            TCCR1B |= (1<<4);
            #endif
            TCCR1B |= (1<<0) | (1<<1);
            TCCR1B &= ~(1<<2);
            break;
        case 256:
            TCCR1A |= (1<<4);
            TCCR1A &= ~(1<<0);
            TCCR1A &= ~(1<<1);
            TCCR1B |= (1<<3);
            #if defined(__WAVE_USE_OCR1A)
            TCCR1B &= ~(1<<4);
            #elif defined(__WAVE_USE_ICR1)
            TCCR1B |= (1<<4);
            #endif
            TCCR1B |= (1<<2);
            TCCR1B &= ~(1<<0);
            TCCR1B &= ~(1<<1);
            break;
        case 1024:
            TCCR1A |= (1<<4);
            TCCR1A &= ~(1<<0);
            TCCR1A &= ~(1<<1);
            TCCR1B |= (1<<3);
            #if defined(__WAVE_USE_OCR1A)
            TCCR1B &= ~(1<<4);
            #elif defined(__WAVE_USE_ICR1)
            TCCR1B |= (1<<4);
            #endif
            TCCR1B |= (1<<0) | (1<<2);
            TCCR1B &= ~(1<<1);
            break;
        default:
            
            break;
    }
    // pD4 must be set output ( oc1b )
    DDRD |= (1<<W1BPIN);
    // init TCNT1 to 0
    TCNT1 = 0;
}

void waveGenerator1B_stop(void)
{
    TCNT1 = 0;
    // oc1b disconnected and normal port operation
    TCCR1A &= ~(1<<5);      
    TCCR1A &= ~(1<<4);  
}
