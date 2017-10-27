#include "icapture.h"


#ifdef __PER_INT
volatile static uint16_t g_u16Temp2 = 0;
volatile static uint16_t g_u16Temp1 = 0;
volatile uint16_t g_u16CapValue = 0;
volatile static uint8_t g_u8Counter = 0;
volatile static uint8_t g_u8ovNum = 0;
volatile uint8_t g_u8Flag = 0;
//char register volatile value asm("r16");
#endif

uint16_t g_u16Prescaler = 0;


// MOD: added auto range functionality to the end of the init function for unkown freq (0 for unknown)
// MOD: changed the starting prescaler for unknown freq to 64 so it wont mess up if the freq is too low or
// not so high as it can measure up to 250KHz and in autorange routine it will auto correct it to a better
// prescaler
void inputCapture_init(const uint32_t maxPeriod, const uint8_t noiseCanceler, const uint8_t edge)
{
	// 1 represent rising edge, 0 respresnts falling edge
	// maxPeriod is in micro seconds, if unknown put = 0

	// removed but kept for reference
	// expectedg_u16Prescaler can be :
	//											0 --> unknown wave freq		 starts with [250KHz : 3Hz] and auto ranges
	//											1 --> high freq 					 16MHz : 244Hz
	//											2 --> med-high freq			 	 2Mhz : 30Hz
	//											3 --> medium freq					 250KHz : 3Hz
	// 											4 --> medium-low freq		 	 62.5KHz : 0.95Hz
	//											5 --> low freq						 15.5KHz : 0.23Hz

	if(maxPeriod == 0)	g_u16Prescaler = 64;
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
	 g_u16Prescaler = 1024;
	}

	// works in normal mode by default
	TCNT1 = 0;
	TCCR1A = 0;
	if(noiseCanceler)	TCCR1B |= (1<<7);
	else TCCR1B &= ~(1<<7);
	if(edge)	TCCR1B |= (1<<6);
	else if(!edge)	TCCR1B &= ~(1<<6);
	else TCCR1B |= (1<<6);

	#ifdef INTE
	sei();
	#endif

	#ifdef __PER_INT
	TIMSK |= (1<<5); 
	#endif

	switch(g_u16Prescaler)
	{
		case 1:
			TCCR1B = (1<<0);
			break;
		case 8:
			TCCR1B = (1<<1);
			break;
		case 64:
			TCCR1B = (1<<0) | (1<<1);
			break;
		case 256:
			TCCR1B = (1<<2);
			break;
		case 1024:
			TCCR1B = (1<<0) | (1<<2);
			break;
		default:
			break;
	}
	// MOD: if unknown freq, feed 0 to the init function. it will do init as normal and then will auto scale
	if(maxPeriod == 0){
		g_u16Prescaler = inputCapture_autoRange();
	}
}

// MOD: modified this api to return the calculated period + 20% 
// calculates the maxperiod in micro seconds for u
uint32_t inputCapture_calculateMaxPeriod(uint32_t freq)
{
	uint32_t period;
	period = (uint32_t)((float)(1.0f/freq) * 1000000.0f);
	period = (uint32_t)((float)period + (20.0f/100.0f) * (float)period);
	return period;
}

// returns the time in micro seconds
float inputCapture_getPeriod(void)
{
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

	#if defined (__PER_INT) && defined (__LO_FREQ)
	float calculatedPeriod = 0.0;
	if(g_u8Flag){
		g_u16CapValue = g_u16Temp2 - g_u16Temp1;
		g_u8Flag = 0;
		// again this casting is so VI that it messed up testing without it
		calculatedPeriod = (float)g_u16CapValue * g_u16Prescaler * (float)(1.0f/(float)F_CPU) * 1000000.0f;
		return calculatedPeriod;
	}
	else return 0;
	
	#endif

	#ifndef __PER_INT
	TIMSK &= ~(1<<5);
	TIFR |= (1<<5);
	uint16_t temp;
	uint16_t value;
	float calculatedPeriod;

	while((TIFR & (1<<5))==0);
	temp = ICR1;
	TIFR |= (1<<5);
	while((TIFR & (1<<5))==0);
	value = ICR1;
	TIFR |= (1<<5);
	// dont just stop the timer as it maybe used by another operation in compare/normal or pwm mode
	//TCCR1B = 0;			// stop the timer
	value = value - temp;
	calculatedPeriod = (float)value * g_u16Prescaler * (float)(1.0/F_CPU) * 1000000.0f;
	return calculatedPeriod;
	#endif
}

