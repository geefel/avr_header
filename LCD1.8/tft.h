#ifndef TFT_H
#define TFT_H

#include "pin.h"
#include <avr/io.h>
																 //Arduino UNO Pin
#define SCK_PIN 	 	PINDEF(B, 5) //13	CLK	Orange
#define MISO_PIN 	 	PINDEF(B, 4) //12	
#define DIN_PIN 	 	PINDEF(B, 3) //11	DIN Grün
#define CS_PIN 			PINDEF(B, 2) //10	CS	Gelb
#define DC_PIN 			PINDEF(B, 1) //9	DC	Blau
#define RESET_PIN		PINDEF(B, 0) //8	RST	Braun
#define BL_PIN 			PINDEF(D, 7) //7	BL	Grau
//~ #define SD_CS_PIN  	PINDEF(D, 5) //5
//~ #define TP_CS_PIN  	PINDEF(D, 4) //4
//~ #define TP_IRQ_PIN 	PINDEF(D, 3) //3

#define CASET 0x2A ///< Column Address Set
#define RASET 0x2B ///< Page Address Set
#define RAMWR 0x2C ///< Memory Write
#define RAMRD 0x2E ///< Memory Read

#define TOUCH_THRES 20

extern uint16_t xDispMax;
extern uint16_t yDispMax;
extern uint16_t dispRotation;


#define SPI_hard

void init_Pins();
//~ #if defined(SPI_hard)
void init_SPI();
//~ #endif
void init_TFT();
void set_TFT_rotation(uint16_t grad);
void init_Touch();
void getTouchPos(uint16_t *x, uint16_t *y, uint16_t *z);
uint16_t get_Touch_Val(uint16_t command);
void drawPix(uint16_t x, uint16_t y, uint16_t color);
void drawWindow(uint16_t x, uint16_t y, uint16_t x1, uint16_t y2, uint16_t color);
void setBacklightDim(int8_t dim);


#endif	//TFT_H

