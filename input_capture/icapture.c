#include "icapture.h"


#ifdef __PER_INT
volatile static uint16_t g_u16Temp2 = 0;
volatile static uint16_t g_u16Temp1 = 0;
volatile uint16_t g_u16CapValue = 0;
volatile static uint8_t g_u8Counter = 0;
volatile uint8_t g_u8Flag = 0;
//char register volatile value asm("r16");
#endif

uint16_t g_u16Prescaler = 0;
static volatile uint8_t g_u8PWMFlag = 0;
static volatile uint8_t g_u8AutoFlag = 0;


// MOD: added auto range functionality to the end of the init function for unkown freq (0 for unknown)
// MOD: changed the starting prescaler for unknown freq to 64 so it wont mess up if the freq is too low or
// not so high as it can measure up to 250KHz and in autorange routine it will auto correct it to a better
// prescaler
void inputCapture_init(const uint32_t maxPeriod, const uint8_t noiseCanceler, const uint8_t edge, const uint8_t pwmFlag)
{
    // 1 represent rising edge, 0 respresnts falling edge
    // maxPeriod is in micro seconds, if unknown put = 0

    // pwmFlag = 0 ---> normal wave
    // pwmFlag = 1 ---> pwm
    g_u8PWMFlag = pwmFlag;

    // AUTOFlag = 0 --> no auto scale
    // AUTOFlag = 1 --> auto scaled
    g_u8AutoFlag = (uint8_t) !maxPeriod;

    // removed but kept for reference
    // expectedg_u16Prescaler can be :
    //                                          0 --> unknown wave freq      starts with [250KHz : 3Hz] and auto ranges
    //                                          1 --> high freq                      16MHz : 244Hz
    //                                          2 --> med-high freq              2Mhz : 30Hz
    //                                          3 --> medium freq                    250KHz : 3Hz
    //                                          4 --> medium-low freq            62.5KHz : 0.95Hz
    //                                          5 --> low freq                       15.5KHz : 0.23Hz

    if(maxPeriod == 0)  g_u16Prescaler = 8;
    else if((float)maxPeriod <= (float)(1.0f*(float)(1.0f/(F_CPU)))*65536.0f*1000000.0f){
        g_u16Prescaler = 1;
    }else if((float)maxPeriod <= (float)(8.0f*(float)(1.0/(F_CPU)))*65536.0f*1000000.0f){
        g_u16Prescaler = 8;
    }else if((float)maxPeriod <= (float)(64.0f*(float)(1.0f/(F_CPU)))*65536.0f*1000000.0f){
        g_u16Prescaler = 64;
    }else if((float)maxPeriod <= (float)(256.0f*(float)(1.0f/(F_CPU)))*65536.0f*1000000.0f){
        g_u16Prescaler = 256;
    }else if((float)maxPeriod <= (float)(1024.0f*(float)(1.0f/(F_CPU)))*65536.0f*1000000.0f){
        g_u16Prescaler = 1024;
    }else{
     g_u16Prescaler = 8;
    }

    // works in normal mode by default
    TCNT1 = 0;
    TCCR1A = 0;
    if(noiseCanceler)   TCCR1B |= (1<<7);
    else TCCR1B &= ~(1<<7);
    if(edge)    TCCR1B |= (1<<6);
    else if(!edge)  TCCR1B &= ~(1<<6);
    else TCCR1B |= (1<<6);

    #ifdef INTE
    sei();
    #endif

    #ifdef __PER_INT
    TIMSK |= (1<<5); 
    #endif

    #ifndef __PER_INT
    TIMSK &= ~(1<<5);
    #endif

    switch(g_u16Prescaler)
    {
        case 1:
            TCCR1B |= (1<<0);
            break;
        case 8:
            TCCR1B |= (1<<1);
            break;
        case 64:
            TCCR1B |= (1<<0) | (1<<1);
            break;
        case 256:
            TCCR1B |= (1<<2);
            break;
        case 1024:
            TCCR1B |= (1<<0) | (1<<2);
            break;
        default:
            break;
    }
}
/***********************************************************************/
/***********************************************************************/
// MOD: modified this api to return the calculated period + 20% 
// calculates the maxperiod in micro seconds for u
uint32_t inputCapture_calculateMaxPeriod(uint32_t freq)
{
    uint32_t period;
    period = (uint32_t)((float)(1.0f/freq) * 1000000.0f);
    period = (uint32_t)((float)period + (20.0f/100.0f) * (float)period);
    return period;
}

