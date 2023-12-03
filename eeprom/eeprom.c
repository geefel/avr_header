#include <avr/interrupt.h>
#include "eeprom.h"

void saveToEEPROM(uint8_t address, uint8_t data) {
	cli();										//auf jeden Fall Interrupts ausschalten
	while (EECR & (1<<EEPE));	//Warte bis vorheriges Schreiben fertig
  EEAR = address;
  EEDR = data;
  EECR |= (1<<EEMPE);				// Setze EEMPE-Bit
  EECR |= (1<<EEPE);				// EEPROM schreiben starten durch Setzen des EEPE-Bits
  sei();
}

uint8_t readFromEEPROM(uint8_t address) {
	cli();										//auf jeden Fall Interrupts ausschalten
	while (EECR & (1<<EEPE));	// Warte bis vorheriges Schreiben fertig
  EEAR = address;
  EECR |= (1<<EERE);				// Starte EEPROM lesen
  sei();
  return EEDR;							// Lese Datenbyte
}
