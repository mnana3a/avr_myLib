/* log:
// xxMOD:28-9-2017 as isr only gets reading and do nothing until u read it
// MOD:28-9-2017 changed when there is an overflow no need to ignore the reading as it is valid with the 2nd comp
// MOD: add a way to auto scale the freq range (prescaler) i.e, unknown freq that the micro auto range for it
*/
// NOTE: this driver doesn't assume where the signal is comming from [analog comp - external pin] it just uses it, by default it is from the external pin but if analog comp is used this driver will work with it as well

// the input to this driver can come from two sources:
//          1- the external input capture pin
//          2- the output of the analog comparator ( by setting it in the comparator registers )

// this lib works in two ways:
// first:   uses polling mode for capturing once then stops the capturing mode.
//              this mode can be used with sensors and has a high range for measruing up to 1Mhz
//
// second : uses interrupt and this mode doesnt stop the timer it works permenently 
//                  this mode can be used for applications that depend mainly on capturing and measure singalns for the most part

// NOTE: using _delay_ms messes up with the functionality of the code and the input capture
// FIXME: with low freq the result is wrong
// NOTE: This lib changes timer1 prescaler, so make sure if timer 1 is used elsewhere that the prescaler isnt messed up
// FIXME: add a way to protect the prescaler between libs

// FIXME: add an overflow check for when the max prescaler is selected but still the period is larger

// NOTE: (done) a fix for overflow happeneing and giving a faulty results is to check for the overflow when getting the 2nd reading and if there is an overflow. clear it, ignore this reading and get another one

// FIXME: add a way in polling mode when you finish measuring the pulse/period, to not disable the timer if another lib is using it, and to disable it if there is no other lib using it............... it can be done using #define in all libs using the timer and make the init function define it so when it is defined dont disable the timer when done

// NOTE: to use auto scale, init the inputCap with 0 as default and then use the autorange function
// auto scale can be done by:
//          --> add a function that does the follwoing: uint16_t inputCapture_autoRange(void)
//                          1- init the input capture unit and measures the freq with default pre (64)
//                          1.5- if the measured freq > 200Khz set the prescaler to 1
//                          2- if the measured freq <= 200Khz set the prescaler to 8
//                          3- if the measured freq <= 100Khz set the prescaler to 64
//                          4- if the measured freq <= 10Khz set the prescaler to 256
//                          5- if the measured freq <= 100hz set the prescaler to 1024
//                  the function returns the prescaler to be used if needed and for checking

// NOTE: tested a code with read, calculate period , read second in capture isr
//          vs a test flag first read, test flag 2nd read, until you use the value dont read..... and the second was far better in assembly 

#ifndef __I_CAPTURE_H__
#define __I_CAPTURE_H__

#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <math.h>

// NOTE: added for geany proj plug-in to catch the def and go to the file by pressing on
// lcd.h
extern uint8_t icapture;

#if defined (__WAVE_USE_ICR1)
#undef __WAVE_USE_ICR1
#define __WAVE_USE_OCR1A
#endif

#if defined (__PWM_USE_ICR1)
#undef __PWM_USE_ICR1
#define __PWM_USE_OCR1A
#endif

#define INTE
#define __PER_INT
#define __LO_FREQ           // always use it
//#define __HI_FREQ     // only use it when the freq is above 1.5 Mhz up to 2 Mhz

#ifndef F_CPU
    #define F_CPU 16000000UL
#endif
//#define SREG (*((volatile uint8_t * const)(0x3f)))
//char register volatile value asm("r16");

#define ICP1 6

#ifdef __PER_INT
volatile uint16_t g_u16CapValue;        // used to save the measured period when interrupt used
volatile uint8_t g_u8Flag;
#endif
uint16_t g_u16Prescaler;            // use it when linked with another lib that uses timer 1

// maxPeriod is in micro seconds, if unknown put = 0
// noiseCanceler = 0  if not wanted
// edge = 0 falling edge , = 1 rising edge 
void inputCapture_init(const uint32_t maxPeriod, const uint8_t noiseCanceler, const uint8_t edge, const uint8_t pwmFlag);
// check the output of those functions as if it didnt read correctly it will return 0
float inputCapture_getPeriod(void);
uint32_t inputCapture_getFreq(void);
uint32_t inputCapture_calculateMaxPeriod(uint32_t freq);


#endif
