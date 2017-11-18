#include "gpio.h"
#include "hw_types.h"

/**
* Func : init_pin
* Args : reg -> register name(BASE_A,BASE_B,BASE_C,or BASE_D)
       : pin -> pin number (0,1,2,3,4,5,6,or 7)
       : dir -> INPUT
             -> OUTPUT
*Return : state -> it includes two possiblilities 
          SUCCESS  = function success
          E_FAIL  = function fail
*/
uint8_t gpio_config_pin(uint8_t reg, uint8_t pin, uint8_t dir)
{
    uint8_t state = SUCCESS;
        if (OUTPUT == dir) {
            if (reg == BASE_F)
            {
                state = HW_WR_BIT(reg + F_DIR_OFFSET , pin , dir);
            }
            else
            {
                state = HW_WR_BIT(reg + DIR_OFFSET , pin , dir);
            }
            
        } 
        else if (INPUT == dir) {
            if (reg == BASE_F)
            {
                state = HW_WR_BIT(reg + F_DIR_OFFSET , pin , dir);
            }
            else
            {
                state = HW_WR_BIT(reg + DIR_OFFSET , pin , dir);
            }
        }
    return state;
}
//------------------------------------------------------------------
/**
* Func : init_port
* Args : reg -> register name(BASE_A,BASE_B,BASE_C,or BASE_D)
       : dir -> PORT_INPUT
             -> PORT_OUTPUT
*Return : state -> it includes two possiblilities 
          SUCCESS  = function success
          E_FAIL  = function fail
*/
uint8_t gpio_config_port(uint8_t reg, uint16_t val)
{
    uint8_t state = SUCCESS;
    if (reg == BASE_F)
    {
        state = HW_WR_PORT(reg + F_DIR_OFFSET,val) ;
    }
    else
    {
        state = HW_WR_PORT(reg + DIR_OFFSET,val) ;
    }
    return state;
}
//------------------------------------------------------------------
/**
* Func : write_pin
* Args : reg -> register name(BASE_A,BASE_B,BASE_C,or BASE_D)
       : pin -> pin number (0,1,2,3,4,5,6,or 7)
       : val -> HIGH
             -> LOW
*Return : state -> it includes two possiblilities 
          SUCCESS  = function success
          E_FAIL  = function fail
*/
uint8_t gpio_put_pin(uint8_t reg, uint8_t pin, uint8_t val)
{
    uint8_t state = SUCCESS ;
    if (HIGH == val)
    {
        if (reg == BASE_F)
        {
            state = HW_WR_BIT(reg + F_DATA_OFFSET , pin , val);
        }
        else
        {
            state = HW_WR_BIT(reg + DATA_OFFSET , pin , val);
        }
    } 
    else if (LOW == val)
    {
        if (reg == BASE_F)
        {
            state = HW_WR_BIT(reg + F_DATA_OFFSET ,pin , val);
        }
        else
        {
            state = HW_WR_BIT(reg + DATA_OFFSET ,pin , val);
        }
    }

    return state;
}
//------------------------------------------------------------------
/**
* Func : write_port
* Args : reg -> register name(BASE_A,BASE_B,BASE_C,or BASE_D)
       : val -> PORT_HIGH
             -> PORT_LOW
*Return : state -> it includes two possiblilities 
          SUCCESS  = function success
          E_FAIL  = function fail
*/
uint8_t gpio_put_port(uint8_t reg, uint16_t val)
{
    uint8_t state = SUCCESS;
    if (reg == BASE_F)
    {
        state = HW_WR_PORT(reg + F_DATA_OFFSET, val);
    }
    else
    {
        state = HW_WR_PORT(reg + DATA_OFFSET, val);
    }
    return state;
}
//------------------------------------------------------------------
/**
* Func : read_pin
* Args : reg -> register name(BASE_A,BASE_B,BASE_C,or BASE_D)
       : pin -> pin number (0,1,2,3,4,5,6,or 7)
       : *data->pointer to return the value in
*Return : state -> it includes two possiblilities 
          SUCCESS  = function success
          E_FAIL  = function fail
*/
uint8_t gpio_get_pin(uint8_t reg, uint8_t pin, uint8_t * data)
{
    uint8_t state = SUCCESS;
    if (reg == BASE_F) state = HW_RD_BIT(reg + F_INPUT_OFFSET , pin , data);
    else
    {
        state = HW_RD_BIT(reg + INPUT_OFFSET , pin , data);
    }
    
    return state;
}
//---------------------------------------------------------------- 
/**
* Func : read_port
* Args : reg -> register name(BASE_A,BASE_B,BASE_C,or BASE_D)
       : pin -> pin number (0,1,2,3,4,5,6,or 7)
       : *val ->pointer to return the value in
*Return : state -> it includes two possiblilities 
          SUCCESS  = function success
          E_FAIL  = function fail
*/
uint8_t gpio_get_port(uint8_t reg, uint8_t *val)
{
    uint8_t state = SUCCESS;
    if (reg == BASE_F) state = HW_RD_PORT(reg + F_INPUT_OFFSET , val);
    else
    {
        state = HW_RD_PORT(reg + INPUT_OFFSET , val);
    }
    
    return state;
}


uint8_t gpio_toggle_pin(uint8_t reg, uint8_t pin)
{
    uint8_t state = SUCCESS;
    if (reg == BASE_F)  state = HW_TOG_BIT(reg + F_DATA_OFFSET , pin);
    else
    {
        state = HW_TOG_BIT(reg + DATA_OFFSET , pin);
    }
    
    return state;
}

uint8_t gpio_toggle_port(uint8_t reg, uint8_t val)
{
    uint8_t state = SUCCESS;
    if (reg == BASE_F)  state = HW_TOG_PORT(reg + F_DATA_OFFSET , val);
    else
    {
        state = HW_TOG_PORT(reg + DATA_OFFSET , val);
    }
    
    return state;
}
