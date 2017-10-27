// timer 1 uses mode 14 which uses ICR1 for top value..
// cant use ICR1 for something else while using this lib.
// FIXME: add a support for mode 15 which uses OCR1A for top value.
//          but doing so will make OC1A not an option while in that mode.
// FIXME: to add support for mode 15 follow the steps done in wave_gen.c
//                  #if defined(__WAVE_USE_OCR1A)
//                  OCR1A = 65535;
//                  #elif defined(__WAVE_USE_ICR1)
//                  ICR1 = 65535;
//                  #endif
// + change the mode in registers + top value register

// LOG:
/* 30-7-2017 : added a part in stopPWM TCCRn Register to make it normal port when stopping it
*                       : added a part in dutyPWM to reenable pwm mode after it is been disabled by stopPWM
*/

#ifndef __FAST_PWM__
#define __FAST_PWM__

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// NOTE: added for geany proj plug-in to catch the def and go to the file by pressing on
// fastPWM.h
extern uint8_t fastPWM;

//#define __PWM_USE_OCR1A
//#define __PWM_USE_ICR1

#ifndef F_CPU
    #define F_CPU 16000000UL
#endif
#define PWM0 3
#define PWM1A 5
#define PWM1B 4
#define PWM2 7

void fastPWM0_init(uint32_t freq);
void fastPWM0_duty(uint8_t duty);
void fastPWM0_stop(void);

// dont operate both 1A and 1B at the same time
// only operate them together if they operate on the same freq
void fastPWM1A_init(uint32_t freq);
void fastPWM1A_duty(float duty);
void fastPWM1A_stop(void);

void fastPWM1B_init(uint32_t freq);
void fastPWM1B_duty(float duty);
void fastPWM1B_stop(void);

void fastPWM2_init(uint32_t freq);
void fastPWM2_duty(uint8_t duty);
void fastPWM2_stop(void);

#endif
