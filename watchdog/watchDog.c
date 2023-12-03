//WDTCR für ATtinyN5
//WDTCSR für ATmega328

#include <avr/wdt.h>
#include <avr/interrupt.h>

void wdtOn();
void wdtOff();
void wdtReset();
void initWDT();

void initWDT() {
	WDTCR |= (1 << WDP3) | (1 << WDP0);	//Prescale 
}
void wdtOn() {
	WDTCR |= (1 << WDIE);	//Watchdog Timeout Interrupt Enable und start!
	sei();
}
void wdtOff() {
	wdt_disable();
}
void wdtReset() {
	wdt_reset();
}
int main() {
	initWDT();
	return 0;
}
ISR(WDT_vect){  // watchdog interrupt
	cli();
	//todo
	wdtReset();
	sei();
}
