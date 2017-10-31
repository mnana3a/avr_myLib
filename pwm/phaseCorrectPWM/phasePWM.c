#include "phasePWM.h"

// timer 0
void phasePWM0_init(uint32_t freq)
{
    // this routine works with non-inverted mode only
    uint16_t prescaler = 0;
    // freq can be from 617222 hz with pre = 1 : 61 hz with pre = 1024

    // select prescaler with the min. possible error margin
    // freq = F_CPU / (256*Prescaler)
    // for default value set the freq argument with the value 0
    if(freq == 0){
        prescaler = 1;
    }
    else
    {
        prescaler = F_CPU / (freq * 510);
        if(prescaler < 1)   prescaler = 1;
        else if(prescaler >= 1 && prescaler <= 4)   prescaler = 1;
        else if(prescaler > 4 && prescaler <= 36)   prescaler = 8;
        else if(prescaler > 36 && prescaler <= 160) prescaler = 64;
        else if(prescaler > 160 && prescaler <= 640)    prescaler = 256;
        else if(prescaler > 640 && prescaler <= 1024)   prescaler = 1024;
        else if(prescaler > 1024)   prescaler = 1024;
    }

    // set the prescaler in the register
    switch(prescaler)
    {
        case 1:
            TCCR0 |= (1<<0) | (1<<6) | (1<<5);
            TCCR0 &= ~(1<<1);
            TCCR0 &= ~(1<<2);
            break;
        case 8:
            TCCR0 |= (1<<1) | (1<<6) | (1<<5);
            TCCR0 &= ~(1<<0);
            TCCR0 &= ~(1<<2);
            break;
        case 64:
            TCCR0 |= (1<<0) | (1<<1) | (1<<6) | (1<<5);
            TCCR0 &= ~(1<<2);
            break;
        case 256:
            TCCR0 |= (1<<2) | (1<<6) | (1<<5);
            TCCR0 &= ~(1<<0);
            TCCR0 &= ~(1<<1);
            break;
        case 1024:
            TCCR0 |= (1<<0) | (1<<2) | (1<<6) | (1<<5);
            TCCR0 &= ~(1<<1);
            break;
        default:
            TCCR0 |= (1<<0) | (1<<6) | (1<<5);
            TCCR0 &= ~(1<<1);
            TCCR0 &= ~(1<<2);
            break;
    }

    // pb3 must be set output ( oc0 )
    DDRB |= (1<<PWM0);
    // init TCNT0 to 0
    TCNT0 = 0;
}

void phasePWM0_duty(uint8_t duty)
{
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR0 |= (1<<5);
    OCR0 = (duty * 255) / 100;
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
    uint16_t prescaler = 0;
    // freq can be from 617222 hz with pre = 1 : 61 hz with pre = 1024

    // select prescaler with the min. possible error margin
    // freq = F_CPU / (256*Prescaler)
    // for default value set the freq argument with the value 0
    if(freq == 0){
        prescaler = 1;
    }
    else
    {
        prescaler = F_CPU / (freq * 510);
        if(prescaler < 1)   prescaler = 1;
        else if(prescaler >= 1 && prescaler <= 4)   prescaler = 1;
        else if(prescaler > 4 && prescaler <= 20)   prescaler = 8;
        else if(prescaler > 20 && prescaler <= 48)  prescaler = 32;
        else if(prescaler > 48 && prescaler <= 96)  prescaler = 64;
        else if(prescaler > 96 && prescaler <= 192) prescaler = 128;
        else if(prescaler > 192 && prescaler <= 640)    prescaler = 256;
        else if(prescaler > 640 && prescaler <= 1024)   prescaler = 1024;
        else if(prescaler > 1024)   prescaler = 1024;
    }

    // set the prescaler in the register
    switch(prescaler)
    {
        case 1:
            TCCR2 |= (1<<0) | (1<<6) | (1<<5);
            TCCR2 &= ~(1<<1);
            TCCR2 &= ~(1<<2);
            break;
        case 8:
            TCCR2 |= (1<<1) | (1<<6) | (1<<5);
            TCCR2 &= ~(1<<0);
            TCCR2 &= ~(1<<2);
            break;
        case 32:
            TCCR2 |= (1<<0) | (1<<1) | (1<<6) | (1<<5);
            TCCR2 &= ~(1<<2);
            break;
        case 64:
            TCCR2 |= (1<<2) | (1<<6) | (1<<5);
            TCCR2 &= ~(1<<0);
            TCCR2 &= ~(1<<1);
            break;
        case 128:
            TCCR2 |= (1<<0) | (1<<2) | (1<<6) | (1<<5);
            TCCR2 &= ~(1<<1);
            break;
        case 256:
            TCCR2 |= (1<<1) | (1<<2) | (1<<6) | (1<<5);
            TCCR2 &= ~(1<<0);
            break;
        case 1024:
            TCCR2 |= (1<<0) | (1<<1) | (1<<2) | (1<<6) | (1<<5);
            break;
        default:
            TCCR2 |= (1<<0) | (1<<6) | (1<<5);
            TCCR2 &= ~(1<<1);
            TCCR2 &= ~(1<<2);
            break;
    }

    // pb3 must be set output ( oc0 )
    DDRD |= (1<<PWM2);
    // init TCNT0 to 0
    TCNT2 = 0;
}

