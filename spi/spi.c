// TODO: add isr code with a queue or buffer for incoming strings

#include "spi.h"
#include "spi_interface.h"

volatile static uint8_t g_u8Buf_idx = 0;
volatile static uint8_t g_u8Buf_id0 = 0;
volatile static uint8_t g_u8Queue[MAX_SPI_BUF_SIZE] = {0};

#ifdef INTE
    static void spi_queue(void);
    static void (*SPI_ISR)(void) = spi_queue;
#endif

static void (*SPI_ISR)(void) = (void *) 0;

struct spi_config __config = 
{
    ._spi_mode = SPI_MODE,
    ._spi_prescaler = SPI_PRESCALE,
    ._spi_enable = SPI_ENABLE,
    ._spi_speed = SPI_SPEED_MODE,
    ._spi_polarity = SPI_POLARITY,
    ._spi_sampleEdge = SPI_SAMPLE_EDGE,
    ._spi_intEnable = SPI_INT_ENABLE,
    ._spi_dataOrder = SPI_DATA_ORDER
};

void spi_init(void)
{
    if (__config._spi_mode == SPI_MASTER)
    {
        SPI_DDR |= (1<<SPI_SS);
        SPI_DDR |= (1<<SPI_SCK);
        SPI_DDR |= (1<<SPI_MOSI);
        SPI_DDR &= ~(1<<SPI_MISO);
        SPI_PORT |= (__config._spi_polarity<<SPI_SCK);
        SPI_PORT &= ~(1<<SPI_SS);
        SPCR |= (__config._spi_prescaler);
    }
    else if (__config._spi_mode == SPI_SLAVE)
    {
        SPI_DDR &= ~(1<<SPI_SS);
        SPI_DDR &= ~(1<<SPI_SCK);
        SPI_DDR &= ~(1<<SPI_MOSI);
        SPI_DDR |= (1<<SPI_MISO);
    }
    
    SPCR |= (__config._spi_intEnable << SPIE);
    SPCR |= (__config._spi_dataOrder << DORD) | (__config._spi_mode << MSTR);
    SPCR |= (__config._spi_polarity << CPOL) | (__config._spi_sampleEdge << CPHA);
    SPSR |= (__config._spi_speed << SPI2X);
    SPCR |= (__config._spi_enable << SPE);      // NOTE: must leave activation to last
}

void spi_setSpeed(uint8_t speed)
{
    SPCR &= 0XFC;
    switch (speed)
    {
        case 2:
            SPSR |= (1<<SPI2X);
            break;
        case 4:
            SPSR &= ~(1<<SPI2X);
            break;
        case 8:
            SPSR |= (1<<SPI2X);
            SPCR |= (1<<SPR0);
            break;
        case 16:
            SPSR &= ~(1<<SPI2X);
            SPCR |= (1<<SPR0);
            break;
        case 32:
            SPSR |= (1<<SPI2X);
            SPCR |= (1<<SPR1);
            break;
        case 64:
            SPSR &= ~(1<<SPI2X);
            SPCR |= (1<<SPR1);
            break;
        case 128:
            SPSR &= ~(1<<SPI2X);
            SPCR |= (1<<SPR0);
            SPCR |= (1<<SPR1);
            break;
        default:
            // do nothing
            break;
    }
}


uint8_t spi_transfer(uint8_t _data)
{
    SPDR = _data;
    // slave waits here for the master to do the comm
    while((SPSR & (1<<SPIF)) == 0);
    _data = SPDR;
    return _data;
}


// TEST
#ifdef INTE
uint8_t spi_slaveGet(void)
{
    // NOTE: this is done with the intent of being used for the slave that doesnt
    //       need to respond back to the master
    
    uint8_t __tmp = 0;
    if (g_u8Buf_id0 == g_u8Buf_idx)  return NO_DATA;
    if (g_u8Buf_id0 >= MAX_SPI_BUF_SIZE)
    {
        g_u8Buf_id0 = 0;
    }
    __tmp = g_u8Queue[g_u8Buf_id0];
    g_u8Buf_id0++;
    
    return __tmp;
}

uint8_t spi_slavePut(uint8_t __sdata)
{
    return spi_transfer(__sdata);
}
#endif

uint8_t spi_checkCollision(void)
{
    uint8_t local_u8CollStatus = (SPSR >> WCOL) & 0x01;
    return local_u8CollStatus;
}

// use it at run time to run a user-defined function when transmission compeletes by interrupt
void spi_callbackFn(void (*ptrToSPI_ISR)(void))
{
    SPI_ISR = ptrToSPI_ISR;
}


#ifdef INTE
ISR()
{
    SPI_ISR();
}

static void spi_queue(void)
{
    volatile uint8_t data;
    
    data = SPDR;

    if((uint16_t)g_u8Buf_idx < MAX_SPI_BUF_SIZE)
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
        }
    }
}
#endif


/*
uint8_t spi_trans_multibyte(const unsigned char * ptr, uint8_t argc)
{
    uint8_t i;
    uint8_t returned;
    for(i = 0 ; i <= argc ; i++){
        returned = spi_trans(*(ptr+i));
        _delay_ms(30);
}
* void spi_preload(uint8_t data)        // for using with interrupt driven spi to receive data from slave in multibyte
{
    SPDR = data;
}

* */


void spi_setSS(void)
{
    #if defined (SPI_SS_ACTIVE_LOW)
        SPI_PORT &= ~(1<<SPI_SS);
    #elif defined (SPI_SS_ACTIVE_HIGH)
        SPI_PORT |= (1<<SPI_SS);
    #endif
}

void spi_clearSS(void)
{
    #if defined (SPI_SS_ACTIVE_LOW)
        SPI_PORT |= (1<<SPI_SS);
    #elif defined (SPI_SS_ACTIVE_HIGH)
        SPI_PORT &= ~(1<<SPI_SS);
    #endif
}
