#include "buffer.h"

#define DATA_MAX 16

static uint8_t data[DATA_MAX];

static int8_t dataAnz = -1;

uint8_t setData(uint8_t buf) {
	dataAnz++;
	if (dataAnz > DATA_MAX) {
	  dataAnz--;
	  return 0;
	}
	data[dataAnz] = buf;
	return 1;
}

uint8_t getData(uint8_t *data) {
	dataAnz--;
	if (dataAnz < -1) {
		dataAnz++;
		return 0;
	}
	*data = data[dataAnz + 1];
	return 1;
}

uint8_t getDataAnz() {
	return dataAnz;
}
