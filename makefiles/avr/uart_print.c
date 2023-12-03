#include "uart_hard.h"
//#include "uart_soft.h"

#include <util/delay.h>
#include <stdlib.h>

void send(const char txt);

void send(const char txt) {
	uart_Transmit(txt);	//für uart_hart.h
//	sendDat(txt);				//für uart_soft.h
}

void pText(const char *txt) {
	uint8_t i = 0;
	while (txt[i] != '\0'){
		send(txt[i]);
		++i;
		_delay_ms(1);
	}
}

void pTextn(const char *txt) {
	pText(txt);
	send('\n');
}

void pItoA(int16_t x) {
	char txt[18];
	itoa(x, txt, 10);
	pText(txt);
}

void pItoAn(int16_t x) {
	char txt[18];
	itoa(x, txt, 10);
	pTextn(txt);
}

void pInt(uint8_t x) {
	send(x);
}

void pIntn(uint8_t x) {
	send(x);
	pTextn("");
}
