/*
 * Software UART
 * RX-Pin, TX-Pin, Timer, RX-Pin-Interrupt
 * 
 * 1. init baud, byte-length, parität, bitZähler = 0
 * 2. init Timer
 * 3. init Pin-Interrupt
 * 4. Init RX-Pin auf HIGH
 * 5. init TX-Pin auf HIGH
 * 
 * Senden über TX-Pin:
 * wir haben uint16_t zum senden
 * 1. parität berechnen, Parität-bit speichern
 * 2. Start-bit: TX = LOW
 * 3. Timer starten
 * 4. auf timer-Interrupt warten
 * 5. TX-Pin setzen auf byte16_t[bitZähler++]
 * 6. while (bitzähler < byte-length) goto 4 else
 * 7. Stop-bit: TX = HIGH
 * 8. Timer stoppen
 * 9. bitzähler = 0
 * 10. maske = 1
 * 
 * Empfangen über RX-Pin, Interrupt-Pin ist startklar
 * Interrupt-Pin wird ausgelöst
 * 1. Interrupt-Pin deaktivieren
 * 2. Timer starten mit halber bit-Zeit
 * 3. Timer-Interrupt
 * 4. Timer auf ganze bit-Zeit stellen
 * 5. Timer-Interrupt
 * 6. while (bitzähler < byte-length + 1) byte16_t[bitzähler++] = Status(RX-Pin) goto 5 else
 * 7. Timer-Interrupt stoppen
 * 8. Interrupt-Pin aktivieren
 * 9. bitzähler = 0
 * 10. Parität prüfen
 * 11. byte16_t in grösseres Array speichern
 *  
 * Als erstes hier die Ports usw. definieren, soviel mam will.
 * #define TX_PIN1 PINDEF(B, 3)  //11 PORT0  SDATA
 * #define RX_PIN1 PINDEF(B, 4)  //12
 * #define BAUD_1 9600
 * #define DATA_LENGTH_1 8  //7, 8 oder 9
 * #define PARITY_1 2       //0: kein, 1: odd, 2: even
 * 
 * Dann #define UART_n erstellen.
 * #define UART_1 TX_PIN1,RX_PIN1,BAUD_1,DATALENGTH_1,PARITY_1
 * 
 * Dann im Scatch oder wo auch immer:
 * 
 * #include "uart_gfl.h"
 * 
 * void setup() {
 *  setUartOutIn(UART_n);
 *  setUartOutIn(UART_2);  //wenn man zwei hat
 * }
 * 
 * Dann: 
 * 
 * void sendData8(UART_n, uint8_t dat);    //oder
 * void sendDataAr(UART_n, uint8_t *dat, uint8_t size_of_dat);
 * 
 * Wenn man über mehrere Funktionen gehen will immer uart_gfl.h includieren, 
 * der erste Aufrufer benutzt 
 * foo(UART_n);
 * dann:
 * void foo(PARAMETER_ALL) {uint8_t data = 0x16; sendData8(PARA_ALL, data);}  //PARAMETER_ALL und PARA_ALL müssen benutzt werden!
 * 
 */
 
#ifndef UART_GFL_H
#define UART_GFL_H

#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>

extern uint16_t rx_data[64];
extern uint8_t rx_data_count;
/*
 * maske_ = 1 wird in nächster Zeile geschiftet und später in z.B. uint8_t getPin(PARAMETERLISTE) benutzt
 */
static __attribute__((unused)) uint8_t maske_ = 1;
/*
 * Makro um z.B. &DDRB,&PORTB,PINB,0b00010000 zu kreieren
 */
#define PINDEF(PORTNR, PINNR) &DDR##PORTNR,&PORT##PORTNR,&PIN##PORTNR,maske_<< PINNR
/*
 * Makros um z.B. TX_PIN durch &DDRB,&PORTB,PINB,0b00010000 zu ersetzen
 */

#define RX_PIN_1 PINDEF(D, 0)  //12
#define TX_PIN_1 PINDEF(D, 1)  //11
#define BAUD_1 19200
#define DATA_LENGTH_1 8  //7, 8 oder 9
#define PARITY_1 2       //0: kein, 1: odd, 2: even

/*
 * Makro um die Parameter für die Funktionen in ein Wort zusammenzufassen
 */
#define PARAMETERLISTE \
  volatile __attribute__((unused)) uint8_t *DDR, \
  volatile __attribute__((unused)) uint8_t *PORT, \
  volatile __attribute__((unused)) uint8_t *PIN,  \
  volatile __attribute__((unused)) uint8_t maske_

static inline uint8_t getPin(PARAMETERLISTE) {
  return (*PIN & maske_) ? 1 : 0;
}
static inline void setOutput(PARAMETERLISTE) {
  *DDR |= maske_;
}
static inline void setInput(PARAMETERLISTE) {
  *DDR &= ~maske_;
}
static inline void setInputPullup(PARAMETERLISTE) {
  *DDR &= ~maske_;
  *PORT |= maske_;
}
static inline void setPin(PARAMETERLISTE) {
  *PORT |= maske_;
}
static inline void clrPin(PARAMETERLISTE) {
  *PORT &= ~maske_;
}
static inline void togglePin(PARAMETERLISTE) {
  *PORT ^= maske_;
}

void initUart();
void sendDataAs8(uint8_t d);
void sendDataAs16(uint16_t d);
void sendDataAsArr(uint16_t *d, uint16_t lenge);
ISR(PCINT0_vect);
ISR(PCINT1_vect);
ISR(PCINT2_vect);
ISR(TIMER0_COMPA_vect);
ISR(TIMER2_COMPA_vect);

#endif  //UART_GFL_H
