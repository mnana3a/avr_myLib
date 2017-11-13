#include "uart.h"

volatile static uint8_t g_u8Buf_idx = 0;
volatile static uint8_t g_u8Buf_id0 = 0;
volatile static uint8_t g_u8Queue[MAX_UART_BUF_SIZE] = {0};

void uart_init(uint16_t baud)
{
    uint16_t baudValue = ((((F_CPU >> 4) + (baud >> 1)) / (baud)) - 1);
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
}

uint8_t uart_putReady(void)
{
    if(UCSRA & (1<<5))
        return 1;
    else
        return 0;
}

uint8_t uart_getReady(void)
{
    if(UCSRA & (1<<7))
        return 1;
    else
        return 0;
}


void uart_putChar(uint8_t u8data)
{
    while(!(UCSRA & (1<<5)));
    UDR = u8data;
}

void uart_putString(const uint8_t * u8SData)
{
    while(*u8SData){
        uart_putChar(*u8SData++);
    }
    uart_putChar('\0');
    uart_putChar('\n');
}

void  uart_putChar_hex(uint8_t u8data)
{
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
}

void uart_putString_hex(const uint8_t * u8SData)
{
   unsigned int i=0;
   while (u8SData[i] != '\x0') 
   {
        uart_putChar_hex(u8SData[i++]);
        uart_putChar(' ');
    };
}

/**************************************************************************/

#ifdef INTE
ISR(USART_RXC_vect)
{
    volatile uint8_t data;
    
    data = UDR;

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


uint8_t uart_getChar(void)
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
    while(!(UCSRA & (1<<7)));
    return UDR;
    #endif
}

uint8_t *uart_getString(void)
{
    #ifdef INTE
    static unsigned char recievedString[MAX_STRING_SIZE] = {'\0'};
    uint8_t i=0;

    while((i < MAX_STRING_SIZE) && (*(recievedString+i) = uart_getChar()) != TERMINATOR1 && (*(recievedString+i) != TERMINATOR2))
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
        *(recievedString + i) = uart_getChar();
        if(*(recievedString+i) == TERMINATOR || i > MAX_UART_BUF_SIZE)
            break;
        else
            i++;
    }
    *(recievedString+i) = '\0';
    return recievedString;
    #endif
}


