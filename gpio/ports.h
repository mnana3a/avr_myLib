#ifndef __PORTS_H
#define __PORTS_H

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

#if defined(__AVR_ATmega32__) || defined(__AVR_ATmega16__)
#define BASE_A            0x39
#define BASE_B            0x36
#define BASE_C            0x33
#define BASE_D            0x30

#define INPUT_OFFSET      0x00
#define DIR_OFFSET        0x01
#define DATA_OFFSET       0x02

#elif defined(__AVR_ATmega128__)
#define BASE_A            0x39
#define BASE_B            0x36
#define BASE_C            0x33
#define BASE_D            0x30
#define BASE_E            0x21
#define BASE_F            0x20
#define BASE_G            0x63

#define INPUT_OFFSET      0x00
#define DIR_OFFSET        0x01
#define DATA_OFFSET       0x02

#define F_INPUT_OFFSET    0x00
#define F_DIR_OFFSET      0x41
#define F_DATA_OFFSET     0x42

#endif

/************************************************************************/
/* Interrupt registers                                                  */
/************************************************************************/

#define SET_REGISTER                             0x5F
#define GENERAL_INTERRUPT_CONTROL_REG            0x5B
#define GENERAL_INTERRUPT_FLAG_REG               0x5A
#define MICRO_CONTROLLER_CONTROL_REG             0x55
#define MICRO_CONTROLLER_CONTROL_AND_STATUS_REG  0x54

#endif
