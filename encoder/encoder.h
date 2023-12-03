#ifndef ENCODER_H
#define ENCODER_H

#include <avr/io.h>
#include "pin.h"

#define ENCODER_PIN_A PINDEF(D, 2)	//PinA -> INT0			 D2
#define ENCODER_PIN_B PINDEF(D, 3)  //PinB -> egal 			 D3
#define ENCODER_TASTE PINDEF(C, 0)	//Taste -> PCINT8    A0

/**
 * encoderDir (1,-1) wird in ISR gesetzt
 * encoderAktion (1,0) wird in ISR gesetzt und in if(encoderAktion) zurück gesetzt
 **/
volatile int8_t encoderDir, encoderAktion, encoderTaste;

void initEncoder();

#endif	//ENCODER_H
