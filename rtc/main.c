#include <stdio.h>
#include <stdint.h>
#include "i2c.h"
#include "rtc.h"
int main() {
	set_I2C_Clock(100000);
	setClock();
	setSquarOn(1);
	return 0;
}
