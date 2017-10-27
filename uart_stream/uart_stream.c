#include "uart_stream.h"

FILE uart_stream = FDEV_SETUP_STREAM(uart_send_u8, NULL, _FDEV_SETUP_WRITE);


void stream_init(uint16_t baud)
{
	uart_init(baud);
	stdout = &uart_stream;
	fprintf(stdout, "initiating...");
	sei();
}
