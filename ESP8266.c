#include <util/delay.h>

#include "ESP8266.h"
#include "UART.h"

void initServerEsp(void)
{
	initUART();
	uartWriteString("ATE0\r\n"); // echo off
	_delay_ms(300);
	uartWriteString("AT+CIPMODE=0\r\n"); // cipmode must be 0 to allow cipmux=1
	_delay_ms(300);
	uartWriteString("AT+CIPMUX=1\r\n"); // multiple connections
	_delay_ms(300);
	uartWriteString("AT+CIPSERVER=1,65000\r\n"); // start tcp server on port 65000
	_delay_ms(300);
}
