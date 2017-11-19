#include "uart.h"

volatile static uint8_t g_u8Buf_idx = 0;
volatile static uint8_t g_u8Buf_id0 = 0;
volatile static uint8_t g_u8Queue[MAX_UART_BUF_SIZE] = {0};

#ifdef INTE
    static void uart_queue(uint8_t uartx);
    static void (*UART_ISR)(uint8_t uartx) = uart_queue;
#else
    static void (*UART_ISR)(uint8_t uartx) = (void *)0;
#endif

// wrapper functions for atmega32/16
 #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
void uart_init(uint16_t baud)
{
    uart0_init(baud);
}

uint8_t uart_putReady(void)
{
    return uart0_putReady();
}

uint8_t uart_getReady(void)
{
    return uart0_getReady();
}

void uart_putChar(uint8_t u8data)
{
    uart0_putChar(u8data);
}

void uart_putString(const uint8_t * u8SData)
{
    uart0_putString(u8SData);
}

void  uart_putChar_hex(uint8_t u8data)
{
    uart0_putChar_hex(u8data);
}

void uart_putString_hex(const uint8_t * u8SData)
{
    uart0_putString_hex(u8SData);
}

uint8_t uart_getChar(void)
{
    return uart0_getChar();
}

uint8_t *uart_getString(void)
{
    return uart0_getString();
}
#endif

void uart0_init(uint16_t baud)
{
    uint16_t baudValue = ((((F_CPU >> 4) + (baud >> 1)) / (baud)) - 1);
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
    UCSRC &= ~(1<<7);
    UBRRH = (uint8_t) (baudValue >> 8);
    UBRRL = (uint8_t) baudValue;
    UCSRC |= (1<<7) | (3<<1);
    UCSRB |= (3<<3);        
    //if(mode == 'd'|| mode=='D')
        //UCSRA |= (1<<1);
    //else
        //UCSRA &= ~(1<<1);
    #ifdef INTE
    UCSRB |= (1<<7);        // interrupts enabled on receive only
    sei();
    #endif

    #elif defined(__AVR_ATmega128__)
    UBRR0H = (uint8_t) (baudValue >> 8);
    UBRR0L = (uint8_t) baudValue;
    UCSR0C &= ~(1<<7);
    UCSR0C |= (3<<1);
    UCSR0B |= (3<<3);
    #ifdef INTE
    UCSR0B |= (1<<7);        // interrupts enabled on receive only
    sei();
    #endif

    #endif
}

#if defined(__AVR_ATmega128__)
void uart1_init(uint16_t baud)
{
    uint16_t baudValue = ((((F_CPU >> 4) + (baud >> 1)) / (baud)) - 1);
    UBRR1H = (uint8_t) (baudValue >> 8);
    UBRR1L = (uint8_t) baudValue;
    UCSR1C &= ~(1<<7);
    UCSR1C |= (3<<1);
    UCSR1B |= (3<<3);
    #ifdef INTE
    UCSR1B |= (1<<7);        // interrupts enabled on receive only
    sei();
    #endif
}
#endif


uint8_t uart0_putReady(void)
{
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
    if(UCSRA & (1<<5))
        return 1;
    else
        return 0;
    #elif defined(__AVR_ATmega128__)
    if(UCSR0A & (1<<5))
        return 1;
    else
        return 0;
    #endif
}

#if defined(__AVR_ATmega128__)
uint8_t uart1_putReady(void)
{
    if(UCSR1A & (1<<5))
        return 1;
    else
        return 0;
}
#endif


uint8_t uart0_getReady(void)
{
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
    if(UCSRA & (1<<7))
        return 1;
    else
        return 0;
    #elif defined(__AVR_ATmega128__)
    if(UCSR0A & (1<<7))
        return 1;
    else
        return 0;
    #endif
}

#if defined(__AVR_ATmega128__)
uint8_t uart1_getReady(void)
{
    if(UCSR1A & (1<<7))
        return 1;
    else
        return 0;
}
#endif

void uart0_putChar(uint8_t u8data)
{
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
    while(!(UCSRA & (1<<5)));
    UDR = u8data;
    #elif defined(__AVR_ATmega128__)
    while(!(UCSR0A & (1<<5)));
    UDR0 = u8data;
    #endif
}

#if defined(__AVR_ATmega128__)
void uart1_putChar(uint8_t u8data)
{
    while(!(UCSR1A & (1<<5)));
    UDR1 = u8data;
}
#endif

