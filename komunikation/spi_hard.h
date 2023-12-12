#ifndef SPI_HARD_H
#define SPI_HARD_H

#include "pin.h"
#include <avr/io.h>
																 //Arduino UNO Pin
#define SCK_PIN 	 	PINDEF(B, 5) //13
#define MISO_PIN 	 	PINDEF(B, 4) //12
#define MOSI_PIN 	 	PINDEF(B, 3) //11
#define CS_PIN 			PINDEF(B, 2) //10

//für 4-Draht-Übertragung
//#define DC_PIN 			PINDEF(D, 7) //7

void initSPI();
uint8_t send_Data(
#if defined(DC_PIN)
  uint8_t is_data_command, 
#endif
                            uint8_t val);

#endif	//SPI_HARD_H
