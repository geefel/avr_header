#ifndef USI_ATTINY_H
#define USI_ATTINY_H

#include "pin.h"

#define TX_PIN PINDEF(B, 2)  //6

//OCR_VALUE=((F_CPU/(BAUD_*PRESCALE))-1
#define BAUD_ 19200
#define PRESCALE 8
#define OCR_VALUE 51
#define F_CPU 8000000UL

void initSoftUart();


void initUsi();
void pText(const char *txt);
void pInt(int16_t x);
void pN();
#endif	//USI_ATTINY_H
