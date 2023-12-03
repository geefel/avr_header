#ifndef UART_ATTINY_H
#define UART_ATTINY_H

#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "pin.h"

#define TIMER_0

#define RX_PIN PINDEF(B, 1)	//Muss eingestellt werden!!!!!!!!!!! PCINT1
#define TX_PIN PINDEF(B, 4)	//Muss eingestellt werden!!!!!!!!!!!
#define PCINTn PCINT1       //Muss eingestellt werden!!!!!!!!!!!

//ORCVAL = F_CPU / BAUD_ / PRESCALE
#define BAUD_ 9600	          						//Muss eingestellt werden!!!!!!!!!!!
#define PRESCALE 1
//~ #define ORCVAL 103         	  						//Muss eingestellt werden!!!!!!!!!!!
//~ #define ORCVAL_HALB (ORCVAL - 40)					//MUSS eingestellt werden!!!!!!!!!!!
#define PRESCALE_T1 (1 << CS10) 					//MUSS eingestellt werden!!!!!!!!!!!
#define PRESCALE_T0 (1 << CS00) 					//MUSS eingestellt werden!!!!!!!!!!!

void initUartSoftATtiny();
void sendDat(uint8_t dat);

void clearNewData();
uint8_t isNewData();
uint8_t getNewData();
uint8_t ergNr;

#endif //UART_ATTINY_H
