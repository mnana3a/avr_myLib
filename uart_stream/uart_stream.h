#include "uart.h"
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

void stream_init(uint16_t baud);

extern FILE uart_stream;
