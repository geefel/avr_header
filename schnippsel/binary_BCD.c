#include "binary_BCN.h"

//Convert a binary coded decimal value to binary.
static uint8_t bcd2bin(uint8_t val) {
	return val - 6 * (val >> 4);
}

//Convert a binary value to BCD format.
static uint8_t bin2bcd(uint8_t val) {
	return val + 6 * (val / 10);
}
