#ifndef UART_SOFT_H
#define UART_SOFT_H

#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pin.h"
#include "buffer.h"


#define RX_PIN PINDEF(B, 4)	//Muss eingestellt werden!!!!!!!!!!! PCINT4
#define TX_PIN PINDEF(B, 3)	//Muss eingestellt werden!!!!!!!!!!!

#define LED_PIN PINDEF(B, 1)
void pinLeuchten();

//ORCVAL = F_CPU / BAUD_ / PRESCALE
#define BAUD_ 9600	          						//Muss eingestellt werden!!!!!!!!!!!
#define ORCVAL 103         	  						//Muss eingestellt werden!!!!!!!!!!!
#define ORCVAL_HALB (ORCVAL - 40)					//MUSS eingestellt werden!!!!!!!!!!!

//~ #define BAUD_ 4800	          						//Muss eingestellt werden!!!!!!!!!!!
//~ #define ORCVAL 208         	  						//Muss eingestellt werden!!!!!!!!!!!
//~ #define ORCVAL_HALB (ORCVAL - 79)					//MUSS eingestellt werden!!!!!!!!!!!

#define INTERRUPT_TIMER0_ENABLE 	TIMSK0 |= (1 << OCIE0A)
#define INTERRUPT_TIMER0_DISABLE 	TIMSK0 &= ~(1 << OCIE0A)
#define INTERRUPT_INT0_ENABLE 		GIMSK |= (1 << INT0)
#define INTERRUPT_INT0_DISABLE 		GIMSK &= ~(1 << INT0)
#define INT0_RESET 								GIFR |= 1 << INTF0
#define INTERRUPT_ALL_DISABLE 		SREG &= ~0x80
#define INTERRUPT_ALL_ENABLE 			SREG |= 0x80
#define TIMER0_STOP 							TCCR0B &= ~((1 << CS02) || (1 << CS01) || (1 << CS00))
#define TIMER0_START 							TCCR0B |= (1 << CS00)				//Muss eingestellt werden!!!!!!!!!!! Prescale
#define TIMER0_RESET 							TCNT0 = 0
#define INTERRUPT_PCINT_ENABLE 		PCICR |= (1 << PCIE2)				//Muss eingestellt werden!!!!!!!!!!! PCINTn
#define INTERRUPT_PCINT_DISABLE 	PCICR &= ~(1 << PCIE2)			//Muss eingestellt werden!!!!!!!!!!! PCINTn
#define PCINT_RESET 							PCIFR |= 1<< PCIF2					//Muss eingestellt werden!!!!!!!!!!! PCINTn
#define PCINT_PORT_SELECT					PCMSK2 = 1 << PCINT16				//Muss eingestellt werden!!!!!!!!!!! PCINTn

void initUartSoft(uint16_t baud);
void uart_Transmit_Soft(uint8_t dat);

void clearNewData();
uint8_t isNewData();
uint8_t getDat();
static volatile uint16_t mask;
uint8_t ergNr;
volatile uint8_t isTimerInterrupt;
volatile uint8_t isINT0_Interrupt;

#endif //UART_SOFT_H

