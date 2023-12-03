#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include "drawGeometric.h"
#include "drawLetter.h"
#include "tft.h"
#include "uart_hard.h"

#define BAUD 19200

#define WHITE         0x0000
#define BLACK         0xFFFF    
#define BLUE          0x001F  
#define BRED          0XF81F
#define GRED          0XFFE0
#define GBLUE         0X07FF
#define RED           0xF800
#define MAGENTA       0xF81F
#define GREEN         0x07E0
#define CYAN          0x7FFF
#define YELLOW        0xFFE0
#define BROWN         0XBC40 
#define BRRED         0XFC07 
#define GRAY          0X8430 
#define DARKBLUE      0X01CF  
#define LIGHTBLUE     0X7D7C   
#define GRAYBLUE      0X5458 
#define LIGHTGREEN    0X841F 
#define LGRAY         0XC618 
#define LGRAYBLUE     0XA651
#define LBBLUE        0X2B12 
#define LETTER_COL		0x0AA0

#define D_P_COL_FLAG_O		(1 << 8)
#define D_P_COL_FLAG_M		(1 << 7)
#define D_P_COL_FLAG_U		(1 << 6)
#define FLAG_REG					GPIOR0

#define POS_X_Z					50
#define POS_Y_Z_OBEN		100
#define POS_Y_Z_MITTE		60
#define POS_Y_Z_UNTEN		20

#define POS_X_T					73
#define POS_Y_T_OBEN		87
#define POS_Y_T_MITTE		47
#define POS_Y_T_UNTEN		7

static struct zeit_ {
	uint16_t zeitOben, zeitMitte, zeitUnten;
	char zeitObenAr[6], zeitMitteAr[6], zeitUntenAr[6];
	char zeitObenAltAr[6], zeitMitteAltAr[6], zeitUntenAltAr[6]; 
} zeit;

void drawDPunkt(uint8_t pos, uint8_t onOff);
void drawScreenText(uint8_t pos, char txt);
void intToTime(uint16_t i, char t[6]);
uint16_t timeToInt(char t[6]);

//t[0]: sekEiner, t[1]: sekZehner, t[2]: minEinser, ...
void intToTime(uint16_t i, char t[6]) {
	t[0] = i % 10;
	i /= 10;
	t[1] = i % 6;
	i /= 6;
	t[2] = i % 10;
	i /= 10;
	t[3] = i % 6;
	i /= 6;
	t[4] = i % 10;
	i /= 10;
	t[5] = i;
}

//t[0]: sekEiner, t[1]: sekZehner, t[2]: minEinser, ...
uint16_t timeToInt(char t[6]) {
	return t[0] + t[1] * 10 + t[2] * 10 * 6 + t[3] * 10 * 6 * 10 + \
				 t[4] * 10 * 6 * 10 * 6 + t[5] * 10 * 6 * 10 * 6 * 10; 
}

/*
 * pos: POS_Y_T_OBEN, POS_Y_T_MITTE oder POS_Y_T_UNTEN
 * onOff: on: 1, off 2
 */
void drawDPunkt(uint8_t pos, uint8_t onOff) {
	if (onOff)
		drawChar(POS_X_Z + 60, pos, LETTER_COL, ':');
	else
		drawChar(POS_X_Z + 60, pos, BLACK, ':');		
}

/*
 * pos: POS_Y_T_OBEN, POS_Y_T_MITTE oder POS_Y_T_UNTEN
 * zahl: die darzustellende zahl
 * zahlAlt: die Zahl mit der Ziffer gelöscht werden soll
 * stelle: von rechts nach links mit sekundenEinser = 0 beginnend
 */
void drawZiffer(uint8_t pos, uint8_t zahl, char *zahlAlt, uint8_t stelle) {
	switch (stelle) {
		case 0:
			drawChar(POS_X_Z + 90, pos, BLACK,  *zahlAlt + 48);
			drawChar(POS_X_Z + 90, pos, LETTER_COL, zahl + 48);
			*zahlAlt = zahl;
			break;
		case 1:
			drawChar(POS_X_Z + 70, pos, BLACK,  *zahlAlt + 48);
			drawChar(POS_X_Z + 70, pos, LETTER_COL, zahl + 48);
			*zahlAlt = zahl;
			break;
		case 2:
			drawChar(POS_X_Z + 40, pos, BLACK,  *zahlAlt + 48);
			drawChar(POS_X_Z + 40, pos, LETTER_COL, zahl + 48);
			*zahlAlt = zahl;
			break;
		case 3:
			drawChar(POS_X_Z + 20, pos, BLACK,  *zahlAlt + 48);
			drawChar(POS_X_Z + 20, pos, LETTER_COL, zahl + 48);
			*zahlAlt = zahl;
			break;
	}
}

/*
 * pos: POS_Y_T_OBEN, POS_Y_T_MITTE oder POS_Y_T_UNTEN
 * txt: s für std/min, m für min/sec 
 */
