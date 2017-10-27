/* uart driver for atmega16/32
* to use the interrupt driven mode (  #define inte  )
* received string max 20 characters

* development (to be done):
	- change how the string is terminated
	- improve the isr
	- the buffer the user is allowed to use is the " gu8Aux_buf " "give it a better name for interfacing"
	- receving the data in the buffer and the aux buffer doesnt interfere, i.e, make the main buffer keep receiving and at the same time data is being transfered to aux buf so the main buffer is always available for data input

* 	NOTES:
	1- USE gu8Command_ready in the source code and reset it when you're done with the string
	2- the string received from the terminal must be newline terminated
	3- the buffer the user is allowed to use is the " gu8Aux_buf " "give it a better name for interfacing"
  	4- to use the interrupt driven mode (  #define INTE  )
 	5- received string max 20 characters
  	6- added sending new line char. at the end of string after sending a terminator 
 	7- added a function to send hex values to be shown as ascii
  	8- added two level buffer (main + aux) in the interrupt reception
*/ 


#ifndef __UART_H__
#define __UART_H__

	#define INTE
	#define TERMINATOR '\n'
	#define EOF	(-1)
	#define F_CPU 16000000UL
	#define MAX_UART_BUF_SIZE 20
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <string.h>
	#include <util/atomic.h>


	volatile uint8_t gu8Buf_size;
	volatile uint8_t gu8Buf_idx;
	volatile uint8_t gu8Main_buf[MAX_UART_BUF_SIZE];
	volatile uint8_t gu8Aux_buf[MAX_UART_BUF_SIZE];
	volatile uint8_t gu8Command_ready;

	void uart_init(uint16_t baud);
	uint8_t uart_send_ready(void);
	uint8_t uart_receive_ready(void);
	void uart_send_u8(uint8_t u8data);
	void uart_sendString(const uint8_t * u8SData);
	void  uart_send_hex(uint8_t u8data);
	void uart_sendString_hex(const uint8_t * u8SData);
	#ifndef INTE
	uint8_t uart_recieve_u8(void);
	uint8_t * uart_recieveString(void);
	#endif
	void copy_command(void);

#endif
