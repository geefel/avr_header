#include <stdint.h>
#include <stdio.h>

#define KLASSISCH
//#define ZU_FUSS

#ifdef ZU_FUSS
uint32_t dividieren32(uint32_t *z, uint32_t m) {
	uint32_t count = 0;
	if(*z > m) {		
		do {
			count++;
		} while((*z -= m) >= m);
	}
	return count;
}
uint16_t dividieren16(uint16_t *z, uint16_t m) {
	uint16_t count = 0;
	if(*z > m) {		
		do {
			count++;
		} while((*z -= m) >= m);
	}
	return count;
}
uint8_t dividieren8(uint8_t *z, uint8_t m) {
	uint8_t count = 0;
	if(*z > m) {		
		do {
			count++;
		} while((*z -= m) >= m);
	}
	return count;
}

void zuFuss(uint32_t fre, char *txt) {
		/**
	 * Aus einer Zahl (65535 < Zahl < 131072) ASCII machen.
	 * Die letzten beiden Ziffern sind falsch!	4,9
	 */
	union{
		uint32_t f;
		uint8_t b[4];	//LSB first
	} freq;
	
	freq.f = fre;
	
	uint16_t fr = freq.b[1];	//letzte beiden Bytes zusammenfassen
	fr <<= 8;
	fr += freq.b[0];
	fr >>= 1; 								//und durch 2 teilen (aus z.B. 91200 wird 12832)
	fr |= 0x8000;							//die 0x01 aus freq[2] dazu tun (= 45600)
	fr = dividieren16(&fr, 10);	// = 4560
	fr <<= 1;									// = 9120 (d.i. 91200/10, aber nur in 16bit gerechnet)
	
	uint8_t i = 5;
	txt[i++] = dividieren16(&fr, 10000) + 48;
	if(txt[i -1] == '0')
		i--;
	txt[i++] = dividieren16(&fr, 1000) + 48;
	txt[i++] = dividieren16(&fr, 100) + 48;
	txt[i++] = ',';
	txt[i++] = dividieren16(&fr, 10) + 48;
	//txt[i++] = fr + 48;
}
#endif	//ZU_FUSS

#ifdef KLASSISCH
void klassisch(uint32_t fr, char *txt) {
	uint8_t i = 5;
	txt[i++] = (fr / 100000) + 48;
	if(txt[i -1] == '0')
		i--;
	txt[i++] = (fr / 10000 % 10) + 48;
	txt[i++] = (fr / 1000 % 10) + 48;
	txt[i++] = ',';
	txt[i++] = (fr / 100 % 10) + 48;
}
#endif	//KLASSISCH

int main(void) {
	uint32_t fr = 91200;
	char txt[15]  = "Freq ";
	
#ifdef KLASSISCH
	klassisch(fr, txt);
#endif
#ifdef ZU_FUSS
	zuFuss(fr, txt);
#endif

	printf("%s", txt);
}
