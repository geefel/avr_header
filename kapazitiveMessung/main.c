/*
 * Kapazitive Messung:
 * Hier: D2 (Arduino D2) als Output, D3 (Arduino D3) als Input.
 * Zw. Output und Input ein Widerstand > 1MOhm.
 * Am Input ein Metallblech (leitend) anschließen, der den Sensor darstellt.
 * Output wechselt Zustand, nach einer Zeit t wechselt der Zustand am Eingang.
 * t ist abhängig von R und C (t = R * C). R ist der Widerstand und C ist das Metalblech.
 * Diese ist veränderlich, je nachdem wie nahe z.B. eine Hand dem Blech kommt.
 */ 

#include <stdint.h>
#include <util/delay.h>
#include "zeitMessen.h"
#include "pin.h"
#include "uart_hard.h"
#include "uart_print.h"
#define KAPA_OUTPIN PINDEF(D, 2)
#define KAPA_INPIN  PINDEF(D, 3)

uint16_t zaehlen();

void setupKapazi() {
	setupTimer();
	setOutput(KAPA_OUTPIN);
	setInput(KAPA_INPIN);
	setPin(KAPA_OUTPIN);
	
	//Zähler einrichten
}

uint16_t zaehlen() {
	resetTime();
	togglePin(KAPA_OUTPIN);
	uint8_t status = getPin(KAPA_INPIN);
	startTimer();
	while(getPin(KAPA_INPIN) == status) {
		;
	}
	stopTimer();
	return getTime();
}


int main() {
	uint16_t erg[32];
	uint8_t i = 0;
	uint16_t e = 0;
	initUartHW(9600);
	setupKapazi();
	_delay_ms(1000);
	while(1) {
		//~ pText("ti: ");
		//~ pItoA(zaehlen());
		//~ pText("  In: ");
		zaehlen();
		erg[i++] = (getPin(KAPA_INPIN));
		if (i == 32) {
			for (int j = 0; j < 32; j++)
				e += erg[j];
			i = 0;
			pText("Durchsch: ");
			pItoAn(e / 32);
			e = 0;
			
		}
		_delay_ms(500);
	}
	
	return 0;
}
