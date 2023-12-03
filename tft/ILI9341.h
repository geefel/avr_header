#ifndef ILI9341_H
#define ILI9341_H

//ILI9341 kontrolieren!!!!!!!
#define SCK_PIN 	 	PINDEF(B, 5) //13
#define MISO_PIN 	 	PINDEF(B, 4) //12
#define MOSI_PIN 	 	PINDEF(B, 3) //11
#define LCD_CS_PIN 	PINDEF(D, 5) //10
#define LCD_BL_PIN 	PINDEF(D, 7) //9
#define LCD_DC_PIN 	PINDEF(D, 6) //7 Data Command control pin
#define SD_CS_PIN  	PINDEF(D, 5) //5
#define TP_CS_PIN  	PINDEF(D, 4) //4
#define TP_IRQ_PIN 	PINDEF(D, 3) //3

#define CASET 0x2A ///< Column Address Set
#define PASET 0x2B ///< Page Address Set
#define RAMWR 0x2C ///< Memory Write
#define RAMRD 0x2E ///< Memory Read

#define TOUCH_THRES 20

void init_TFT_ILI9341();

#endif  //ILI9341_H
