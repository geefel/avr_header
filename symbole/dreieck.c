#include "segment14Font.h"
#include "tft.h"

void drawTriangl(uint16_t x, uint16_t y, uint8_t dicke, int16_t color, uint8_t dir) {
	uint8_t anfang, ende, i, j;
	switch (dir) {
		case 0: {             //nach oben
			if (dicke % 2 == 0) {
				anfang = dicke / 2;
				ende = anfang + 1;
			}
			else if (dicke % 2 > 0) {
				anfang = dicke / 2 - 1;
				ende = anfang + 2;
			}
			for (i = 0; i < dicke / 2; ++i, --anfang) {
				for (j = anfang; j < ende; ++j)
					drawPix(x + i, y + j, color);
			}
			for (; i < dicke; ++i, ++anfang) {
				for (j = anfang; j < ende; ++j)
					drawPix(x + i, y + j, color);
			}
			break;
		}
		case 3: {               //nach rechts
			anfang = 0;
			ende = dicke - 1;
			for (i = 0; i < dicke; ++i, ++anfang, --ende) {
				for (j = anfang; j < ende; ++j)
					drawPix(x + i, y + j, color);
			}
			break;
		}
		case 6: {             //nach unten
			anfang = 0;
			ende = 1;
			for (i = 0; i < dicke / 2; ++i, ++ende) {
				for (j = anfang; j < ende; ++j)
					drawPix(x + i, y + j, color);
			}
			for (; i < dicke; ++i, --ende) {
				for (j = anfang; j < ende; ++j)
					drawPix(x + i, y + j, color);
			}
			break;
		}
		case 9: {                 //nach links
			if (dicke % 2 > 0) {
				anfang = dicke / 2;
				ende = anfang + 1;
			}
			else if (dicke % 2 == 0) {
				anfang = dicke / 2 - 1;
				ende = anfang + 2;
			}
			for (i = 0; i < dicke; ++i, --anfang, ++ende) {
				for (j = anfang; j < ende; ++j)
					drawPix(x + i, y + j, color);
			}
		}
		default: break;
	}
}
