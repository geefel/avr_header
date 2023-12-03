#include "drawIcons.h"
#include "tft_ST7735.h"
#include "icon-start-stop-refresh-36.h"
#include "uart_print.h"

void paintIcon(uint8_t x, uint8_t y, const tImage image);

void paintIcon_Start(uint8_t x, uint8_t y) {
	paintIcon(x, y, start_36);
}

void paintIcon_Stop(uint8_t x, uint8_t y) {
	paintIcon(x, y, stop_36);
}

void paintIcon_Refresh(uint8_t x, uint8_t y) {
	paintIcon(x, y, refresh_36);
}

void paintIcon_Glocke_Links(uint8_t x, uint8_t y) {
	paintIcon(x, y, glocke_links_36);
}

void paintIcon_Glocke_Rechts(uint8_t x, uint8_t y) {
	paintIcon(x, y, glocke_rechts_36);
}

void paintIcon(uint8_t x, uint8_t y, const tImage image) {	//x von links nach rechts, y von oben nach unten
	uint16_t h, w;
	uint16_t w_max = image.width;
	uint16_t h_max = image.height;
	uint16_t iAr = 0;	
	for (h = 0; h < h_max; ++h) {	
		for (w = 0; w < w_max; ++w) {
			drawPix(w + x, h + y, pgm_read_word(&(image.data[iAr++])));
		}
	}
}