void uart0_putString(const uint8_t * u8SData)
{
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
    while(*u8SData){
        uart_putChar(*u8SData++);
    }
    uart_putChar('\0');
    uart_putChar('\n');
    #elif defined(__AVR_ATmega128__)
    while(*u8SData){
        uart0_putChar(*u8SData++);
    }
    uart0_putChar('\0');
    uart0_putChar('\n');
    #endif
}

#if defined(__AVR_ATmega128__)
void uart1_putString(const uint8_t * u8SData)
{
    while(*u8SData){
        uart1_putChar(*u8SData++);
    }
    uart1_putChar('\0');
    uart1_putChar('\n');
}
#endif

void  uart0_putChar_hex(uint8_t u8data)
{
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
    unsigned char i,temp;
     
    for (i=0; i<2; i++)
    {
        temp = (u8data & 0xF0)>>4;
        if ( temp <= 9)
            uart_putChar( '0' + temp);
        else
            uart_putChar(  'A' + temp - 10);
        u8data = u8data << 4;    
    }
    #elif defined(__AVR_ATmega128__)
    unsigned char i,temp;
     
    for (i=0; i<2; i++)
    {
        temp = (u8data & 0xF0)>>4;
        if ( temp <= 9)
            uart0_putChar( '0' + temp);
        else
            uart0_putChar(  'A' + temp - 10);
        u8data = u8data << 4;    
    }
    #endif
}

#if defined(__AVR_ATmega128__)
void  uart1_putChar_hex(uint8_t u8data)
{
    unsigned char i,temp;
     
    for (i=0; i<2; i++)
    {
        temp = (u8data & 0xF0)>>4;
        if ( temp <= 9)
            uart1_putChar( '0' + temp);
        else
            uart1_putChar(  'A' + temp - 10);
        u8data = u8data << 4;    
    }
}
#endif

void uart0_putString_hex(const uint8_t * u8SData)
{
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
    unsigned int i=0;
    while (u8SData[i] != '\x0') 
    {
        uart_putChar_hex(u8SData[i++]);
        uart_putChar(' ');
    }
    #elif defined(__AVR_ATmega128__)
    unsigned int i=0;
    while (u8SData[i] != '\x0') 
    {
        uart0_putChar_hex(u8SData[i++]);
        uart0_putChar(' ');
    }
    #endif
}

#if defined(__AVR_ATmega128__)
void uart1_putString_hex(const uint8_t * u8SData)
{
    unsigned int i=0;
    while (u8SData[i] != '\x0') 
    {
        uart1_putChar_hex(u8SData[i++]);
        uart1_putChar(' ');
    }
}
#endif

/**************************************************************************/

#ifdef INTE
#if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
ISR(USART_RXC_vect)
{
    UART_ISR(0);
}

#elif defined(__AVR_ATmega128__)
ISR(USART0_RX_vect)
{
    UART_ISR(0);
}
ISR(USART1_RX_vect)
{
    UART_ISR(1);
}
#endif
#endif

#ifdef INTE
static void uart_queue(uint8_t uartx)
{
    volatile uint8_t data;
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
    data = UDR;
    #elif defined(__AVR_ATmega128__)
    if (uartx == 0) data = UDR0;
    else if (uartx == 1) data = UDR1;
    #endif

    if((uint16_t)g_u8Buf_idx < MAX_UART_BUF_SIZE)
    {
        if(g_u8Buf_idx >= g_u8Buf_id0)
        {
            g_u8Queue[g_u8Buf_idx] = data;
            g_u8Buf_idx++;
        }
        else if(g_u8Buf_idx < g_u8Buf_id0)
        {
            if((g_u8Buf_id0 - g_u8Buf_idx) > 1 )
            {
                g_u8Queue[g_u8Buf_idx] = data;
                g_u8Buf_idx++;
            }   
            else
            {
                //#error "QUEUE OVERFLOW"
                // add a separate buffer to copy the current queue into and start receving data into this queue again
            }
        }
    }
    else
    {
        if(g_u8Buf_id0 > 1)
        {
            g_u8Buf_idx = 0;
            g_u8Queue[g_u8Buf_idx++] = data;
        }
        else
        {
            //#error "QUEUE OVERFLOW"
            // add a separate buffer to copy the current queue into and start receving data into this queue again
        }
    }
}
#endif

