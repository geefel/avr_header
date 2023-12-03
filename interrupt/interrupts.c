#include <avr/interrupt.h>
#include <util/delay.h>
#include "sleep.h"

#define SEC_MAX 30

void setSleeping();

uint8_t sekunde;
uint8_t sleeping;

void initEncoder() {
	setInputPullup(ENCODER_PIN_A);
	setInputPullup(ENCODER_PIN_B);
	setInputPullup(ENCODER_TASTE);
	setInputPullup(TASTE_1);
	setInputPullup(TASTE_2);
	setInputPullup(TASTE_3);
	
	setInputPullup(CLOCK_1_SEC_PIN);

	encoderDir = 0;
	encoderAktion = 0;
	encoderTaste = -1;
	encoderModus = 0;
	sekunde = 0;
	
	//INT0 einstellen
	EICRA |= 1 << ISC00;	 //Pegelwechsel an A_INT0 löst Interrupt aus
	EIMSK |= 1 << INT0;		 //Interruptfreigabe für INT0
	
	PCICR  |=  1 << PCIE1;	 //Interruptfreigabe für PCINT1
	PCMSK1 |= (1 << PCINT8) | (PCINT9 << 1) | (PCINT10 << 1); //Pegelwechsel an PCINT1 löst Interrupt aus
	PCICR  |=  1 << PCIE2;	 //Interruptfreigabe für PCINT2
	PCMSK2 |=  1 << PCINT20;
	PCICR  |=  1 << PCIE0;	 //Interruptfreigabe für PCINT0
	PCMSK0 |=  1 << PCINT0;
	
}

void clearSekunden() {
	sekunde = 0;
}

void stopPCInt0() {
	PCICR  &=  ~(1 << PCIE0);	 //Interruptfreigabe aufheben für PCINT0 (Sekundentakt)
}

void startPCInt0() {
	PCICR  |=  (1 << PCIE0);	 //Interruptfreigabe für PCINT0 (Sekundentakt)
}

void setSleeping() {
	sleeping = 1;
}
uint8_t isSleeping() {
	return sleeping;
}

void clrSleeping() {
	sleeping = 0;
}

/**
 * wenn A_INT0,B_PIN 1,1 oder 0,0 dann rechts gedreht
 * wenn A_INT0,B_PIN 0,1 oder 1,0 dann links gedreht
 **/
ISR(INT0_vect) {			//Encoder
	if (isSleeping()) { _delay_ms(10); return;};
	_delay_ms(10);				//Hardware-Prellen unterdrücken
	if (getPin(ENCODER_PIN_A) == getPin(ENCODER_PIN_B))
		encoderDir = rechtsrum;
	else
		encoderDir = linksrum;
	encoderTaste = encoder_rotation;
	encoderAktion = rotation;	//um if(encoderAktion) auszulösen
	EIFR |= 1 << INTF0;	//Interrupt-Flag löschen: Software-Prellen unterdrücken. Umgekerte Logig hier!
}

ISR(PCINT0_vect) {	//1 sec Rechteck an D8
	if (isSleeping()) return;
	_delay_ms(1);				//Hardware-Prellen unterdrücken
	if ((getPin(CLOCK_1_SEC_PIN))) {
		sekundenAktion = 1;
	}
	else
		sekundenAktion = 0;
	PCIFR |= 1 << PCIF0;		//Interrupt-Flag löschen
	if(++sekunde == SEC_MAX) {
		stopPCInt0();
		setSleeping();
		clearSekunden();
	}
}

ISR(PCINT1_vect) {					//die drei schwarzen Tasten	
	cli();
	if (isSleeping()) { _delay_ms(10); return;}
	_delay_ms(10);				    	//Hardware-Prellen unterdrücken
	if (!getPin(TASTE_1)) {
		encoderTaste = taste_1;	//negative Logig!
		encoderAktion = taste;	//um if(encoderAktion) auszulösen
	}
	else if (!getPin(TASTE_2)) {
		encoderTaste = taste_2;	//negative Logig!
		encoderAktion = taste;	//um if(encoderAktion) auszulösen
	}
	else if (!getPin(TASTE_3)) {
		encoderTaste = taste_3;	//negative Logig!
		encoderAktion = taste;	//um if(encoderAktion) auszulösen
	}
	PCIFR |= 1 << PCIF1;	  //Interrupt-Flag löschen: Software-Prellen unterdrücken. Umgekerte Logig hier!
}

ISR(PCINT2_vect) {				//Encoder-Taste
	if (isSleeping()) { _delay_ms(10); return;}
	_delay_ms(10);				    //Hardware-Prellen unterdrücken
	if (!getPin(ENCODER_TASTE)) {
		encoderTaste = encoder_taste; //negative Logig!
		encoderAktion = taste;	//um if(encoderAktion) auszulösen
		encoderModus = encoderModus == play ? edit : play;
	}
	PCIFR |= 1 << PCIF2;	 //Interrupt-Flag löschen: Software-Prellen unterdrücken. Umgekerte Logog hier!
}
