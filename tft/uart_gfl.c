/*
   9600 8N1 heißt Baudrate (= bitrate) 9600, 8 bit Datenlänge, no parity, 1 Stopbit
   7E1 heißt 7 bit Datenlänge, gerade parity, 1 Stopbit
   9O1 9 bit Datenlänge, odd parity, 1 Stopbit
   Unterschieden wird zwischen zwei Paritätsprotokollen, in beiden Fällen wird die Anzahl
   der Einsen innerhalb des zugeordneten Wortes ermittelt:

   Parität „even“ (gerade):
   Ist eine ungerade Anzahl Datenbits im Datum gesetzt, so wird das Paritätsbit gesetzt.
   Ist eine gerade Anzahl Datenbits im Datum gesetzt, so wird das Paritätsbit nicht gesetzt.

   Parität „odd“ (ungerade):
   Ist eine gerade Anzahl Datenbits im Datum gesetzt, so wird das Paritätsbit gesetzt.
   Ist eine ungerade Anzahl Datenbits im Datum gesetzt, so wird das Paritätsbit nicht gesetzt.

   Mit „gerade“ oder „ungerade“ ist also die Anzahl gesetzter Datenbits
   inklusive des Paritätsbits gemeint.
*/

#include "uart_gfl.h"

uint8_t bit_count = 0;
uint8_t parity = 0;
uint16_t txrx_maske = 1;
uint16_t data = 0;
uint16_t prescal;
uint16_t ocr;
uint8_t txrx;
uint16_t rx_data[64];
uint8_t rx_data_count = 0;

/***********TIMER***************/
void initTimer(uint16_t ocr_) {	//Timer 2
	TCCR2A = 0;
	TCCR2B = 0;
	TCCR2A = 1 << WGM21;			//CTC
	TCNT2  = 0;								//der eigentliche Zähler
	OCR2A = ocr_;							//Vergleichszahl-A
	OCR2B = ocr_ / 2;					//Vergleichszahl-B
	//TIMSK2 |= (1 << OCIE2A);	//Interrupt Vergleichszahl-A freigeben
	//TIMSK2 |= (1 << OCIE2B);	//Interrupt Vergleichszahl-B freigeben
}
void setTimerTime(uint8_t full) {
	if (full)
		TIMSK2 |= (1 << OCIE2A);	//Interrupt Vergleichszahl-A freigeben
	else
		TIMSK2 |= (1 << OCIE2B);	//Interrupt Vergleichszahl-B freigeben
}
void startTimer(uint16_t prescal) {
	switch (prescal) {
		case 1024: TCCR2B |= (1 << CS22) || (1 << CS20); return;
		case 256:  TCCR2B |= (1 << CS22);                return;
		case 64: 	 TCCR2B |= (1 << CS21) || (1 << CS20); return;
		case 8:    TCCR2B |= (1 << CS21);                return;
		case 1:    TCCR2B |= (1 << CS20);                return;
	}
}
void stopTimer() {
	TCCR2B &= ~((1 << CS22) || (1 << CS21) || (1 << CS20));		
}

/**************Pin-Interrupt****************/
void stopPinchange(PARAMETERLISTE) {
	switch (*PORT) {
		case 1: PCMSK0 &= ~(1 << *PIN); return;
		case 2: PCMSK1 &= ~(1 << *PIN); return;
		case 3: PCMSK1 &= ~(1 << *PIN); return;
	}
}
void startPinchange(PARAMETERLISTE) {
	switch (*PORT) {
		case 1: PCMSK0 |= (1 << *PIN); return;
		case 2: PCMSK1 |= (1 << *PIN); return;
		case 3: PCMSK1 |= (1 << *PIN); return;
	}
}
/****************************************/