// returns the period in micro seconds for non-pwm
// returns the pulse in micro seconds for pwm
static float inputCapture_Period(void)
{
    // FIXME: check the overflow for acurate results
    #if defined (__PER_INT) && defined (__HI_FREQ)
    float calculatedPeriod = 0.0;
    uint16_t buffer[2] = {0};
    while(g_u8Counter < 2){
        if(g_u8Flag){
            g_u16CapValue = g_u16Temp2 - g_u16Temp1;
            buffer[g_u8Counter++] = g_u16CapValue;
            g_u8Flag = 0;
            if(g_u8Counter == 2){
                g_u8Counter = 0;
                // TEST: is buffer[1] == buffer[0] does something wrond??
                if(abs(buffer[1] - buffer[0]) == 0 || abs(buffer[0] - buffer[1]) == 0){
                    // again this casting is so VI that it messed up testing without it
                    calculatedPeriod = (float)g_u16CapValue * g_u16Prescaler * (float)(1.0f/(float)F_CPU) * 1000000.0f;
                    return calculatedPeriod;
                }
                else return 0;
            }
        }
    }
    #endif

    // FIXME: add overflow check
    #if defined (__PER_INT) && defined (__LO_FREQ)
    static float calculatedPeriod = 0.0;
    if(g_u8Flag == 2){
        if(g_u16Temp2 > g_u16Temp1)     // NO overflow ignoring if the overflow happened and counted to a higher value
        {
            g_u16CapValue = g_u16Temp2 - g_u16Temp1;
            g_u8Flag = 0;
            // again this casting is so VI that it messed up testing without it
            calculatedPeriod = (float)g_u16CapValue * g_u16Prescaler * (float)(1.0f/(float)F_CPU) * 1000000.0f;
            return calculatedPeriod;
        }
        else
        {
            g_u16CapValue = (65536 - g_u16Temp1) + g_u16Temp2;
            g_u8Flag = 0;
            calculatedPeriod = (float)g_u16CapValue * g_u16Prescaler * (float)(1.0f/(float)F_CPU) * 1000000.0f;
            return calculatedPeriod;
        }
    }
    else
    {
        // to not show 0 on the lcd ever
        return calculatedPeriod;
    }
    #endif

    #ifndef __PER_INT
    if(g_u8PWMFlag == 0)            // measure the period
    {
        uint16_t value;
        uint16_t temp;
        float calculatedPeriod;

        TIFR |= (1<<5);
        while((TIFR & (1<<5))==0);
        temp = ICR1;
        TIFR |= (1<<5);
        while((TIFR & (1<<5))==0);
        value = ICR1;
        //TCCR1B = 0;           // stop the timer
        
        if(value > temp)
        {
            value = value - temp;
        }
        else
        {
            value = (65536 - temp) + value;
        }
        calculatedPeriod = (float)value * g_u16Prescaler * (float)(1.0f/(float)F_CPU) * 1000000.0f;
        return calculatedPeriod;
    }
    else if(g_u8PWMFlag == 1)                           // measure the pulse width
    {
        uint16_t value;
        uint16_t temp;
        float calculatedPeriod;

        TIFR |= (1<<5);
        while((TIFR & (1<<5))==0);
        temp = ICR1;
        TIFR |= (1<<5);
        TCCR1B ^= (1<<6);
        while((TIFR & (1<<5))==0);
        value = ICR1;
        TIFR |= (1<<5);
        TCCR1B ^= (1<<6);
        //TCCR1B = 0;           // stop the timer
        if(value > temp)
        {
            value = value - temp;
        }
        else
        {
            value = (65536 - temp) + value;
        }
        calculatedPeriod = (float)value * g_u16Prescaler * (float)(1.0/F_CPU) * 1000000.0f;
        return calculatedPeriod;
    }
    #endif
}

