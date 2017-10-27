#include "adc.h"

volatile uint16_t adcval;
volatile float adcValue;
uint8_t gmode;
float volt;

// NOTE: THE "do nothing" in the default cases is for when we need to change 
//the adc channel use we have to call the init again with the new inpupt 
//and just put zeros in the rest of the parameters

// THIS FUNCTION DEALS ONLY WITH THE SINGLE ENDED ADC ONLY
void adc_init(uint8_t ainput, int8_t convSpeed, signed char refVolt, int8_t mode)
{
    // pin-0 is default selected
    // mode is either 8 bit or 10 bit
    
    #ifdef INTE     // compile if interrupt is enabled for this module
    ADCSRA |= (1<<3);
    sei();
    #endif
    
    // set the analog input channel
    switch (ainput)
    {
        case 0:
        DDRA &= ~(1<<0);
        ADMUX &= 0XE0;
        break;
        case 1:
        DDRA &= ~(1<<1);
        ADMUX &= 0XE0;
        ADMUX |= (1<<0);
        break;
        case 2:
        DDRA &= ~(1<<2);
        ADMUX &= 0XE0;
        ADMUX |= (1<<1);
        break;
        case 3:
        DDRA &= ~(1<<3);
        ADMUX &= 0XE0;
        ADMUX |= (1<<0) | (1<<1);
        break;
        case 4:
        DDRA &= ~(1<<4);
        ADMUX &= 0XE0;
        ADMUX |= (1<<2);
        break;
        case 5:
        DDRA &= ~(1<<5);
        ADMUX &= 0XE0;
        ADMUX |= (1<<0) | (1<<2);
        break;
        case 6:
        DDRA &= ~(1<<6);
        ADMUX &= 0XE0;
        ADMUX |= (1<<1) | (1<<2);
        break;
        case 7:
        DDRA &= ~(1<<7);
        ADMUX &= 0XE0;
        ADMUX |= (1<<0) | (1<<1) | (1<<2);
        break;
        default:
        // do nothing
        break;
    }
    // set the conversion speed
    switch (convSpeed)
    {
        case -1:
            ADCSRA |= (1<<0)|(1<<1)|(1<<2);
            break;
        case 2:
            ADCSRA |= (1<<0);
            break;
        case 4:
            ADCSRA |= (1<<1);
            break;
        case 8:
            ADCSRA |= (1<<0)|(1<<1);
            break;
        case 16:
            ADCSRA |= (1<<2);
            break;
        case 32:
            ADCSRA |= (1<<0)|(1<<2);
            break;
        case 64:
            ADCSRA |= (1<<1)|(1<<2);
            break;
        case 128:
            ADCSRA |= (1<<0)|(1<<1)|(1<<2);
            break;
        default:
            //ADCSRA |= (1<<0)|(1<<1)|(1<<2);
            // do nothing
            break;
    }
    switch (refVolt)
    {
        case -1:           // default if none is chosen
            ADMUX &= ~(1<<7);
            ADMUX &= ~(1<<6);
            volt = 5.0;
            break;
        case 'i':       // internal
        case 'I':
            ADMUX |= (1<<6) | (1<<7);
            volt = 2.56;
            break;
        case 'v':       // connected to Avcc = Vcc
        case 'V':
            ADMUX |= (1<<6);
            ADMUX &= ~(1<<7);
            volt = 5.0;
            break;
        case 'e':       // connected to external source can be any volatge value
        case 'E':
            ADMUX &= ~(1<<7);
            ADMUX &= ~(1<<6);
            volt = 5.0;
            break;
        default:
            // do nothing
            break;
    }
    if (mode == -1)
    {
        ADMUX |= (1<<5);    // HERE READ ONLY THE HIGH BYTE OF THE ADC
        gmode = 8;      // set the mode global variable
    }
    else if(mode == 8)
    {
        ADMUX |= (1<<5);    // HERE READ ONLY THE HIGH BYTE OF THE ADC
        gmode = 8;
    }
    else if(mode == 10)
    {
        ADMUX &= ~(1<<5);   // HERE READ THE LOW BYTE FIRST THEN THE HIGH BYTE
        gmode = 10;
    }
    
    // enable the adc module
    ADCSRA |= (1<<ADEN);
    ADCSRA |= (1<<ADSC);
}   //END ADC_INIT FUNCTION 


#ifndef INTE
// upon calling this function it reads in only one sample and saves it in global variable (adcval)
uint16_t adc_read(void)
{
    if (gmode == 8){
        ADCSRA |= (1<<6);
        while((ADCSRA & (1<<ADIF)) == 0);
        ADCSRA |= (1<<ADIF);
        adcval = ADCH;
    }
    else if (gmode == 10){
        ADCSRA |= (1<<6);
        while((ADCSRA & (1<<ADIF)) == 0);
        ADCSRA |= (1<<ADIF);
        adcval = ADCL;
        adcval = adcval | (ADCH << 8);
    }
    adcValue = adcval;
    return adcValue;
}
#endif

#ifdef INTE
ISR(ADC_vect)
{
    if(gmode == 8){
        adcval = ADCH;
        adcValue = adcval;
    }
    else if (gmode == 10){
        ADCSRA |= (1<<6);
        adcval = ADCL;
        adcval = adcval | (ADCH << 8);
        adcValue = adcval;
    }
    ADCSRA |= (1<<6);       // START CONVERSION AGAIN
}

uint16_t adc_read(void)
{
    return adcValue;
}

#endif

void adc_stop(void)
{
    ADCSRA &= ~(1<<ADIE);
    ADCSRA &= ~(1<<ADEN);
}

void adc_restart(void)
{
    #ifdef INTE
        ADCSRA |= (1<<ADIE);
    #endif
    ADCSRA |= (1<<ADEN);
    ADCSRA |= (1<<6);       // START CONVERSION AGAIN
}
