/*
 * Hier muss noch die Prescale automatisiert werden
 */

#include <stdint.h>
#include <avr/interrupt.h>
#include "zeitMessen.h"
#include "uart_print.h"

static uint16_t ti = 0;

#if defined(attiny45)		//====attiny45====
void setupTimer() { 
  GTCCR = 0x00; 									// Wave Form Generation Mode 0: Normal Mode; OC2A disconnected
  OCR1A = 200; 										//für 0,1 ms pro Interrupt bei 8MHz prescale 4
  TIMSK = (1<<OCIE1A);						//interrupt wenn Zähler erreicht Vergleichsregister
  //TCCR1 = (1<<CS11) + (1<<CS10);	// prescaler = 4, startet Zähler!
} 
void startTimer() {
	TCCR1 = (1<<CS11) + (1<<CS10);	// prescaler = 4, startet Zähler!
}
void stopTimer() {
	TCCR1 = 0;
}
ISR(TIMER1_COMPA_vect){
	ti++;
}
#endif						      //====attiny45======

#if defined(atmega328p)	//====atmega328p====
void setupTimer() {
	TCCR2A = 0;
	TCCR2A |= (1 << WGM21);	//nur CTC anschalten
	TCCR2B = 0;
	TIMSK2 |= (1 << OCIE2A); //Interrupt Timer2 Vergleich A
	OCR2A = 200;						//für 0,1 ms pro Interrupt bei 16MHz prescale 8
	//TCCR2B = (1 << CS21);	//Prescale 8, startet Zähler!
	
}
void startTimer() {
	sei();
	TCCR2B |= (1 << CS20);	//Prescale 8, startet Zähler!
}
void stopTimer() {
	TCCR2B = 0;
	cli();
}
ISR(TIMER2_COMPA_vect){
	ti++;
}
#endif									//====atmega328p====

uint16_t getTime() {
	return ti;
}
void resetTime() {
	ti = 0;
}