/***********************************************************************/
/***********************************************************************/
// returns the freq
static uint32_t inputCapture_Freq(void)
{
    // FIXME: add overflow check
    #if defined (__PER_INT) && defined (__HI_FREQ)
    float calculatedPeriod = 0.0;
    uint32_t calculatedFreq = 0;
    uint16_t buffer[2] = {0};
    while(g_u8Counter < 2){
        if(g_u8Flag){
            g_u16CapValue = g_u16Temp2 - g_u16Temp1;
            buffer[g_u8Counter++] = g_u16CapValue;
            g_u8Flag = 0;
            if(g_u8Counter == 2){
                g_u8Counter = 0;
                if(abs(buffer[1] - buffer[0]) == 0 || abs(buffer[0] - buffer[1]) == 0){
                    // again this casting is so VI that it messed up testing without it
                    calculatedPeriod = (float)g_u16CapValue * g_u16Prescaler * (float)(1.0f/(float)F_CPU);
                    calculatedFreq = 1.0f / (float)calculatedPeriod;
                    return calculatedFreq;
                }
                else return 0;
            }
        }
    }
    #endif

    // FIXME: add overflow check
    #if defined (__PER_INT) && defined (__LO_FREQ)
    float calculatedPeriod = 0.0;
    static uint32_t calculatedFreq = 0;
    if(g_u8Flag == 2){
        if(g_u16Temp2 > g_u16Temp1)     // NO overflow ignoring if overflow happened and counted to a higher value
        {
            g_u16CapValue = g_u16Temp2 - g_u16Temp1;
            g_u8Flag = 0;
            // again this casting is so VI that it messed up testing without it
            calculatedPeriod = (float)g_u16CapValue * g_u16Prescaler * (float)(1.0f/(float)F_CPU);
            calculatedFreq = 1.0f / (float)calculatedPeriod;
            return calculatedFreq;
        }
        else
        {
            g_u16CapValue = (65536 - g_u16Temp1) + g_u16Temp2;
            g_u8Flag = 0;
            calculatedPeriod = (float)g_u16CapValue * g_u16Prescaler * (float)(1.0f/(float)F_CPU);
            calculatedFreq = 1.0f / (float)calculatedPeriod;
            return calculatedFreq;
        }
    }
    else
    {
        // to not show 0 on the lcd ever
        return calculatedFreq;
    }
    #endif

    #ifndef __PER_INT
    if(g_u8PWMFlag == 0)
    {
        float __period;
        static uint32_t calculatedFreq;

        __period = inputCapture_Period();
        __period /= 1000000.0f;
        if(__period)    calculatedFreq = 1.0f / (float)__period;
        return calculatedFreq;
    }
    else if(g_u8PWMFlag == 1)
    {
        uint16_t value;
        uint16_t temp;
        float calculatedPeriod;
        static uint32_t calculatedFreq;

        TIFR |= (1<<5);
        while((TIFR & (1<<5))==0);
        temp = ICR1;
        TIFR |= (1<<5);
        while((TIFR & (1<<5))==0);
        value = ICR1;
        TIFR |= (1<<5);
        //TCCR1B = 0;           // stop the timer
        if(value > temp)
        {
            value = value - temp;
        }
        else
        {
            value = (65536 - temp) + value;
        }
        calculatedPeriod = (float)value * g_u16Prescaler * (float)(1.0f/(float)F_CPU);
        if(calculatedPeriod) calculatedFreq = 1.0f / (float)calculatedPeriod;
        return calculatedFreq;
    }
    #endif
}
/***********************************************************************/
/***********************************************************************/
// TODO: add code to handle getting pulse using interrupt
#ifdef __PER_INT
ISR(TIMER1_CAPT_vect)
{
    if(g_u8PWMFlag == 0)
    {
        #if defined (__HI_FREQ)
        // for high freq
        if(g_u8Flag == 0){
            g_u16Temp1 = ICR1;
            //while(TIFR & (1<<5) == 0);        // takes 3/4 clocks which is alot for freq above 1.5Mhz
            asm volatile ("nop\n");     // for freq 2M it is enough 1 clock to set the g_u8Flag so we clear it without having to poll it
            TIFR |= (1<<5);
            g_u16Temp2 = ICR1;
            g_u8Flag = 1;
        }
        #elif defined (__LO_FREQ)
        if(g_u8Flag == 0){
            g_u16Temp1 = ICR1;
            g_u8Flag++;
        }
        else if(g_u8Flag == 1){
            g_u16Temp2 = ICR1;
            g_u8Flag++;
        }
        #endif
    }
    else if(g_u8PWMFlag == 1)
    {
        // TEST: added this part to get the pulse width using interrupt method, 
        // TEST: behaviour when the duty cycle is so low maybe a problem
        #if defined (__HI_FREQ)
        // for high freq
        if(g_u8Flag == 0){
            g_u16Temp1 = ICR1;
            TCCR1B ^= (1<<6);
            while((TIFR & (1<<5))==0);      // takes 3/4 clocks which is alot for freq above 1.5Mhz
            //asm volatile ("nop\n");       // for freq 2M it is enough 1 clock to set the g_u8Flag so we clear it without having to poll it
            TIFR |= (1<<5);
            g_u16Temp2 = ICR1;
            g_u8Flag = 1;
        }
        #elif defined (__LO_FREQ)
        if(g_u8Flag == 0){
            g_u16Temp1 = ICR1;
            TCCR1B ^= (1<<6);
            g_u8Flag++;
        }
        else if(g_u8Flag == 1){
            g_u16Temp2 = ICR1;
            TCCR1B ^= (1<<6);
            g_u8Flag++;
        }
        #endif
    }
}
#endif
/***********************************************************************/
/***********************************************************************/
static void inputCapture_autoFreqRange(uint32_t freq)
{
    uint16_t _prescaler = 0;
    if((uint32_t)freq > 900ul){
        // set prescaler to 1
        TCCR1B = (1<<0);
        TCCR1B |= (1<<6);
        _prescaler = 1;
    }
    else if(((uint32_t)freq <= 900ul) && ((uint32_t)freq > 90ul)){
        // set prescaler to 8
        TCCR1B = (1<<1);
        TCCR1B |= (1<<6);
        _prescaler = 8;
    }else if(((uint32_t)freq <= 90ul) && ((uint32_t)freq > 20ul)){
        // set prescaler to 64
        TCCR1B = (1<<0) | (1<<1);
        TCCR1B |= (1<<6);
        _prescaler = 64;
    }else if(((uint32_t)freq <= 20ul) && ((uint32_t)freq > 3ul)){
        // set prescaler to 256
        TCCR1B = (1<<2);
        TCCR1B |= (1<<6);
        _prescaler = 256;
    }else if((uint32_t)freq <= 3ul){
        // set prescaler to 1024
        TCCR1B = (1<<0) | (1<<2);
        TCCR1B |= (1<<6);
        _prescaler = 1024;
    }
    g_u16Prescaler = _prescaler;
}

