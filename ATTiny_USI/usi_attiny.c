#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "usi_attiny.h"
#include "pin.h"


void sendDat(uint8_t dat);

void pText(const char *txt) {
	uint8_t i = 0;
	 while (txt[i] != '\0'){
		sendDat(txt[i]);
		++i;
		_delay_us(500);
	}
}

void pInt(int16_t x) {
	char txt[6];
	itoa(x, txt, 10);
	pText(txt);
}

void pN() {
	sendDat('\n');
}

/*
   9600 8N1 heißt Baudrate (= bitrate) 9600, 8 bit Datenlänge, no parity, 1 Stopbit
   7E1 heißt 7 bit Datenlänge, gerade parity, 1 Stopbit
   9O1 9 bit Datenlänge, odd parity, 1 Stopbit
   Unterschieden wird zwischen zwei Paritätsprotokollen, in beiden Fällen wird die Anzahl
   der Einsen innerhalb des zugeordneten Wortes ermittelt:

   Parität „even“ (gerade):
   Ist eine ungerade Anzahl Datenbits im Datum gesetzt, so wird das Paritätsbit gesetzt.
   Ist eine gerade Anzahl Datenbits im Datum gesetzt, so wird das Paritätsbit nicht gesetzt.

   Parität „odd“ (ungerade):
   Ist eine gerade Anzahl Datenbits im Datum gesetzt, so wird das Paritätsbit gesetzt.
   Ist eine ungerade Anzahl Datenbits im Datum gesetzt, so wird das Paritätsbit nicht gesetzt.

   Mit „gerade“ oder „ungerade“ ist also die Anzahl gesetzter Datenbits
   inklusive des Paritätsbits gemeint.
*/

/*maximal baud = 57600*/

#define INTERRUPT_TIMER0_ENABLE  TIMSK  |=  (1 << OCIE0A)
#define INTERRUPT_TIMER0_DISABLE TIMSK  &= ~(1 << OCIE0A)
#define INTERRUPT_ALL_DISABLE    SREG   &= ~0x80
#define INTERRUPT_ALL_ENABLE     SREG   |=  0x80
#define TIMER0_STOP              TCCR0B &= ~((1 << CS02) || (1 << CS01) || (1 << CS00))
#define TIMER0_START             TCCR0B =  (1 << CS01)	//Der muss auf '=' und nicht '|='?
#define TIMER0_RESET             TCNT0   =   0

volatile uint16_t ocrVal = 103;
volatile uint8_t isTimerInterrupt = 0;

void initUsi() {
	INTERRUPT_ALL_DISABLE;
	INTERRUPT_TIMER0_DISABLE;
	
	//Ports
  setOutput(TX_PIN);
  setPin(TX_PIN);
  
  // TIMER0
  //TCCR0A = 0x00;    //Init.					    //Das kann nicht benutzt werden?
  //TCCR0B = 0x00;    //Init. Vorteiler   //Das kann nicht benutzt werden?
  TCCR0A = (1 << WGM01);			//Der muss auf '=' und nicht '|='?// Timer in CTC mode. Interrupt bei erreichen von TCNT0 == 0CR0A
  
	//Vergleichsregister
	OCR0A = OCR_VALUE;
  
  INTERRUPT_ALL_ENABLE;
}

//Es wird TIMER0 an/aus gemacht, sonst keiner (auch global)
void sendDat(uint8_t dat) {
	uint16_t maske = 0x01;
	uint16_t d = (dat << 1) + 512;
	
	TIMER0_RESET;
	INTERRUPT_TIMER0_ENABLE;
	isTimerInterrupt = 0;
	TIMER0_START;			//hier erst Timer starten!!!!

	for (uint8_t i = 0; i < 10; i++) {	
		if (d & maske) 
			setPin(TX_PIN);
		else
			clrPin(TX_PIN);
		maske <<= 1;
		isTimerInterrupt = 0;
		while (!isTimerInterrupt) {asm volatile ("nop" ::);}
	}
  TIMER0_STOP;
}

//TIMER0 interrupt service routine.
ISR(TIMER0_COMPA_vect) {
	isTimerInterrupt = 1;
}
