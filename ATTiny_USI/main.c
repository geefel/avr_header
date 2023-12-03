/*
 *                       ATMEL ATTINY 25/45/85. Ich habe 45er
 *
 *                                     +-\/-+
 * PCINT5 !RESET dW         ADC0 PB5  1|    |8  VCC
 * PCINT3 XTAL1  CLKI !OC1B ADC3 PB3  2|    |7  PB2 SCK  USCK SCL ADC1 T0    INT0 PCINT2 
 * PCINT4 XTAL2  CLKO  OC1B ADC2 PB4  3|    |6  PB1 MISO DO       AIN1 OC0B  OC1A PCINT1 
 *                               GND  4|    |5  PB0 MOSI DI   SDA AIN0 OC0A !OC1A PCINT0 AREF
 *                                     +----+
 *                                 Arduinobelegung
 *                                     +-\/-+
 *                        Digi 0 PB5  1|    |8  VCC
 *                        Digi 1 PB3  2|    |7  PB2 Digi 4 Analog 2 
 *               Analog 1 Digi 2 PB4  3|    |6  PB1 Digi 3 Analog 3
 *                               GND  4|    |5  PB0 Digi 5 Analog 0
 *                                     +----+
*/ 


// siehe auch http://127.0.1.1/dokuwiki/doku.php?id=attiny_serial_monitor
#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include "pin.h"
#include "usi_attiny.h"
#define LED_PIN PINDEF(B, 3)

int main() {
	initUsi();
	setOutput(LED_PIN);
	_delay_ms(2000);
	while(1) {
		setPin(LED_PIN);
		pText("~1");
		pN();
		_delay_ms(1000);
		clrPin(LED_PIN);
		pText("~2");
		pN();
		_delay_ms(1000);
	}
	return 0;
}
