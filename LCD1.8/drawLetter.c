#include <avr/pgmspace.h>
#include "drawLetter.h"
#include "tft.h"
#include "freeSans.h"
#include "uart_hard.h"


uint8_t getLetterNr(char letter);

void drawText(int8_t x, int8_t y, uint16_t color, char *txt) {
	uint8_t strIndex = 0;
	while (txt[strIndex] != '\0') {
		drawChar(x, y, color, txt[strIndex]);
		strIndex++;
	}		
}


void drawChar(int8_t x, int8_t y, uint16_t color, char letter) {
	int8_t i, j, z, s, x2;
	uint8_t width, hight;
	uint16_t maske = 0x8000;
	uint8_t arrayNr = getLetterNr(letter);
	uint16_t zeile = 0;
	if (arrayNr != 100) {
		if ((letter > 47) && (letter < 59)) {	//Ziffer oder Doppelpunkt
			width = pgm_read_byte(&(zahlen[arrayNr].bbx[0]));	//14, 22, 1, -1
			hight = pgm_read_byte(&(zahlen[arrayNr].bbx[1]));
			x += 		pgm_read_byte(&(zahlen[arrayNr].bbx[2])); 
			y += 		pgm_read_byte(&(zahlen[arrayNr].bbx[3])) + hight;
			
			for (z = 0; z < hight; --y, ++z) {
				zeile = pgm_read_word(&(zahlen[arrayNr].bitmap[z]));
				x2 = x;
				for (s = 0; s < width; ++x2, ++s) {
					if (zeile & maske)
						drawPix(y, x2, color);
					maske >>= 1;
				}
				maske = 0x8000;
			}
		}
		else if ((letter > 96) && (letter < 123)) {	//Kleinbuchstaben
			width = pgm_read_byte(&(buchstaben[arrayNr].bbx[0]));
			hight = pgm_read_byte(&(buchstaben[arrayNr].bbx[1]));
			x += 		pgm_read_byte(&(buchstaben[arrayNr].bbx[2])); 
			y += 		pgm_read_byte(&(buchstaben[arrayNr].bbx[3])) + hight;
			
			for (z = 0; z < hight; --y, ++z) {
				zeile = pgm_read_word(&(buchstaben[arrayNr].bitmap[z]));
				x2 = x;
				for (s = 0; s < width; ++x2, ++s) {
					if (zeile & maske)
						drawPix(y, x2, color);
					maske >>= 1;
				}
				maske = 0x8000;
			}
		}
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
