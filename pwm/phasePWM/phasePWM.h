// USER: dont operate both 1A and 1B at the same time only operate them together if they operate on the same freq
// USER: the freq upper limit is F_CPU/2
// USER: to change the freq, must stop the ocx first then change it
// USER: cant use ICR1 for something else while using OC1A
// USER: WHEN USING A TIMER IN A PWM MODE DONT USE IT IN ANOTHER MODE ALONG SIDE PWM MODE cause wave gen mode changes the timer sequence
// USER: dont operate both 1A and 1B at the same time, only operate them together if they operate on the same freq

// LOG:
/* 30-7-2017 : added a part in stopPWM TCCRn Register to make it normal port when stopping it
*                       : added a part in dutyPWM to reenable pwm mode after it is been disabled by stopPWM
*/

#ifndef __PHASE_PWM__
#define __PHASE_PWM__

#ifndef F_CPU
    #define F_CPU 16000000UL
#endif

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// NOTE: added for geany proj plug-in to catch the def and go to the file by pressing on
// phasePWM.h
extern uint8_t phasePWM;


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
    #define PWM0PIN  3
    #define PWM1APIN 5
    #define PWM1BPIN 4
    #define PWM2PIN  7
#elif defined(__AVR_ATmega128__)
    #define PWM0DDR DDRB
    #define PWM1DDR DDRB
    #define PWM2DDR DDRB
    #define PWM3DDR DDRE
    #define PWM0PIN   4
    #define PWM1APIN  5
    #define PWM1BPIN  6
    #define PWM1CPIN  7
    #define PWM2PIN   7
    #define PWM3APIN  3
    #define PWM3BPIN  4
    #define PWM3CPIN  5
#endif

// these are used to perform division by 10 or 100 without using software divider
// derived from ((1/100)*2^8)
#define DIV100_8  3u
// derived from ((1/100)*2^16)
#define DIV100_16 655u          // this is changed from 656 in other files as it causes overflow issues with this file
// derived from ((1/100)*2^32)
#define DIV100_32 42949673ul
// derived from ((1/10)*2^8)
#define DIV10_8   26u
// derived from ((1/10)*2^16)
#define DIV10_16  6554u

#define AV_01 ((uint32_t)(((F_CPU >> 9)  + (F_CPU >> 12)) >> 1))
#define AV_02 ((uint32_t)(((F_CPU >> 12) + (F_CPU >> 15)) >> 1))
#define AV_03 ((uint32_t)(((F_CPU >> 15) + (F_CPU >> 17)) >> 1))
#define AV_04 ((uint32_t)(((F_CPU >> 17) + (F_CPU >> 19)) >> 1))

#define AV_11 ((uint32_t)(((F_CPU >> 9)  + (F_CPU >> 12)) >> 1))
#define AV_12 ((uint32_t)(((F_CPU >> 12) + (F_CPU >> 14)) >> 1))
#define AV_13 ((uint32_t)(((F_CPU >> 14) + (F_CPU >> 15)) >> 1))
#define AV_14 ((uint32_t)(((F_CPU >> 15) + (F_CPU >> 16)) >> 1))
#define AV_15 ((uint32_t)(((F_CPU >> 16) + (F_CPU >> 17)) >> 1))
#define AV_16 ((uint32_t)(((F_CPU >> 17) + (F_CPU >> 19)) >> 1))

void phasePWM0_init(uint32_t freq);
void phasePWM0_duty(uint8_t duty);
void phasePWM0_stop(void);

void phasePWM1A_init(uint32_t freq);
void phasePWM1A_duty(uint8_t duty);
void phasePWM1A_stop(void);

void phasePWM1B_init(uint32_t freq);
void phasePWM1B_duty(uint8_t duty);
void phasePWM1B_stop(void);

void phasePWM2_init(uint32_t freq);
void phasePWM2_duty(uint8_t duty);
void phasePWM2_stop(void);

#if defined(__AVR_ATmega128__)
void phasePWM1C_init(uint32_t freq);
void phasePWM1C_duty(uint8_t duty);
void phasePWM1C_stop(void);

void phasePWM3A_init(uint32_t freq);
void phasePWM3A_duty(uint8_t duty);
void phasePWM3A_stop(void);

void phasePWM3B_init(uint32_t freq);
void phasePWM3B_duty(uint8_t duty);
void phasePWM3B_stop(void);

void phasePWM3C_init(uint32_t freq);
void phasePWM3C_duty(uint8_t duty);
void phasePWM3C_stop(void);
#endif

#endif
