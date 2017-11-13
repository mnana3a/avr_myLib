#include "uart/uart.h"
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

extern FILE uart_stream;

void stream_init(uint16_t baud);
