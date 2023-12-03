#include <avr/pgmspace.h>
#include "drawLetter.h"
#include "tft_ST7735.h"
#include "freeSans_24.h"
//#include "freeSansOriginal.h"
#include "uart_hard.h"


uint8_t getLetterNr(char letter);
void paintChar(uint8_t x, uint8_t y, uint16_t color, char letter);


void drawText(int8_t x, int8_t y, uint16_t color, char *txt) {
	uint8_t strIndex = 0;
	while (txt[strIndex] != '\0') {
		drawChar(x, y, color, txt[strIndex]);
		strIndex++;
	}		
}

//~ void drawChar(int8_t x, int8_t y, uint16_t color, char letter) {
	//~ uint16_t h, w;
	//~ uint16_t h_max;
	//~ uint16_t w_max;
	//~ uint16_t iAr = 0;
	//~ uint8_t maske = 1;
	//~ const tLetter *l;
	//~ l = 0;
	//~ switch (letter) {
		//~ case '0':	l = &freeSans_0x30; break;	//48
		//~ case '1':	l = &freeSans_0x31; break;
		//~ case '2':	l = &freeSans_0x32; break;
		//~ case '3':	l = &freeSans_0x33; break;
		//~ case '4':	l = &freeSans_0x34; break;
		//~ case '5':	l = &freeSans_0x35; break;
		//~ case '6':	l = &freeSans_0x36; break;
		//~ case '7':	l = &freeSans_0x37; break;
		//~ case '8':	l = &freeSans_0x38; break;
		//~ case '9':	l = &freeSans_0x39; break;
		//~ case ':':	l = &freeSans_0x3a; break;	//58
	//~ }
	//~ h_max = l->height;
	//~ w_max = l->width;
	//wenn Fint bitmap ist
	//~ for (h = 0; h < h_max; ++h) {	
		//~ for (w = 0; w < w_max; ++w) {
			//~ drawPix(w + x, h + y, pgm_read_word(&(l->data[iAr++])));
		//~ }
	//~ }
	//~ for (h = 0; h < h_max; h++) {
		//~ for (w = 0; w < w_max; w++) {
			//~ if (l->data[iAr] & maske)
				//~ drawPix(w + x, h + y, color);
			//~ maske <<= 1;
			//~ if (maske == 0) {
				//~ maske = 1;
				//~ iAr++;
			//~ }
		//~ }
		//~ maske = 1;
		//~ iAr++;
	//~ }
//~ }


void drawChar(int8_t x, int8_t y, uint16_t color, char letter) {
	int8_t w, h;
	uint8_t w_max, h_max;
	uint16_t maske = 0x8000;
	uint8_t arrayNr = getLetterNr(letter);
	uint16_t zeile = 0;	
	
	//if (arrayNr != 100) {
		if ((letter > 47) && (letter < 59)) {	//Ziffer oder Doppelpunkt
			w_max = pgm_read_byte(&(zahlen[arrayNr].bbx[0]));	//14, 22, 1, -1
			h_max = pgm_read_byte(&(zahlen[arrayNr].bbx[1]));
			x += 		pgm_read_byte(&(zahlen[arrayNr].bbx[2])); 
			y += 		pgm_read_byte(&(zahlen[arrayNr].bbx[3]));
			
			for (h = 0; h < h_max; ++h) {
				zeile = pgm_read_word(&(zahlen[arrayNr].bitmap[h]));
				for (w = 0; w < w_max; ++w) {
					if (zeile & maske)
						drawPix(x + w, y + h, color);
					maske >>= 1;
				}
				maske = 0x8000;
			}
		//}
		//~ else if ((letter > 96) && (letter < 123)) {	//Kleinbuchstaben
			//~ w_max = pgm_read_byte(&(buchstaben[arrayNr].bbx[0]));
			//~ h_max = pgm_read_byte(&(buchstaben[arrayNr].bbx[1]));
			//~ x += 		pgm_read_byte(&(buchstaben[arrayNr].bbx[2])); 
			//~ y += 		pgm_read_byte(&(buchstaben[arrayNr].bbx[3]));
			
			//~ for (h = 0; h < h_max; --y, ++h) {
				//~ zeile = pgm_read_word(&(buchstaben[arrayNr].bitmap[h]));
				//~ for (w = 0; w < w_max; ++w) {
					//~ if (zeile & maske)
						//~ drawPix(x + w, y, color);
					//~ maske >>= 1;
				//~ }
				//~ maske = 0x8000;
			//~ }
		//~ }
	}
}

uint8_t getLetterNr(char letter) {
	
	uint8_t erg;
	switch (letter) {
		case '0': erg = 0; break;
		case '1': erg = 1; break;
		case '2': erg = 2; break;
		case '3': erg = 3; break;
		case '4': erg = 4; break;
		case '5': erg = 5; break;
		case '6': erg = 6; break;
		case '7': erg = 7; break;
		case '8': erg = 8; break;
		case '9': erg = 9; break;
		case ':': erg = 10; break;
		case 'd': erg = 0; break;
		case 'e': erg = 1; break;
		case 'i': erg = 2; break;
		case 'k': erg = 3; break;
		case 'm': erg = 4; break;
		case 'n': erg = 5; break;
		case 's': erg = 6; break;
		case 't': erg = 7; break;
		default:  erg = 100; break;
	}
	return erg;
}