void phasePWM2_duty(uint8_t duty)
{
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR2 |= (1<<5);
    OCR2 = (duty * 255) / 100;
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
}
/************************************************************************/
// timer 1
// OC1A

//FIXME : make the freq float
//FIXME : change the hardcoded freq numbers into F_CPU/2... freq dependent numbers
void phasePWM1A_init(uint32_t freq)
{
    // control OC1A only
    // this routine works with non-inverted mode only
    // and using ICR1 for top value only
    uint16_t prescaler = 0;
    float ffreq = 0.0;
    /* freq can be
            top = 0xff  --> f = 31372.5 : 30.6 hz       with specific values depending on pre
            top = 0x1ff --> f = 15655.57 : 15.3 hz  with specific values depending on pre
            top = 0x3ff --> f = 7820.13 : 7.6 hz    with specific values depending on pre
            top = ICR1 || OCR1A
                    pre = 1    -->  f = 8 M    : 122  hz
                    pre = 1024 -->  f = 7812.5 : 0.119 hz
    */
    // freq = F_CPU / (2 * Prescaler * top)

    // set ICR1 with the required value for that freq.
    if(freq == 0){
        prescaler = 1024;
        ffreq = 0.119;

        ICR1 = (uint16_t)((float)((float)F_CPU / ffreq) / (2 * prescaler));
    }
    else
    {
        // the freq can be changed to F_CPU / const for each case
        // but that will make the code even larger...so for now it works with
        // only 16M and for other freq it requires some tweaking
        if(freq >= 122 && freq <= 8000000)  prescaler = 1;
        else if(freq >= 15 && freq <= 1000000)  prescaler = 8;
        else if(freq >= 2 && freq <= 125000)    prescaler = 64;
        else if(freq >= 0 && freq <= 31250) prescaler = 256;
        else if(freq >= 0 && freq <= 7812)  prescaler = 1024;

        ICR1 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / (2 * prescaler));
    }

    // set the prescaler in the register & mode 10
    // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
    //        that disabled me from using both OC1A & OC1B atm..
    //        now i can use them both but they must have the same freq
    switch(prescaler)
    {
        case 1:
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<4) | (1<<0);
            TCCR1B &= ~(1<<1);
            TCCR1B &= ~(1<<2);
            break;
        case 8:
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<4) | (1<<1);
            TCCR1B &= ~(1<<0);
            TCCR1B &= ~(1<<2);
            break;
        case 64:
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<4) | (1<<0) | (1<<1);
            TCCR1B &= ~(1<<2);
            break;
        case 256:
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<4) | (1<<2);
            TCCR1B &= ~(1<<0);
            TCCR1B &= ~(1<<1);
            break;
        case 1024:
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<4) | (1<<0) | (1<<2);
            TCCR1B &= ~(1<<1);
            break;
        default:
            TCCR1A |= (1<<1) | (1<<7);
            TCCR1B |= (1<<4) | (1<<0);
            TCCR1B &= ~(1<<1);
            TCCR1B &= ~(1<<2);
            break;
    }

    // pd5 must be set output ( oc0 )
    DDRD |= (1<<PWM1A);
    // init TCNT0 to 0
    TCNT1H = 0;
    TCNT1L = 0;
}

// it is fat better to make the duty cycle input a float for better accuracy
void phasePWM1A_duty(float duty)
{
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR1A |= (1<<7);
    // duty cycle = OCR1x / top
    if(duty <= 0)   phasePWM1A_stop();
    else    OCR1A = (uint16_t)((float)((float)(duty * (float)ICR1) / 100));
    // old : the uint32_t cast is a must here as : duty * ICR1 value cant be stored in 16 bits
    // new : made it float for better accuracy
}