void drawScreenText(uint8_t pos, char txt) {
	if (txt == 'm') {
		drawChar(POS_X_T + 12, pos, BLACK, 's');
		drawChar(POS_X_T + 20, pos, BLACK, 't');
		drawChar(POS_X_T + 26, pos, BLACK, 'd');
		
		drawChar(POS_X_T + 59, pos, BLACK, 'm');
		drawChar(POS_X_T + 71, pos, BLACK, 'i');
		drawChar(POS_X_T + 75, pos, BLACK, 'n');	
		
		drawChar(POS_X_T + 10, pos, LETTER_COL, 'm');
		drawChar(POS_X_T + 22, pos, LETTER_COL, 'i');
		drawChar(POS_X_T + 26, pos, LETTER_COL, 'n');
		
		drawChar(POS_X_T + 59, pos, LETTER_COL, 's');
		drawChar(POS_X_T + 67, pos, LETTER_COL, 'e');
		drawChar(POS_X_T + 75, pos, LETTER_COL, 'k');		
	}
	else if (txt == 's') {
		drawChar(POS_X_T + 10, pos, BLACK, 'm');
		drawChar(POS_X_T + 22, pos, BLACK, 'i');
		drawChar(POS_X_T + 26, pos, BLACK, 'n');
		
		drawChar(POS_X_T + 60, pos, BLACK, 's');
		drawChar(POS_X_T + 68, pos, BLACK, 'e');
		drawChar(POS_X_T + 76, pos, BLACK, 'k');
		
		drawChar(POS_X_T + 12, pos, LETTER_COL, 's');
		drawChar(POS_X_T + 20, pos, LETTER_COL, 't');
		drawChar(POS_X_T + 26, pos, LETTER_COL, 'd');
		
		drawChar(POS_X_T + 60, pos, LETTER_COL, 'm');
		drawChar(POS_X_T + 72, pos, LETTER_COL, 'i');
		drawChar(POS_X_T + 76, pos, LETTER_COL, 'n');	
	}
}


int main(void) {
	
	initUartHW(BAUD);
	
//Pin-Def in tft.h!
	initForTFT();
	fillScreen(BLACK);
	
	
	zeit.zeitOben = 210;
	zeit.zeitMitte = 600;
	zeit.zeitUnten = 2400;
	FLAG_REG = 0;

	//Doppelpunkte
	drawDPunkt(POS_Y_Z_OBEN, 1);
	drawDPunkt(POS_Y_Z_MITTE, 1);
	drawDPunkt(POS_Y_Z_UNTEN, 1);

	//Sec-, Min- und Std-Textzeile
	drawScreenText(POS_Y_T_OBEN, 'm');
	drawScreenText(POS_Y_T_MITTE, 'm');
	drawScreenText(POS_Y_T_UNTEN, 'm');
	
	intToTime(zeit.zeitOben, zeit.zeitObenAr);
	intToTime(zeit.zeitMitte, zeit.zeitMitteAr);
	intToTime(zeit.zeitUnten, zeit.zeitUntenAr);
	
	//die Zeitziffern mit den Werten aus dem EEProm
	for (uint8_t i = 0; i < 4; ++i) {
		drawZiffer(POS_Y_Z_OBEN,  zeit.zeitObenAr[i],  &zeit.zeitObenAltAr[i],  i);
		drawZiffer(POS_Y_Z_MITTE, zeit.zeitMitteAr[i], &zeit.zeitMitteAltAr[i], i);
		drawZiffer(POS_Y_Z_UNTEN, zeit.zeitUntenAr[i], &zeit.zeitUntenAltAr[i], i);
	}
	
	while(1) {
		intToTime(--zeit.zeitOben, zeit.zeitObenAr);
		for (uint8_t i = 0; i < 4; ++i) 
			drawZiffer(POS_Y_Z_OBEN, zeit.zeitObenAr[i], &zeit.zeitObenAltAr[i], i);
		
		//~ sE--;
		//~ if (sE == -1) {
			//~ sE = 9;
			//~ drawChar(x + 90, y + 20, BLACK, sEo + 48);
			//~ drawChar(x + 90, y + 20, LETTER_COL, sE + 48);
			
			//~ sZ--;			
			//~ if (sZ == -1) {
				//~ sZ = 5;
				//~ drawChar(x + 70, y + 20, BLACK, sZo + 48);
				//~ drawChar(x + 70, y + 20, LETTER_COL, sZ + 48);
				
				//~ mE--;
				//~ if(mE == -1) {
					//~ mE = 9;
					//~ drawChar(x + 40, y + 20, BLACK, mEo + 48);
					//~ drawChar(x + 40, y + 20, LETTER_COL, mE + 48);
					
					//~ mZ--;
					//~ drawChar(x + 20, y + 20, BLACK, mZo + 48);
					//~ drawChar(x + 20, y + 20, LETTER_COL, mZ + 48);
				//~ }
				//~ else {
					//~ drawChar(x + 40, y + 20, BLACK, mEo + 48);
					//~ drawChar(x + 40, y + 20, LETTER_COL, mE + 48);
				//~ }
			//~ }
			//~ else {
				//~ drawChar(x + 70, y + 20, BLACK, sZo + 48);
				//~ drawChar(x + 70, y + 20, LETTER_COL, sZ + 48);
			//~ }
		//~ }
		//~ else {
			//~ drawChar(x + 90, y + 20, BLACK, sEo + 48);
			//~ drawChar(x + 90, y + 20, LETTER_COL, sE + 48);
		//~ }

	_delay_ms(1000);
	//~ for (int i = 0; i < 10; ++i) {
		//~ setBacklightDim(i);
		//~ _delay_ms(500);
	//~ }
	//~ for (int i = 10; i > 0; --i) {
		//~ setBacklightDim(i);
		//~ _delay_ms(500);
	//~ }
	}
	return 0;
}

