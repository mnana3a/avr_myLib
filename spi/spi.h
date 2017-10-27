// NOTE: if the slave has other tasks to do in case of slave add a way instead of waiting for the master to start comm. use interrupt and set a flag in it.. in the super loop poll that flag to see if there is data use it otherwise move on and dont wait

// NOTE: to add a flow control method to the spi, make the master ss pin as input and when the slave wants to communicate with the master it changes ss which will result in master mode changing into slave so you need to reset it to master again and respond to the slave

// NOTE: keep in mind this chip(atmega32/16) doesn't recommend using the 2 divider for stability issues

#ifndef __SPI_INCLUDED
#define __SPI_INCLUDED

#ifndef F_CPU
    #define F_CPU 16000000UL
#endif
#include <inttypes.h>
#include <avr/io.h>
#include "spi_interface.h"

// NOTE: added for geany proj plug-in to catch the def and go to the file by pressing on
// lcd.h
extern uint8_t spi;

#ifdef INTE
    #include <avr/interrupt.h>
#endif

/*****************PINS*************/
#define B4 4
#define B5 5
#define B6 6
#define B7 7
#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_PIN PINB
#define SPI_SS B4
#define SPI_MOSI B5
#define SPI_MISO B6
#define SPI_SCK B7
#define GARBAGE 0X00
#define OK 0X01
/*****************SPCR*************/
#define SPIE    7
#define SPE     6
#define DORD    5
#define MSTR    4
#define CPOL    3
#define CPHA    2
#define SPR1    1
#define SPR0    0
/*****************SPSR*************/
#define SPIF    7
#define WCOL    6
#define SPI2X   0

#define MAX_SPI_BUF_SIZE 100u
#define MAX_STRING_SIZE 20u
#define NO_DATA 0u

enum _SPI_MODE {SPI_MASTER = 1, SPI_SLAVE = 0};
enum _SPI_PRESCALAR {SPI_FOSC4 = 0b00, SPI_FOSC16 = 0b01, SPI_FOSC64 = 0b10, SPI_FOSC128 = 0b11};
enum _SPI_ENABLE {SPI_ON =1 , SPI_OFF = 0};
enum _SPI_DOUBLE_SPEED {SPI_NORMAL, SPI_DOUBLE};
enum _SPI_POLARITY {SPI_IDLE_LOW, SPI_IDLE_HIGH};
enum _SPI_SAMPLING_EDGE {SPI_SAMPLE_LEADING = 0 , SPI_SAMPLE_TRAILING = 0};
enum _SPI_INT_ENABLE {SPI_INT_OFF = 0, SPI_INT_ON = 1};
enum _SPI_DATA_ORDER {SPI_LSB = 1, SPI_MSB = 0};

struct spi_config
{
    enum _SPI_MODE              _spi_mode;
    enum _SPI_PRESCALAR        _spi_prescaler;
    enum _SPI_ENABLE           _spi_enable;
    enum _SPI_DOUBLE_SPEED     _spi_speed;
    enum _SPI_POLARITY        _spi_polarity;
    enum _SPI_SAMPLING_EDGE   _spi_sampleEdge;
    enum _SPI_INT_ENABLE      _spi_intEnable;
    enum _SPI_DATA_ORDER      _spi_dataOrder;
};

#ifdef INTE
    uint8_t spi_slaveGet(void);
    uint8_t spi_slavePut(uint8_t __sdata);
#endif


void spi_init(void);
void spi_setSpeed(uint8_t speed);
uint8_t spi_transfer(uint8_t _data);
uint8_t spi_checkCollision(void);
void spi_callbackFn(void (*ptrToSPI_ISR)(void));
void spi_clearSS(void);
void spi_setSS(void);

#endif
