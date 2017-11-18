#ifndef __GPIO_H
#define __GPIO_H

#include <avr/io.h>
#include <inttypes.h>
#include "ports.h"
extern uint8_t gpio;
#ifndef F_CPU
    #define F_CPU 16000000UL
#endif

#define INPUT  0
#define OUTPUT 1

#define HIGH 1
#define LOW  0

#define SUCCESS 0
#define ERROR 1

#define PORT_INPUT  0x00
#define PORT_OUTPUT 0xFF

#define PORT_LOW  0x00
#define PORT_HIGH 0xFF

#define MAX_PINS 7

#define PIN_NUM_0 0
#define PIN_NUM_1 1
#define PIN_NUM_2 2
#define PIN_NUM_3 3
#define PIN_NUM_4 4
#define PIN_NUM_5 5
#define PIN_NUM_6 6
#define PIN_NUM_7 7

/************************************************************************/
/* Interrupt registers                                                  */
/************************************************************************/

#define SET_REGISTER                             0x5F
#define GENERAL_INTERRUPT_CONTROL_REG            0x5B
#define GENERAL_INTERRUPT_FLAG_REG               0x5A
#define MICRO_CONTROLLER_CONTROL_REG             0x55
#define MICRO_CONTROLLER_CONTROL_AND_STATUS_REG  0x54


uint8_t gpio_config_pin(uint8_t reg, uint8_t pin, uint8_t dir);
uint8_t gpio_config_port(uint8_t reg, uint16_t val);
uint8_t gpio_put_pin(uint8_t reg, uint8_t pin, uint8_t val);
uint8_t gpio_put_port(uint8_t reg, uint16_t val);
uint8_t gpio_get_pin(uint8_t reg, uint8_t pin, uint8_t * data);
uint8_t gpio_get_port(uint8_t reg, uint8_t *val);
uint8_t gpio_toggle_pin(uint8_t reg, uint8_t pin);
uint8_t gpio_toggle_port(uint8_t reg, uint8_t val);

#endif
