#include <avr/interrupt.h>
#include <util/delay.h>
#include "encoder.h"

#include "uart_hard.h"
#include "uart_print.h"

//zur schnellen Ausgabe
void print1(const char *txt, int8_t val) {
	pText(txt);
	pText(" = ");
	pItoAn(val);
}

void initEncoder() {
	setInput(ENCODER_PIN_A);
	setInput(ENCODER_PIN_B);
	setInput(ENCODER_TASTE);
	setInputPullup(ENCODER_PIN_A);
	setInputPullup(ENCODER_PIN_B);
	setInputPullup(ENCODER_TASTE);

	encoderDir = 0;
	encoderAktion = 0;
	
	//INT0 einstellen
	EICRA |= 1 << ISC00;	 //Pegelwechsel an A_INT0 löst Interrupt aus
	EIMSK |= 1 << INT0;		 //Interruptfreigabe für INT0
	
	PCICR |= 1 << PCIE1;	 //Interruptfreigabe für PCINT
	PCMSK1 |= 1 << PCINT8; //Pegelwechsel an PCINT0 löst Interrupt aus
}

/**
 * wenn A_INT0,B_PIN 1,1 oder 0,0 dann rechts gedreht
 * wenn A_INT0,B_PIN 0,1 oder 1,0 dann links gedreht
 **/
ISR(INT0_vect){
	_delay_ms(1);				//Hardware-Prellen unterdrücken
	if (getPin(ENCODER_PIN_A) == getPin(ENCODER_PIN_B))
		encoderDir = 1;
	else
		encoderDir = -1;
	encoderAktion = 1;	//um if(encoderAktion) auszulösen
	EIFR |= 1 << INTF0;	//Interrupt-Flag löschen: Software-Prellen unterdrücken. Umgekerte Logog hier!
}

ISR(PCINT1_vect) { 
	_delay_ms(1);				//Hardware-Prellen unterdrücken
	encoderAktion = 2;	//um if(encoderAktion) auszulösen
	encoderTaste = getPin(ENCODER_TASTE);
	PCIFR |= 1 << PCIF1;	//Interrupt-Flag löschen: Software-Prellen unterdrücken. Umgekerte Logog hier!
}