void getPrescaleOcr(uint16_t *prescal, uint16_t *ocr) {
	*prescal = 0;
	*ocr = 0;
#if F_CPU == UL16000000
	switch (BAUD_1) {
		case 50: *ocr = 312; *prescal = 1024; break;
		case 110: *ocr = 141; *prescal = 1024; break;
		case 150: *ocr = 103; *prescal = 1024; break;
		case 300: *ocr = 207; *prescal = 256; break;
		case 1200: *ocr = 207; *prescal = 64; break;
		case 2400: *ocr = 103; *prescal = 64; break;
		case 4800: *ocr = 51; *prescal = 64; break;
		case 9600: *ocr = 207; *prescal = 8; break;
		case 19200: *ocr = 103; *prescal = 8; break;
		case 38400: *ocr = 51; *prescal = 8; break;
		case 57600: *ocr = 34; *prescal = 8; break;
		case 115200: *ocr = 138; *prescal = 1; break;
		case 230400: *ocr = 68; *prescal = 1; break;
		case 460800: *ocr = 34; *prescal = 1; break;
		case 921600: *ocr = 16; *prescal = 1; break;
		case 2000000: *ocr = 7; *prescal = 1; break;
		case 3000000: *ocr = 4; *prescal = 1; break;
	}
#endif
}

//Parität-bit-Berechnung	Todo: even/odd/no
uint8_t getParity (uint16_t p, uint8_t len) {
	uint8_t tst = 0;
	uint8_t mask = 1;
  for (uint8_t i = 0; i < len; ++i) {
    if (mask & p)
      tst++;
    mask <<= 1;
  }
  tst %= 2;
  return tst;	//bei ungerade: 1, gerade: 0
}

void initUart() {
	cli();
  setOutput(TX_PIN_1);
  setInput(RX_PIN_1);
  setPin(TX_PIN_1);
	setPin(RX_PIN_1);
	getPrescaleOcr(&prescal, &ocr);
	initTimer(ocr);
	setTimerTime(1);
	sei();
	
	startPinchange(RX_PIN_1);
}

void sendDat(uint16_t dat) {
	data = dat;
	txrx = 1;
	data |= getParity(dat, PARITY_1) << (DATA_LENGTH_1 + 1);	//Todo: even/odd/no
	
	clrPin(TX_PIN_1);	//start
	
	startTimer(prescal);
	while (bit_count != DATA_LENGTH_1 + 1)	//Todo: even/odd/no
		;
	setPin(TX_PIN_1);
	stopTimer();
	txrx_maske = 1;
	bit_count = 0;
	data = 0;
}

void sendDataAs8(uint8_t d) {
	uint16_t dat = d;
	sendDat(dat); 
}
void sendDataAs16(uint16_t d) {
	sendDat(d);
}
void sendDataAsArr(uint16_t *d, uint16_t lenge) {
	for (uint8_t i = 0; i < lenge; ++i) {
		sendDat(d[i]);
	}
}

void reseiveData() {
	if (bit_count != DATA_LENGTH_1 + 1)	{//Todo: even/odd/no
		data = getPin(RX_PIN_1) | txrx_maske;
		bit_count++;
		txrx_maske <<= 1; 
	}
	else {
		stopTimer();
		rx_data[rx_data_count] = data;	//Todo: paritycheck
		txrx_maske = 1;
		bit_count = 0;
		data = 0;
		startPinchange(RX_PIN_1);
	}
}

/************Interrupts*************/
ISR(PCINT0_vect) {
	setTimerTime(0);	//auf halbe bit-Zeit-Länge setzen
	startTimer(prescal);
	stopPinchange(RX_PIN_1);
}
ISR(PCINT1_vect) {
	setTimerTime(0);
	startTimer(prescal);
	stopPinchange(RX_PIN_1);
}
ISR(PCINT2_vect) {
	setTimerTime(0);
	startTimer(prescal);
	stopPinchange(RX_PIN_1);
}

ISR(TIMER2_COMPA_vect) {
	if (txrx) {								//transmit
		if (data && txrx_maske)
			setPin(TX_PIN_1);
		else
			clrPin(TX_PIN_1);
		txrx_maske <<= 1;
	}
	else {										//reseive
		reseiveData();
	}
}

ISR(TIMER2_COMPB_vect) {		//nur reseive
	setTimerTime(1);	//auf bit-Zeit-Länge setzen
}