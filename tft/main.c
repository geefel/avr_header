#include <stdio.h>
#include <util/delay.h>
//#include "uart_print.h"
#include "paint.h"
#include "punktfont.h"
#include "segment14Font.h"
#include "digicoFont.h"
#include "tft.h"
//#include "uart_gfl.h"

#define BAUD 19200

int main(void) {
//Pin-Def in tft.h!
	initForTFT();
	
	//initUart();
	
	//initUartHW(BAUD);

	set_TFT_rotation(90);
	
	uint16_t color1 = 0, color2 = 0, color3 = 0, color4 = 0;
 	uint16_t x = 0, y = 0, z = 0;
	uint8_t r, g, b;
	r = 31;
	g = 63;
	b = 31;
	color1 = (r << 11) | (g << 5) | b;
	r = 31;
	g = 0;
	b = 0;
	color2 = (r << 11) | (g << 5) | b;
	r = 0;
	g = 0x2B;
	b = 0;
	color3 = (r << 11) | (g << 5) | b;
	r = 0;
	g = 0;
	b =0x2B;
	color4 = (r << 11) | (g << 5) | b;
	
	//fillScreen(color1);
	drawFilledRect(0, 0, 240, 320, 0);
	//drawRect(0, 0, 240, 320, 5, color2);
	
	//drawCircle(100, 50, 1, 0);
	drawCircle(100, 75, 2, 0);
	drawCircle(100, 100, 3, 0);
	drawCircle(100, 125, 4, 0);
	drawCircle(100, 150, 5, 0);
	drawCircle(100, 175, 6, 0);
	drawCircle(100, 200, 7, 0);
	drawCircle(100, 225, 8, 0);
	drawCircle(100, 250, 9, 0);
	//drawCircle(100, 275, 10, color1);
	//drawTriangl(100, 100, 30, 0, 6);
	//~ drawText(10, 10, 3, 0, "ZUKI");
	//~ drawText(10, 50, 3, 0, "GHIJK");
	//~ drawText(10, 90, 3, 0, "LMNOP");
	//~ drawText(10, 130, 3, 0, "QRSTU");
	//~ drawText(10, 170, 3, 0, "VWXYZ");
	//~ drawText(10, 210, 3, 0, "12345");
	
	//drawText(10, 10, 25, 0, 0xFFFF, "*");
	
	uint8_t rad = 4;
    char t1[] = "22:12.59";
    char t3[] = "15.02.2022";
    char t2[] = "Donnerstag";
    
    //drawStringDigicoFont(10, 10, 1, 0, color3, 0, t2);
    //~ drawStringDigicoFont(188, 10, 1, 0, color3, 0, t3);
    //~ drawStringDigicoFont(10, 40, 1, 0, color3, 1, t2);
    //~ drawStringDigicoFont(188, 40, 1, 0, color3, 1, t3);
    //~ drawStringPF(10, 70, 1, 0, t2);
    //~ drawStringPF(188, 70, 1, 0, t3);
    
    //~ drawText(10, 10, 1, 0, color3, "DONNERSTAG");
    uint16_t colorbackgr = 0x0560;
    uint16_t colorMenue = 0xFE20;
    uint16_t colorMenueSelected = 0xFFFF;
    uint16_t colorClock = 0x630C;
    uint16_t colorTimer = 0xF803;
    
//    drawFilledRect(0, 0, 320, 240, colorbackgr);
//    drawStringDigicoFont(10, 10, 1, colorMenueSelected, colorbackgr, 0, "LS 16");
//    drawStringDigicoFont(110, 10, 1, colorMenue, colorbackgr, 0, "Freq 123,9");
//    drawStringDigicoFont(270, 10, 1, colorMenue, colorbackgr, 0, "DAB");
//    drawStringDigicoFont(10, 40, 1, colorMenue, colorbackgr, 0, "Menue");
//    drawStringDigicoFont(270, 40, 1, colorMenue, colorbackgr, 0, "Aus");
//    
//    drawStringDigicoFont(125, 45, 1,colorTimer, colorbackgr, 1, "W 08:30");
    
    drawStringDigicoFont(10, 90, 1, colorClock, colorbackgr, 1, "Heute im Radio: Radio!");
    
    //drawText(15, 160, 6, colorClock, color3, "22");
    //drawText(100, 160, 6, colorClock, color3, ":");
    //drawText(115, 160, 6, colorClock, color3, "12");
    //drawText(195, 160, 6, colorClock, color3, ".");
    //drawText(215, 160, 6, colorClock, color3, "22");
    
    //drawStringPF(40, 40, 3, color2, t1);
    //~ drawText(40, 40, 3, color2, color3, t1);
    //~ drawText(5, 10, 2, 0, color3, t2);
    //~ drawText(170, 10, 2, 0, color3, t3);
    //~ drawStringPF(0, rad * 60, rad, 0, "uvwxyz");
    //~ drawStringPF(0, rad * 80, rad, 0, "*+-/");
    //drawChar(0, rad * 80, rad, 0, '_');
    //drawText(0, 0, 15, 0,  0xFFFF, "*");
	//drawTriangl(10, 100, 50, 0, 0);
	//drawTriangl(60, 100, 50, 0, 3);
	//drawTriangl(110, 100, 50, 0, 6);
	//drawTriangl(280, 100, 50, 0, 9);
	
	while(1) {
	drawText(10, 10, 3, 0, color3, "21-00.99:88");
	_delay_ms(1000);
	drawText(10, 10, 3, 0, color3, "22.01:11-77");
	_delay_ms(1000);
	
	drawText(10, 10, 20, 0, color3, "0");
	_delay_ms(1000);
	drawText(10, 10, 20, 0, color3, "1");
	_delay_ms(1000);
	drawText(10, 10, 20, 0, color3, "2");
	_delay_ms(1000);
	drawText(10, 10, 20, 0, color3, "3");
	_delay_ms(1000);
	drawText(10, 10, 20, 0, color3, "4");
	_delay_ms(1000);
	drawText(10, 10, 20, 0, color3, "5");
	_delay_ms(1000);
	drawText(10, 10, 20, 0, color3, "6");
	_delay_ms(1000);
	drawText(10, 10, 20, 0, color3, "7");
	_delay_ms(1000);
	drawText(10, 10, 20, 0, color3, "8");
	_delay_ms(1000);
	drawText(10, 10, 20, 0, color3, "9");
	_delay_ms(1000);
	for (int i = 0; i < 10; ++i) {
		setBacklightDim(i);
		_delay_ms(500);
	}
	for (int i = 10; i > 0; --i) {
		setBacklightDim(i);
		_delay_ms(500);
	}

		//_delay_ms(20);
		//fillRect(10, 10, 50, 100, color2);
		//_delay_ms(200);
		//fillRect(10, 10, 50, 100, color3);
		//_delay_ms(200);
		//fillRect(10, 10, 50, 100, color4);
		//_delay_ms(200);


	}
	return 0;
}
