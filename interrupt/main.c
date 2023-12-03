#include <avr/io.h>
#include <util/delay.h>
#include "int0.h"
#include "pin.h"
#include "uart_attiny.h"
#include "print.h"

int main(){
	
	initUartSoft(9600);
	setupINT0();
	sei();
	while(1) {
		//~ _delay_ms(1000);
		//~ out("main\n");
	}
}
