
#include "uart.h"

volatile uint8_t gu8Buf_size = 0;
volatile uint8_t gu8Buf_idx = 0;
volatile uint8_t gu8Main_buf[MAX_UART_BUF_SIZE] = {0};
volatile uint8_t gu8Aux_buf[MAX_UART_BUF_SIZE] = {0};
volatile uint8_t gu8Command_ready = 0;

void uart_init(uint16_t baud)
{
	uint16_t baudValue = ((((F_CPU/16UL) + (baud/2)) / (baud)) - 1);
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
	UCSRB |= (1<<7);		// interrupts enabled on receive only
	sei();
	#endif
	//memset( (void *) &gu8Main_buf[0], 0, MAX_UART_BUF_SIZE); 
}

uint8_t uart_send_ready(void)
{
	if(UCSRA & (1<<5))
		return 1;
	else
		return 0;
}

uint8_t uart_receive_ready(void)
{
	if(UCSRA & (1<<7))
		return 1;
	else
		return 0;
}


void uart_send_u8(uint8_t u8data)
{
	while(!(UCSRA & (1<<5)));
	UDR = u8data;
}

void uart_sendString(const uint8_t * u8SData)
{
	while(*u8SData){
		uart_send_u8(*u8SData++);
	}
	uart_send_u8('\0');
	uart_send_u8('\n');
}

void  uart_send_hex(uint8_t u8data)
{
    unsigned char i,temp;
     
    for (i=0; i<2; i++)
    {
        temp = (u8data & 0xF0)>>4;
        if ( temp <= 9)
            uart_send_u8( '0' + temp);
        else
            uart_send_u8(  'A' + temp - 10);
        u8data = u8data << 4;    
     }   
}

void uart_sendString_hex(const uint8_t * u8SData)
{
   unsigned int i=0;
   while (u8SData[i] != '\x0') 
   {
        uart_send_hex(u8SData[i++]);
        uart_send_u8(' ');
    };
}

/******************************************************/

#ifdef INTE
ISR(USART_RXC_vect)
{
	volatile char data;
	
	data = UDR;
	//FIXME: if(gu8Command_ready == 0)
	gu8Main_buf[gu8Buf_idx++] = data;
	//FIXME : add gu8Command_ready == 0 to the test condition
	if(data == TERMINATOR || gu8Buf_idx >= MAX_UART_BUF_SIZE - 1){
		gu8Main_buf[gu8Buf_idx-1] = '\0';
		copy_command();
        // it can be replaced with a queue
		gu8Buf_idx = 0;
		gu8Command_ready = 1;
	}
}

void copy_command(void) 
{
    // The USART might interrupt this - don't let that happen!
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        // Copy the contents of main buff into aux buff
        memcpy( (void *)&gu8Aux_buf[0],(void *) &gu8Main_buf[0], MAX_UART_BUF_SIZE);

        // Now clear main buff, the USART can reuse it now
        // memset((void *) &gu8Main_buf[0], 0, MAX_UART_BUF_SIZE);

        gu8Buf_size = gu8Buf_idx;
    }
}
#else
uint8_t uart_recieve_u8(void)
{
	while(!(UCSRA & (1<<7)));
	return UDR;
}

uint8_t * uart_recieveString(void)
{
	static unsigned char  recievedString[MAX_UART_BUF_SIZE];
	uint8_t i=0;
	while(1){
		*(recievedString + i) = uart_recieve_u8();
		if(*(recievedString+i) == TERMINATOR || i > MAX_UART_BUF_SIZE)
			break;
		else
			i++;
	}
	*(recievedString+i) = '\0';
	return recievedString;
}
#endif