void phasePWM1A_stop(void)
{
    // notice this way you stop timer 0 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR0 = 0;

    // this way you dont stop the timer so you wont need to reinit
    OCR1A = 0;
    TCNT1 = 0;
    // oc1A pin diconnected, normal port operation
    TCCR1A &= ~(1<<7);
    TCCR1A &= ~(1<<6);
}

/**************************************************************************/
// timer 1
// OC1B

//FIXME : make the freq float
void phasePWM1B_init(uint32_t freq)
{
    // control OC1A only
    // this routine works with non-inverted mode only
    // and using ICR1 for top value only
    uint16_t prescaler = 0;
    float ffreq = 0.0;
    /* freq can be
            top = 0xff  --> f = 31372.5 : 30.6 hz       with specific values depending on pre
            top = 0x1ff --> f = 15655.57 : 15.3 hz  with specific values depending on pre
            top = 0x3ff --> f = 7820.13 : 7.6 hz    with specific values depending on pre
            top = ICR1 || OCR1A
                    pre = 1    -->  f = 8 M    : 122  hz
                    pre = 1024 -->  f = 7812.5 : 0.119 hz
    */
    // freq = F_CPU / (2 * Prescaler * top)

    // set ICR1 with the required value for that freq.
    if(freq == 0){
        prescaler = 1024;
        ffreq = 0.119;

        ICR1 = (uint16_t)((float)((float)F_CPU / ffreq) / (2 * prescaler));
    }
    else
    {
        // the freq can be changed to F_CPU / const for each case
        // but that will make the code even larger...so for now it works with
        // only 16M and for other freq it requires some tweaking
        if(freq >= 122 && freq <= 8000000)  prescaler = 1;
        else if(freq >= 15 && freq <= 1000000)  prescaler = 8;
        else if(freq >= 2 && freq <= 125000)    prescaler = 64;
        else if(freq >= 0 && freq <= 31250) prescaler = 256;
        else if(freq >= 0 && freq <= 7812)  prescaler = 1024;

        ICR1 = (uint16_t)((uint32_t)((uint32_t)F_CPU / freq) / (2 * prescaler));
    }

    // set the prescaler in the register & mode 10
    // edit : made it using bitwise instead of TCCR1A = (1<<1 | (1<<7)
    //        that disabled me from using both OC1A & OC1B atm..
    //        now i can use them both but they must have the same freq
    switch(prescaler)
    {
        case 1:
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<4) | (1<<0);
            TCCR1B &= ~(1<<1);
            TCCR1B &= ~(1<<2);
            break;
        case 8:
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<4) | (1<<1);
            TCCR1B &= ~(1<<0);
            TCCR1B &= ~(1<<2);
            break;
        case 64:
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<4) | (1<<0) | (1<<1);
            TCCR1B &= ~(1<<2);
            break;
        case 256:
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<4) | (1<<2);
            TCCR1B &= ~(1<<0);
            TCCR1B &= ~(1<<1);
            break;
        case 1024:
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<4) | (1<<0) | (1<<2);
            TCCR1B &= ~(1<<1);
            break;
        default:
            TCCR1A |= (1<<1) | (1<<5);
            TCCR1B |= (1<<4) | (1<<0);
            TCCR1B &= ~(1<<1);
            TCCR1B &= ~(1<<2);
            break;
    }

    // pd5 must be set output ( oc0 )
    DDRD |= (1<<PWM1B);
    // init TCNT0 to 0
    TCNT1H = 0;
    TCNT1L = 0;
}

// it is far better to make the duty cycle input a float for better accuracy
void phasePWM1B_duty(float duty)
{
    // so when pwmstop occurs it can call back pwm function without init again
    TCCR1B |= (1<<5);
    // duty cycle = OCR1x / top
    if(duty <= 0)   phasePWM1B_stop();
    else    OCR1B = (uint16_t)((float)((float)(duty * (float)ICR1) / 100));
    // old : the uint32_t cast is a must here as : duty * ICR1 value cant be stored in 16 bits
    // new : made it float for better accuracy
}

void phasePWM1B_stop(void)
{
    // notice this way you stop timer 0 from all opertaions and when you come back you have to reinitialize the timer again
    //TCCR0 = 0;

    // this way you dont stop the timer so you wont need to reinit
    OCR1B = 0;
    TCNT1 = 0;
    // oc1B pin diconnected, normal port operation
    TCCR1A &= ~(1<<5);
    TCCR1A &= ~(1<<4);
}
