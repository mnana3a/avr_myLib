// USER : the freq upper limit is F_CPU/2
// USER : to change the freq, must stop the ocx first then change it
// timer 1 uses mode 14 which uses ICR1 for top value..
// cant use ICR1 for something else while using this lib.

// FIXME : add a support for mode 15 which uses OCR1A for top value.
//          but doing so will make OC1A not an option while in that mode.
// FIXME : to add support for mode 15 follow the steps done in wave_gen.c
//                  #if defined(__WAVE_USE_OCR1A)
//                  OCR1A = 65535;
//                  #elif defined(__WAVE_USE_ICR1)
//                  ICR1 = 65535;
//                  #endif
// + change the mode in registers + top value register

// LOG :
/* 30-7-2017 : added a part in stopPWM TCCRn Register to make it normal port when stopping it
*                       : added a part in dutyPWM to reenable pwm mode after it is been disabled by stopPWM
*/

// USER : WHEN USING A TIMER IN A PWM MODE DONT USE IT IN ANOTHER MODE ALONG SIDE PWM MODE cause wave gen mode changes the timer sequence

// USER : dont operate both 1A and 1B at the same time, only operate them together if they operate on the same freq

#ifndef __FAST_PWM__
#define __FAST_PWM__

#ifndef F_CPU
    #define F_CPU 16000000UL
#endif

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// NOTE : added for geany proj plug-in to catch the def and go to the file by pressing on
// fastPWM.h
extern uint8_t fastPWM;

#define __PWM_USE_OCR1A

#ifdef __CAP_USE_ICR1
    #define __PWM_USE_OCR1A
    #undef __PWM_USE_ICR1
#endif

#if defined(__AVR_ATmega128__)
    #define __PWM_USE_OCR3A
    
    #ifdef __CAP_USE_ICR3
        #define __PWM_USE_OCR3A
        #undef __PWM_USE_ICR3
    #endif
#endif


#if defined(__AVR_ATmega32__) || defined(__AVR_ATmega16__)
    #define PWM0DDR DDRB
    #define PWM1DDR DDRD
    #define PWM2DDR DDRD
    #define PWM0PIN   3
    #define PWM1APIN 5
    #define PWM1BPIN 4
    #define PWM2PIN 7
#elif defined(__AVR_ATmega128__)
    #define PWM0DDR DDRB
    #define PWM1DDR DDRB
    #define PWM2DDR DDRB
    #define PWM3DDR DDRE
    #define PWM0PIN   4
    #define PWM1APIN 5
    #define PWM1BPIN 6
    #define PWM1CPIN 7
    #define PWM2PIN 7
    #define PWM3APIN 3
    #define PWM3BPIN 4
    #define PWM3CPIN 5
#endif

// these are used to perform division by 10 or 100 without using software divider
// derived from ((1/100)*2^8)
#define DIV100_8 3u
// derived from ((1/100)*2^16)
#define DIV100_16 656u
// derived from ((1/10)*2^8)
#define DIV10_8 26u
// derived from ((1/10)*2^16)
#define DIV10_16 6554u

#define AV_01 ((uint32_t)(((F_CPU >> 8) + (F_CPU >> 11)) >> 1))
#define AV_02 ((uint32_t)(((F_CPU >> 11) + (F_CPU >> 14)) >> 1))
#define AV_03 ((uint32_t)(((F_CPU >> 14) + (F_CPU >> 16)) >> 1))
#define AV_04 ((uint32_t)(((F_CPU >> 16) + (F_CPU >> 18)) >> 1))

#define AV_11 ((uint32_t)(((F_CPU >> 8) + (F_CPU >> 11)) >> 1))
#define AV_12 ((uint32_t)(((F_CPU >> 11) + (F_CPU >> 13)) >> 1))
#define AV_13 ((uint32_t)(((F_CPU >> 13) + (F_CPU >> 14)) >> 1))
#define AV_14 ((uint32_t)(((F_CPU >> 14) + (F_CPU >> 15)) >> 1))
#define AV_15 ((uint32_t)(((F_CPU >> 15) + (F_CPU >> 16)) >> 1))
#define AV_16 ((uint32_t)(((F_CPU >> 16) + (F_CPU >> 18)) >> 1))


void fastPWM0_init(uint32_t freq);
void fastPWM0_duty(uint8_t duty);
void fastPWM0_stop(void);

void fastPWM1A_init(uint32_t freq);
void fastPWM1A_duty(uint8_t duty);
void fastPWM1A_stop(void);

void fastPWM1B_init(uint32_t freq);
void fastPWM1B_duty(uint8_t duty);
void fastPWM1B_stop(void);

void fastPWM2_init(uint32_t freq);
void fastPWM2_duty(uint8_t duty);
void fastPWM2_stop(void);

#if defined(__AVR_ATmega128__)
void fastPWM1C_init(uint32_t freq);
void fastPWM1C_duty(uint8_t duty);
void fastPWM1C_stop(void);

void fastPWM3A_init(uint32_t freq);
void fastPWM3A_duty(uint8_t duty);
void fastPWM3A_stop(void);

void fastPWM3B_init(uint32_t freq);
void fastPWM3B_duty(uint8_t duty);
void fastPWM3B_stop(void);

void fastPWM3C_init(uint32_t freq);
void fastPWM3C_duty(uint8_t duty);
void fastPWM3C_stop(void);
#endif

#endif
