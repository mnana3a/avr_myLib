#include "inttypes.h"
#include "ports.h"

#ifndef HW_TYPES_H_
#define HW_TYPES_H_

static inline uint8_t HW_WR_BIT(uint16_t reg ,uint8_t pin , uint8_t val)
{
    uint8_t state = SUCCESS ;
    if (pin > MAX_PINS)
    {
        state = ERROR ;
    }
    else
    {
        if (val == HIGH)
        {
            *(volatile uint8_t*)reg |=(1<<pin);
        }
        else if (val == LOW)
        {
            *(volatile uint8_t*)reg &=~(1<<pin);
        }
    }
     return state ;
}
//----------------------------------------------------------
static inline uint8_t HW_RD_BIT(uint16_t reg , uint8_t pin , uint8_t *data)
{
    uint8_t state = SUCCESS ;
    if (pin > MAX_PINS)
    {
        state = ERROR ;
    }
    else
    {
        *data = (*(volatile uint8_t*)reg &(1<<pin))>>pin;
    }
     
    return state;
}
//----------------------------------------------------------
static inline uint8_t HW_WR_PORT(uint16_t reg ,uint8_t val)
{
    uint8_t state = SUCCESS;
    if (val > 255) {
        state = ERROR;
    }
    else 
    {
        *(volatile uint8_t*)reg = val ;
    }
    
    return state ;
}
//------------------------------------------------------------
static inline uint8_t HW_RD_PORT(uint16_t reg ,uint8_t *val)
{
    uint8_t state = SUCCESS ;
    *val = *(volatile uint8_t*)reg ;
    return state ;
}

static inline uint8_t HW_TOG_BIT(uint16_t reg ,uint8_t pin)
{
    uint8_t state = SUCCESS ;
    if (pin > MAX_PINS)
    {
        state = ERROR ;
    }
    else
    {
        *(volatile uint8_t*)reg ^= (1<<pin);
    }
     return state ;
}


static inline uint8_t HW_TOG_PORT(uint16_t reg ,uint8_t val)
{
    uint8_t state = SUCCESS;
    if (val > 255) {
        state = ERROR;
    }
    else 
    {
        *(volatile uint8_t*)reg ^= val ;
    }
    
    return state ;
}
#endif
