#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

void saveToEEPROM(uint8_t address, uint8_t data);
uint8_t readFromEEPROM(uint8_t address);

#endif	//EEPROM_H
