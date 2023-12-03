#include <stdio.h>
#include <util/delay.h>
#include "print.h"
#include "uart_attiny.h"
#include "pin.h"
//#include "i2c.h"

#define DS1307_ADDRESS 0x68 ///< I2C address for DS1307
#define DS1307_CONTROL_REG 0x07 ///< Control register


int main()
{
	
	_delay_ms(100);
	initUartSoft(9600);
	_delay_ms(100);
uint8_t u = 234;
	while(1) {
		out("Test: %i", &u);
		_delay_ms(500);
	}
		
	return 0;
}

