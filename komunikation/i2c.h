#ifndef I2C_H
#define I2C_H

#include <stdint.h>

#define CLOCK_NORMAL 100000
#define CLOCK_FAST   400000

void set_I2C_Clock_Pin(uint32_t frequency);
void sendDataI2C(uint8_t deviceAddress, uint8_t registerStartNr, uint8_t *data, uint8_t numOfData);
void getDataI2C(uint8_t deviceAddress, uint8_t registerStartNr, uint8_t *data, uint8_t numOfData);

#endif //I2C_H
