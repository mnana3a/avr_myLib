// NOTE: when using getchar() check immedialtly for valid data and use it to set a flag to be used in another location
/* uart driver for atmega16/32

* TODO:
    -  add a feature that allows the use to use backspace character to delete the previous entered character from the queue
    * Editing characters:
 *
 * . \b (BS) or \177 (DEL) delete the previous character
 * . ^u kills the entire input buffer
 * . ^w deletes the previous word
 * . ^r sends a CR, and then reprints the buffer
 * . \t will be replaced by a single space

*   NOTES:
    1- the string received from the terminal must be newline terminated
    2- to use the interrupt driven mode (  #define INTE  )
    3- received string max 20 characters
    4- received characters without reading the queue is 100 characters max
    5- added sending new line char. at the end of string after sending a terminator 
    6- added a function to send hex values to be shown as ascii
    7- changed the receiving buffer from a simple array to a queue
*/ 

// NOTE: using uart_getChar() reads and cleas one char per called time... so if its task is executed each 30 msec and another lcd task each 200 msec that displayes this char you wont get anything on lcd as data is cleared from the queue before you display any of them
// NOTE: so to display the correct data on lcd the uart task time >= lcd time

// TODO: for future add a second queue for when the first queue cant take more
// TODO: when the queue is full and cant receive more characters send a sepcial character to the terminal to specify that the input queue is full:
/*      If the buffer
 * is full (i. e., at RX_BUFSIZE-1 characters in order to keep space for
 * the trailing \n), any further input attempts will send a \a to
 * uart_putchar() (BEL character), although line editing is still
 * allowed.
 * if (c == '\a')
    {
      fputs("*ring*\n", stderr);
      return 0;
    }
*/


#ifndef __UART_H__
#define __UART_H__

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <string.h>
//#include <util/atomic.h>

// NOTE: added for geany proj plug-in to catch the def and go to the file by pressing on
// uart.h
extern uint8_t uart;

//#define INTE

#define TERMINATOR1 '\n'
#define TERMINATOR2 '\r'
#define EOF (-1)
#ifndef F_CPU
    #define F_CPU 16000000UL
#endif
#define MAX_UART_BUF_SIZE 100u
#define MAX_STRING_SIZE 20u
#define NO_DATA 0u


void uart_init(uint16_t baud);
void uart0_init(uint16_t baud);

uint8_t uart_putReady(void);
uint8_t uart0_putReady(void);

uint8_t uart_getReady(void);
uint8_t uart0_getReady(void);

void uart_putChar(uint8_t u8data);
void uart0_putChar(uint8_t u8data);

void uart_putString(const uint8_t * u8SData);
void uart0_putString(const uint8_t * u8SData);

void  uart_putChar_hex(uint8_t u8data);
void  uart0_putChar_hex(uint8_t u8data);

void uart_putString_hex(const uint8_t * u8SData);
void uart0_putString_hex(const uint8_t * u8SData);

uint8_t uart_getChar(void);
uint8_t uart0_getChar(void);

uint8_t *uart_getString(void);
uint8_t *uart0_getString(void);

void uart_callbackFn(void (*ptrToUART_ISR)(uint8_t uartx));

#if defined(__AVR_ATmega128__)
void uart1_init(uint16_t baud);
uint8_t uart1_putReady(void);
uint8_t uart1_getReady(void);
void uart1_putChar(uint8_t u8data);
void uart1_putString(const uint8_t * u8SData);
void  uart1_putChar_hex(uint8_t u8data);
void uart1_putString_hex(const uint8_t * u8SData);
uint8_t uart1_getChar(void);
uint8_t *uart1_getString(void);
#endif

#endif
