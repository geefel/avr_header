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

/*dieses Interface hat das Manko im Receiveteil sehr lange zu baruchen, bis die fallende Flanke gesucht wird
* siehe bitte dort
*/
#include "uart_soft.h"
#include "uart_print.h"
#include "uart_hard.h"

#define INTERRUPT_TIMER0_ENABLE TIMSK0 |= (1 << OCIE0A)
#define INTERRUPT_TIMER0_DISABLE TIMSK0 &= ~(1 << OCIE0A)
#define INTERRUPT_INT0_ENABLE EIMSK |= (1<< INT0)
#define INTERRUPT_INT0_DISABLE EIMSK &= ~(1<< INT0)
#define INTERRUPT_ALL_DISABLE SREG &= ~0x80
#define INTERRUPT_ALL_ENABLE SREG |= 0x80
#define TIMER0_STOP TCCR0B &= ~((1 << CS02) || (1 << CS01) || (1 << CS00))
#define TIMER0_RESET TCNT0 = 1

void setPrescaleOcrVal(uint32_t baud);
void setPrescal();

volatile uint8_t ocrVal = 0;
volatile uint16_t prescaleVal;
volatile uint8_t baudVal;
volatile uint8_t isTimerInterrupt = 0;
volatile uint8_t isINT0_Interrupt = 0;

void setPrescal() {
	switch (prescaleVal) {	
		case 1024: TCCR0B |= (1 << CS02) || (1 << CS00); 	break;	//startet auch Timer
		case 256:  TCCR0B |= (1 << CS02);              		break;   
		case 64:   TCCR0B |= (1 << CS01) || (1 << CS00);	break;
		case 8:    TCCR0B |= (1 << CS01); 								break;
		case 1:    TCCR0B |= (1 << CS00); 								break;
	}
}

void setPrescaleOcrVal(uint32_t baud) {
//#if F_CPU == 16000000
	switch (baud) {
		case 300: 	 prescaleVal = 256; ocrVal = 207; break;
		case 600: 	 prescaleVal = 256; ocrVal = 103; break;
		case 1200: 	 prescaleVal = 64; 	ocrVal = 207; break;
		case 2400: 	 prescaleVal = 64; 	ocrVal = 103; break;
		case 4800: 	 prescaleVal = 64; 	ocrVal = 51; 	break;
		case 9600:   prescaleVal = 8;  	ocrVal = 207; break;
		case 14400:  prescaleVal = 8; 	ocrVal = 138; break;
		case 19200:  prescaleVal = 8; 	ocrVal = 103; break;
		case 28800:  prescaleVal = 8; 	ocrVal = 68; 	break;
		case 38400:  prescaleVal = 8; 	ocrVal = 51; 	break;
		case 56000:  prescaleVal = 8; 	ocrVal = 35; 	break;
		case 57600:  prescaleVal = 8; 	ocrVal = 34; 	break;
		case 115200: prescaleVal = 1; 	ocrVal = 138; break;
		case 128000: prescaleVal = 1; 	ocrVal = 124; break;
		case 256000: prescaleVal = 1; 	ocrVal = 62; 	break;
	}
//#endif
}

void initSoftUart1() {
	setPrescaleOcrVal(BAUD_);
	INTERRUPT_ALL_DISABLE;
	INTERRUPT_INT0_DISABLE;
	INTERRUPT_TIMER0_DISABLE;
	
	//Ports
  setOutput(TX_PIN_1);
  setInput(RX_PIN_1);
  setPin(TX_PIN_1);
  //setPin(RX_PIN_1);
  
  // Timer0
  TCCR0A = 0x00;    //Init.	
  TCCR0B = 0x00;    //Init. Vorteiler
  TCCR0A |= (1 << WGM01);			// Timer in CTC mode. Interrupt bei erreichen von TCNT0 == 0CR0A
	
	//Vergleichsregister
	//ocrVal = (int8_t)((uint32_t)F_CPU / (uint32_t)BAUD_ / getPrescaleVal((uint32_t)BAUD_)) - 1;
	OCR0A = ocrVal;
	//External INT0 interrupt
  EICRA = 0x00;               // Init.
  EICRA |= ( 1 << ISC01 );    // Interrupt sense control: Eine fallende Flanke am INT0-Pin löst Interrupt aus 
  
  INTERRUPT_ALL_ENABLE;
}

//Es wird TIMER0 an/aus gemacht, sonst keiner (auch global)
void sendDat(uint8_t dat) {
	uint16_t maske = 0x01;
	uint16_t d = (dat << 1) + 512;
	
	TIMER0_RESET;
	INTERRUPT_TIMER0_ENABLE;
	isTimerInterrupt = 0;
	setPrescal();			//hier erst Timer starten!!!!

	for (uint8_t i = 0; i < 10; i++) {	
		if (d & maske) 
			setPin(TX_PIN_1);
		else
			clrPin(TX_PIN_1);
		maske <<= 1;
		isTimerInterrupt = 0;
		while (!isTimerInterrupt) {asm volatile ("nop" ::);}
	}
  TIMER0_STOP;
}

uint8_t receiveDat() {																						//von hier
	/*init*/
	uint8_t erg = 0;																								//
	uint8_t mask = 0x01;
	isINT0_Interrupt = 0;																						//
	INTERRUPT_TIMER0_DISABLE;																				//	
	EIFR |= 1 << INTF0;		//Interrupt-Flag löschen
	
	/*warten auf erste fallende Flanke (uart-START) */
	INTERRUPT_INT0_ENABLE;																					//bis hier dauert es zu lange: Evtl. vor einen sendDat-Befehl einsortieren
	while (!isINT0_Interrupt) {asm volatile ("nop" ::);}
	INTERRUPT_INT0_DISABLE;
	
	/*0,5 Takte warten bis Mitte START erreicht */
	isTimerInterrupt = 0;
	OCR0A = ocrVal >> 1; 		//Vergleichsregister
	TIMER0_RESET;
	INTERRUPT_TIMER0_ENABLE;
	setPrescal();						//hier erst Timer starten!!!!
	
	while (!isTimerInterrupt) {asm volatile ("nop" ::);}
	TIMER0_STOP;
	isTimerInterrupt = 0;
	OCR0A = ocrVal; 					//Vergleichsregister
	TIMER0_RESET;							//Zählregister TCNT0
	//INTERRUPT_TIMER0_ENABLE;
	setPrescal();							//hier erst Timer starten!!!!
	
	/*Daten-bits einsammeln*/
	for (uint8_t i = 0; i < 8; ++i) {
		while (!isTimerInterrupt) {asm volatile ("nop" ::);}
		if(getPin(RX_PIN_1))
			erg |= mask;
		mask <<= 1;
		isTimerInterrupt = 0;
	}

	TIMER0_STOP;
	return erg;
}

//Timer0 interrupt service routine.
ISR(TIMER0_COMPA_vect) {
	isTimerInterrupt = 1;
}
//INT0 interrupr service routine.
ISR(INT0_vect) {
	isINT0_Interrupt = 1;
}
