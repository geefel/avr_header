#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <avr/io.h>
#include "pin.h"

#define ENCODER_PIN_A PINDEF(D, 2)	//PinA -> INT0			 D2
#define ENCODER_PIN_B PINDEF(D, 3)  //PinB -> egal 			 D3
#define ENCODER_TASTE PINDEF(D, 4)	//Taste -> PCINT8    D4

#define TASTE_1 PINDEF(C, 0)  			//PCINT(8)           A0
#define TASTE_2 PINDEF(C, 1)  			//PCINT(9)           A1
#define TASTE_3 PINDEF(C, 2)  			//PCINT(10)          A2

#define CLOCK_1_SEC_PIN PINDEF(B, 0) //PCINT(0)          B0 

enum tastenWahl { 
	encoder_taste = 1,
	encoder_rotation,
	taste_1,
	taste_2,
	taste_3
};

enum modus {
	play = 0,
	edit,
	taste,
	rotation
};

enum _rotation {
	linksrum = -1,
	rechtsrum = 1
};

/**
 * encoderDir (1,-1) wird in ISR gesetzt
 * encoderAktion (1,0) wird in ISR gesetzt und in if(encoderAktion) zurück gesetzt
 **/
volatile int8_t encoderDir, encoderAktion, encoderTaste, encoderModus;
volatile int8_t sekundenAktion;
volatile uint16_t timer1, timer2, timer3;

void initEncoder();
void stopPCInt0();
void startPCInt0();
uint8_t isSleeping();
void clrSleeping();
void clearSekunden();

#endif	//INTERRUPTS_H
