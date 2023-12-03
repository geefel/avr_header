#include <stdio.h>
#include <stdint.h>
#include "uart_hard.h"
#include "tft_ST7735.h"
//#include "paintColor.h"
#include "drawIcons.h"
#include "drawLetter.h"

/*
 * aktive Fläche von (2,2) bis (129,160)
 * 
 * x = Columns, width, Spalte, breite		__von links nach rechts__
 * y = Row, Lines, height, high, Zeile	__von oben nach unten__
 * DrawWindow muss mit verändert werden, da sich die Abmaße auch drehen
 *
 * Beschriftung unten
 * 
 * Memory Access Control MADCTL
 * D7 MY Page Address Order: 0 = Top to Botton, 1 = umgekehrt
 * D6 MX Column Address Order: 0 Left to Right, 1 = umgekehrt
 * D5 MV Page/Column Order: 0 = normal, 1 = revers
 * D4 ML Line Address Order: 0 = Refresh top to Bottom,1 = umgekehrt
 * D3 RGB RGB/BGR Order: 0 = RGB,1 = BGR
 * D2 MH Display Data Latch Order: 0 = Refresh Left to Right, 1 = umgekehrt
 * MY MX MV 
 * 0  0  0  normal							(0,0) links oben	drawWindow(0, 0, 128, 160, 0x0);
 * 0  1  1  90° drehen					(0,0) rechts oben	drawWindow(0, 0, 160, 128, 0x0);
 * 1  1  0  180° drehen					(0,0) unten rechts
 * 1  0  1  270° drehen					(0,0) unten links
 * send_LCD_Register(0x36, 0b);
*/
int main() {
	initUartHW(19200);
	init_Pins();
	init_SPI();
	setMADCTL(0b01100000);
	init_TFT();
	set_LCD_BL(1);
	//setBacklightDim(0);
	drawWindow(0, 0, 160, 129, 0x0);
	
	paintIcon_Start(2, 2);
	paintIcon_Refresh(2, 45);
	paintIcon_Stop(2, 90);
	paintIcon_Glocke_Links(50, 2);
	paintIcon_Glocke_Rechts(50, 45);
	//~ drawPix(2, 2, 0xffff);
	//~ drawPix(10,80,0xffff);
	//~ paintIcon_Stop(44, 2);
	//~ paintIcon_Refresh(87, 2);
	drawChar(2, 2, 0xffff, '2');
	
	
	while (1);
	return 0;
}
