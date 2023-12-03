#ifndef UART_SOFT_GFL_H
#define UART_SOFT_GFL_H

#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pin.h"

#define RX_PIN_1 PINDEF(D, 2)  //2
#define TX_PIN_1 PINDEF(D, 6)  //6
#define BAUD_ 57600

//~ #define DATA_LENGTH_1 8  //7, 8 oder 9
//~ #define PARITY_1 0       //0: kein, 1: odd, 2: even

void initSoftUart1();
void sendDat(uint8_t dat);
uint8_t receiveDat();

#endif //UART_SOFT_GFL_H