uint8_t uart0_getChar(void)
{
    #ifdef INTE
    uint8_t __tmp = 0;

    if(g_u8Buf_id0 == g_u8Buf_idx)  return NO_DATA;
    if((uint16_t)g_u8Buf_id0 < MAX_UART_BUF_SIZE)
    {
        __tmp = g_u8Queue[g_u8Buf_id0];
        g_u8Buf_id0++;
    }
    else if(g_u8Buf_id0 >= MAX_UART_BUF_SIZE)
    {
        g_u8Buf_id0 = 0;
        __tmp = g_u8Queue[g_u8Buf_id0];
        g_u8Buf_id0++;
    }
    return __tmp;
    #endif

    #ifndef INTE
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
        while(!(UCSRA & (1<<7)));
        return UDR;
    #elif defined(__AVR_ATmega128__)
        while(!(UCSR0A & (1<<7)));
        return UDR0;
    #endif
    #endif
}

#if defined(__AVR_ATmega128__)
uint8_t uart1_getChar(void)
{
    #ifdef INTE
    uint8_t __tmp = 0;

    if(g_u8Buf_id0 == g_u8Buf_idx)  return NO_DATA;
    if((uint16_t)g_u8Buf_id0 < MAX_UART_BUF_SIZE)
    {
        __tmp = g_u8Queue[g_u8Buf_id0];
        g_u8Buf_id0++;
    }
    else if(g_u8Buf_id0 >= MAX_UART_BUF_SIZE)
    {
        g_u8Buf_id0 = 0;
        __tmp = g_u8Queue[g_u8Buf_id0];
        g_u8Buf_id0++;
    }
    return __tmp;
    #endif

    #ifndef INTE
    while(!(UCSR1A & (1<<7)));
    return UDR1;
    #endif
}
#endif

uint8_t *uart0_getString(void)
{
    #ifdef INTE
    static unsigned char recievedString[MAX_STRING_SIZE] = {'\0'};
    uint8_t i=0;

    while((i < MAX_STRING_SIZE) && (*(recievedString+i) = uart0_getChar()) != TERMINATOR1 && (*(recievedString+i) != TERMINATOR2))
    {
        i++;
    }
    *(recievedString+i) = '\0';
    return recievedString;
    #endif

    #ifndef INTE
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
        static unsigned char  recievedString[MAX_UART_BUF_SIZE];
        uint8_t i=0;
        while(1){
            *(recievedString + i) = uart_getChar();
            if(*(recievedString+i) == TERMINATOR1 || *(recievedString+i) == TERMINATOR2 || i > MAX_UART_BUF_SIZE)
                break;
            else
                i++;
        }
        *(recievedString+i) = '\0';
        return recievedString;
    #elif defined(__AVR_ATmega128__)
        static unsigned char  recievedString[MAX_UART_BUF_SIZE];
        uint8_t i=0;
        while(1){
            *(recievedString + i) = uart0_getChar();
            if(*(recievedString+i) == TERMINATOR1 || *(recievedString+i) == TERMINATOR2 || i > MAX_UART_BUF_SIZE)
                break;
            else
                i++;
        }
        *(recievedString+i) = '\0';
        return recievedString;
    #endif
    #endif
}

#if defined(__AVR_ATmega128__)
uint8_t *uart1_getString(void)
{
    #ifdef INTE
    static unsigned char recievedString[MAX_STRING_SIZE] = {'\0'};
    uint8_t i=0;

    while((i < MAX_STRING_SIZE) && (*(recievedString+i) = uart1_getChar()) != TERMINATOR1 && (*(recievedString+i) != TERMINATOR2))
    {
        i++;
    }
    *(recievedString+i) = '\0';
    return recievedString;
    #endif

    #ifndef INTE
        static unsigned char  recievedString[MAX_UART_BUF_SIZE];
        uint8_t i=0;
        while(1){
            *(recievedString + i) = uart1_getChar();
            if(*(recievedString+i) == TERMINATOR1 || *(recievedString+i) == TERMINATOR2 || i > MAX_UART_BUF_SIZE)
                break;
            else
                i++;
        }
        *(recievedString+i) = '\0';
        return recievedString;
    #endif
}
#endif

// use it at run time to run a user-defined function when transmission compeletes by interrupt
void uart_callbackFn(void (*ptrToUART_ISR)(uint8_t uartx))
{
    UART_ISR = ptrToUART_ISR;
}

