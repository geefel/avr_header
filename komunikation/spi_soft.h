#ifndef SPI_SOFT_H
#define SPI_SOFT_H

#include "pin.h"
#include <avr/io.h>
																 //Arduino UNO Pin
#define SCK_PIN 	 	PINDEF(B, 5) //13
#define MISO_PIN 	 	PINDEF(B, 4) //12
#define MOSI_PIN 	 	PINDEF(B, 3) //11
#define CS_PIN 			PINDEF(B, 2) //10
#define CD_PIN			PINDEF(B, 1) //Command- oder Date-Byte?

void initSPI();
void setCommData(uint8_t c);
uint8_t sendDat(uint8_t dat);
uint8_t sendData8(uint8_t dat);
uint16_t sendData16(uint16_t dat);
void sendDataAr(uint8_t *datReseive, uint8_t *datSend, uint8_t size_of_datSend);

#endif  //SPI_SOFT_H
