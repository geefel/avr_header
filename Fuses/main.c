/*
 * Dies ist ein Programm um Fuses auszulesen.
 * Hat bei mir bisher noch nicht funktioniert.
 */ 

#include <stdio.h>
#include <stdint.h>
#include <avr/boot.h>
int main() {
	uint8_t lf = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
	uint8_t hf = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
	uint8_t ef = boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);
	uint8_t lk = boot_lock_fuse_bits_get(GET_LOCK_BITS);
	printf("lfuse: %d\nhfuse: %d\nefuse: %d\nlook: %d\n", lf, hf, ef, lk);
	return 0;
}
