#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pin.h"
#include "sleep.h"
#include "uart_print.h"
#include "uart_hard.h"

#define INT0_PIN PINDEF(D, 2)
#define INT1_PIN PINDEF(D, 3)

void initInt0();
void initInt1();

int main() {
	uint8_t nr = 0;
	cli();
	
	initSleep(2);
	initUartHW(19200);
	initInt0();
	while(1) {
		pIntn(++nr);
		_delay_ms(1000);
		sei();
		sleepEnable();
		gotoSleep();
		_delay_ms(1000);
	}
	return 0;
}

void initInt0() {
	setInputPullup(INT0_PIN);
	EICRA |= (1 << ISC00);	//Jeder log Wechsel INT0
	EIMSK |= (1 << INT0);		//Freigabe
}

void initInt1() {
	setInputPullup(INT1_PIN);
	EICRA |= (1 << ISC10);	//Jeder log Wechsel INT1
	EIMSK |= (1 << INT1);		//Freigabe
}

ISR(INT0_vect)
{
	sleepDisable();
    // user code here
}

ISR(INT1_vect)
{
	sleepDisable();
    // user code here
}