#if defined(__CAPTURE_GET_PERIOD)
// returns the freq
uint32_t inputCapture_getFreq(void)
{
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

	#if defined (__PER_INT) && defined (__LO_FREQ)
	float calculatedPeriod = 0.0;
	uint32_t calculatedFreq = 0;
	if(g_u8Flag){
		g_u16CapValue = g_u16Temp2 - g_u16Temp1;
		g_u8Flag = 0;
		// again this casting is so VI that it messed up testing without it
		calculatedPeriod = (float)g_u16CapValue * g_u16Prescaler * (float)(1.0f/(float)F_CPU);
		calculatedFreq = 1.0f / (float)calculatedPeriod;
		return calculatedFreq;
	}
	else return 0;
	
	#endif

	#ifndef __PER_INT
	TIMSK &= ~(1<<5);
	TIFR |= (1<<5);
	uint16_t temp;
	uint16_t freq;
	float calculatedPeriod;
	uint32_t calculatedFreq;

	while((TIFR & (1<<5))==0);
	temp = ICR1;
	TIFR |= (1<<5);
	while((TIFR & (1<<5))==0);
	freq = ICR1;
	TIFR |= (1<<5);
	// dont just stop the timer as it maybe operating elsewhere in compare/normal or pwm mode
	//TCCR1B = 0;			// stop the timer
	freq = freq - temp;

	if(freq){
		// again this casting is so VI that it messed up testing without it
		calculatedPeriod = (float)freq * g_u16Prescaler * (float)(1.0f/(float)F_CPU);
		calculatedFreq = 1.0f / (float)calculatedPeriod;
	}else{
		return 0;
	}
	return calculatedFreq;
	#endif
}
#endif

// TODO: add code to handle getting pulse using interrupt
#ifdef __PER_INT
ISR(TIMER1_CAPT_vect)
{
	#if defined (__CAPTURE_GET_PERIOD)
	#if defined (__HI_FREQ)
	// for high freq
	if(g_u8Flag == 0){
		g_u16Temp1 = ICR1;
		//while(TIFR & (1<<5) == 0);		// takes 3/4 clocks which is alot for freq above 1.5Mhz
		asm volatile ("nop\n");		// for freq 2M it is enough 1 clock to set the g_u8Flag so we clear it without having to poll it
		TIFR |= (1<<5);
		g_u16Temp2 = ICR1;
		g_u8Flag = 1;
	}
	#elif defined (__LO_FREQ)
	// add code for low freq that doesnt need to wait and poll the tif g_u8Flag
	if(g_u8Flag == 0){
		g_u16Temp1 = ICR1;
		while((TIFR & (1<<5))==0);		// takes 3/4 clocks which is alot for freq above 1.5Mhz
		//asm volatile ("nop\n");		// for freq 2M it is enough 1 clock to set the g_u8Flag so we clear it without having to poll it
		TIFR |= (1<<5);
		g_u16Temp2 = ICR1;
		g_u8Flag = 1;
	}
	#endif
	// TEST: added this part to get the pulse width using interrupt method, 
	// TEST: behaviour when the duty cycle is so low maybe a problem
	#elif defined (__CAPTURE_GET_PULSE)
	if(g_u8Flag == 0){
		g_u16Temp1 = ICR1;
		TCCR1B ^= (1<<6);
		while((TIFR & (1<<5))==0);		// takes 3/4 clocks which is alot for freq above 1.5Mhz
		//asm volatile ("nop\n");		// for freq 2M it is enough 1 clock to set the g_u8Flag so we clear it without having to poll it
		TIFR |= (1<<5);
		g_u16Temp2 = ICR1;
		g_u8Flag = 1;
	}
	#endif
}

#endif

#ifndef __PER_INT
uint32_t inputCapture_getPeriodClocks(void)
{
	TIMSK &= ~(1<<5);
	uint32_t value;
	uint16_t temp;
	while((TIFR & (1<<5))==0);
	temp = ICR1;
	TIFR |= (1<<5);
	while((TIFR & (1<<5))==0);
	value = ICR1;
	TIFR |= (1<<5);
	TCCR1B = 0;			// stop the timer
	value = value - temp;
	return value;
}

uint32_t inputCapture_getPulseClocks(void)
{
	TIMSK &= ~(1<<5);
	uint32_t value;
	uint16_t temp;
	while((TIFR & (1<<5))==0);
	temp = ICR1;
	TIFR |= (1<<5);
	TCCR1B ^= (1<<6);
	while((TIFR & (1<<5))==0);
	value = ICR1 - temp;
	TIFR |= (1<<5);
	TCCR1B = 0;			// stop the timer
	return value;
}
#endif

// must be used after inputCapture_init()
uint16_t inputCapture_autoRange(void)
{
	uint32_t freq = 0;
	uint16_t _prescaler = 0;
	freq = inputCapture_getFreq();
	if(freq > 200000){
		// set prescaler to 1
		TCCR1B = (1<<0);
		_prescaler = 1;
	}
	else if(freq <= 200000 && freq > 100000){
		// set prescaler to 8
		TCCR1B = (1<<1);
		_prescaler = 8;
	}else if(freq <= 100000 && freq > 10000){
		// set prescaler to 64
		TCCR1B = (1<<0) | (1<<1);
		_prescaler = 64;
	}else if(freq <= 10000 && freq > 100){
		// set prescaler to 256
		TCCR1B = (1<<2);
		_prescaler = 256;
	}else if(freq <= 100 && freq > 0){
		// set prescaler to 1024
		TCCR1B = (1<<0) | (1<<2);
		_prescaler = 1024;
	}
	else if(freq == 0)
	{
		// do nothing keep it as it is
		_prescaler = g_u16Prescaler;
	}
	return _prescaler;
}
