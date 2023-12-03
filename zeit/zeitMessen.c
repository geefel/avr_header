#include <avr/interrupt.h>
#include "zeitMessen.h"

void setupTimerTN5(){ 
  GTCCR = 0x00; // Wave Form Generation Mode 0: Normal Mode; OC2A disconnected
  TCCR1 = (1<<CS11) + (1<<CS10); // prescaler = 4
  TIMSK = (1<<TOIE1); // interrupt when TCNT2 is overflowed
} 

void setupTimer328() {
  TCCR0A = 0x00; // Wave Form Generation Mode 0: Normal Mode; OC2A disconnected
  TCCR0B = (1<<CS02) + (1<<CS00); // prescaler = 1024
  TIMSK0 = (1<<TOIE0); // interrupt wenn Zähler0 overflowed
}

uint8_t getTime() {
	return ti;
}
void resetTimer() {
	ti = 0;
}

ISR(TIMER1_OVF_vect){
	ti++;
}

ISR(TIMER0_OVF_vect) {
  ti++;
}
