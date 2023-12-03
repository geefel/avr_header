
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "main.h"
#include "uart_hard.h"
#include "uart_print.h"
#include "pin.h"
#include "encoder.h"

/**
 * Definiere zwei Pins: PinA vom Encoder auf (hier) INT0 oder INT1
 * PinB des Encoders irgendwo
 */
//#define ENCODER_PIN_A PINDEF(D, 2)	//PinA -> INT0			 D2
//#define ENCODER_PIN_B PINDEF(D, 3)  //PinB -> egal 			 D3
//#define ENCODER_TASTE PINDEF(C, 0)	//Taste -> PCINT(8)  A0

/**
 * encoderDir (1,-1) wird in ISR gesetzt
 * encoderAktion (1,0) wird in ISR gesetzt und in if(encoderAktion) zurück gesetzt
 **/
//volatile int8_t encoderDir, encoderAktion = 0;

//zur schnellen Ausgabe
void print(const char *txt, int8_t val) {
	pText(txt);
	pText("=");
	pItoAn(val);
}

int main() {
	//UARTHW und Pins einstellen
	initUartHW(19200);
	initEncoder();
	sei();
//	setInput(A_INT0);
//	setInput(B_PIN);
//	setInput(ENTER);
//	setInputPullup(A_INT0);
//	setInputPullup(B_PIN);
//	setInputPullup(ENTER);
	
	//INT0 einstellen
//	EICRA |= 1 << ISC00;	//Pegelwechsel an A_INT0 löst Interrupt aus
//	EIMSK |= 1 << INT0;		//Interruptfreigabe für INT0
//	SREG  |= 0x80;				//Globale Interruptfreigabe
	
	int8_t a = 0, b = 0;
	int8_t val = 0;
	
	
	print("a", a);
	print("b", b);
	
/**
 * Parameter ist ausgewählt, dann pollen auf: Encoder wird gedreht. 
 * Encoder wird gedreht -> Interrupt (Ergebnis: encoderDir ist 1 oder -1, encoderAktion ist 1)
 * if(encoderAktion) wird ausgeführt, Funktion aufrufen um val zu verarbeiten
 * dann encoderAktion zurücksetzen.
 * bei tastendruck == 0 (z.B. OK-Button): verlassen von Polling und INT0 ausschalten
 * Taste ist default: HIGH, bei drücken: LOW
 * Encoder ist Default: egal, es wird auf Flankenwechsel getestet
 */
	while(1){
		if(encoderAktion == 1) {				//encoderAktion wird in ISR gesetzt
			val += encoderDir;
			print("a", val);				//hier val an verändernde Funktion übergeben (mach es kurz!!!!)
			encoderAktion = 0;
		}
		else if(encoderAktion == 2) {	//OK-Button negative Logik
			print("Taste", encoderTaste);
			encoderAktion = 0;
		}
		else
			_delay_ms(50);
	}
	return 1;
}

/**
 * wenn A_INT0,B_PIN 1,1 oder 0,0 dann rechts gedreht
 * wenn A_INT0,B_PIN 0,1 oder 1,0 dann links gedreht
 **/
//ISR(INT0_vect){
//	_delay_ms(1);				//Hardware-Prellen unterdrücken
//	if (getPin(A_INT0) == getPin(B_PIN))
//		encoderDir = 1;
//	else
//		encoderDir = -1;
//	encoderAktion = 1;	//um if(encoderAktion) auszulösen
//	EIFR |= 1 < INTF0;	//Interrupt-Flag löschen: Software-Prellen unterdrücken
//}
