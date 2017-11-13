// this driver when working as a master doesnt control the ss pin it sets it as output but leaves it to the user to handle it in the code
// so in main(){ // set ss pin;  // start spi comm. }

#ifndef __SPI_INTERFACE
#define __SPI_INTERFACE

extern char spi_interface;

// this can be ( 0 , 1 , 2 , 3 )
#define SPI_MODE_0

// MODE       : MASTER  -  SLAVE
// PRESCALE   : 4  -  16  -  64  -  128 (NOTE: using dx to get 2 prescaler isnt stable)
// ENABLE     : ON  -  OFF
// SPEED      : NORMAL  -  DOUBLE
// POLARITY   : LOW  -  HIGH
// SAMPLE     : LEADING  -  TRAILING
// INT        : ON  -  OFF
// DATA_ORDER : LSB  -  MSB


#define SPI_MODE                SPI_MASTER
#define SPI_PRESCALE            SPI_FOSC16
#define SPI_ENABLE              SPI_ON
#define SPI_SPEED_MODE          SPI_NORMAL
#define SPI_POLARITY            SPI_IDLE_LOW
#define SPI_SAMPLE_EDGE         SPI_SAMPLE_LEADING
#define SPI_INT_ENABLE          SPI_INT_OFF
#define SPI_DATA_ORDER          SPI_MSB

//#define SPI_SS_ACTIVE_LOW
#define SPI_SS_ACTIVE_HIGH

// uncomment this if you will use interrupt while in slave mode
//#define INTE

#if defined(SPI_MODE_0)
    #define SPI_POLARITY            SPI_IDLE_LOW
    #define SPI_SAMPLE_EDGE         SPI_SAMPLE_LEADING
#elif defined(SPI_MODE_1)
    #define SPI_POLARITY            SPI_IDLE_
    #define SPI_SAMPLE_EDGE         SPI_SAMPLE_TRAILING
#elif defined(SPI_MODE_2)
    #define SPI_POLARITY            SPI_IDLE_HIGH
    #define SPI_SAMPLE_EDGE         SPI_SAMPLE_LEADING
#elif defined(SPI_MODE_3)
    #define SPI_POLARITY            SPI_IDLE_HIGH
    #define SPI_SAMPLE_EDGE         SPI_SAMPLE_TRAILING
#endif

#endif