static void inputCapture_autoPeriodRange(float _period)
{
    uint16_t _prescaler = 0;
        if((float)_period > (1.0f/900.0f)){
            // set prescaler to 1
            TCCR1B = (1<<0);
            TCCR1B |= (1<<6);
            _prescaler = 1;
        }
        else if(((float)_period <= (1.0f/900.0f)) && ((float)_period > (1.0f/90.0f))){
            // set prescaler to 8
            TCCR1B = (1<<1);
            TCCR1B |= (1<<6);
            _prescaler = 8;
        }else if(((float)_period <= (1.0f/90.0f)) && ((float)_period > (1.0f/20.0f))){
            // set prescaler to 64
            TCCR1B = (1<<0) | (1<<1);
            TCCR1B |= (1<<6);
            _prescaler = 64;
        }else if(((float)_period <= (1.0f/20.0f)) && ((float)_period > (1.0f/3.0f))){
            // set prescaler to 256
            TCCR1B = (1<<2);
            TCCR1B |= (1<<6);
            _prescaler = 256;
        }else if((float)_period <= (1.0f/3.0f)){
            // set prescaler to 1024
            TCCR1B = (1<<0) | (1<<2);
            TCCR1B |= (1<<6);
            _prescaler = 1024;
        }
        g_u16Prescaler = _prescaler;
}


float inputCapture_getPeriod(void)
{
    float __CalPeriod = 0.0;
    if(g_u8AutoFlag)
    {
        __CalPeriod = inputCapture_Period();
        inputCapture_autoPeriodRange(__CalPeriod);
        __CalPeriod = inputCapture_Period();
    }
    else
    {
        __CalPeriod = inputCapture_Period();
    }
    return __CalPeriod;
}

uint32_t inputCapture_getFreq(void)
{   
    uint32_t __Freq = 0;
    if(g_u8AutoFlag)
    {
        __Freq = inputCapture_Freq();
        inputCapture_autoFreqRange(__Freq);
        __Freq = inputCapture_Freq();
    }
    else
    {
        __Freq = inputCapture_Freq();
    }
    return __Freq;
}
