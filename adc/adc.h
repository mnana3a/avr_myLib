// TODO: add support for differential adc modes
// NOTE: THIS DRIVER DEALS ONLY WITH THE SINGLE ENDED ADC ONLY
// NOTE: if you're using the 8-bit mode when you turn the digital value
//              into analog val * 5 / 255 -----> not 1024 as you only read 8 bits

// undef the inte macro to disable the interrupt method
// by default the interrupt method is being used

// **===> the variable ( adcval ) is being used to store the adc conversion through interrupt handler
//      so use it when you need to use the conversion value

// the default value for the conversion vcc & Vref is 5v , if you will use external value provide it
// ADC LIB RETURNS A FLOAT VALUE

#ifndef __ADC_H__
#define __ADC_H__

#ifndef F_CPU
    #define F_CPU 16000000UL
#endif
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

//#define INTE


// NOTE: added for geany proj plug-in to catch the def and go to the file by pressing on
// lcd.h
extern uint8_t adc;

#ifdef INTE
#include <avr/interrupt.h>
#endif

extern volatile uint16_t adcval;
extern volatile float adcValue;
extern uint8_t gmode;
extern float volt;

void adc_init(uint8_t ainput, int8_t convSpeed, signed char refVolt, int8_t mode); 
uint16_t adc_read(void);
void adc_stop(void);
void adc_restart(void)

#endif

