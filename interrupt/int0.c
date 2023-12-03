/*
 	ISC01	ISC00
		0			0			The low level of INT0 generates an interrupt request.
		0			1			Any logical change on INT0 generates an interrupt request.
		1			0			The falling edge of INT0 generates an interrupt request.
		1			1			The rising edge of INT0 generates an interrupt request.
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include "print.h"


void setupINT0() {
	//ATmegaX8
	//~ EICRA |= 1 << ISC00;	 //Pegelwechsel an A_INT0 löst Interrupt aus
	//~ EIMSK |= 1 << INT0;		 //Interruptfreigabe für INT0
	
	//ATtinyX5
	MCUCR |= 1 << ISC01;
	GIMSK |= 1 << INT0;
}

ISR(INT0_vect) {
	out("test\n");	
}
