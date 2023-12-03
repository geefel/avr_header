#ifndef FREE_SANS_24_H
#define FREE_SANS_24_H

#include <avr/pgmspace.h>

typedef struct zeichen_ {
	int8_t  bbx[4];	//breit, hoch, x-offset, y-offset
	uint16_t bitmap[24];
}zeichen_z;

const zeichen_z zahlen[]  PROGMEM = { \
{{15, 24, 1, -1}, {
0x03C0, 0x0FF0, 0x1FF8, 0x3C38, 0x381C, 0x701C, 0x700E, 0x700E, 
0x600E, 0xE00E, 0xE00E, 0xE00E, 0xE00E, 0xE00E, 0xE00E, 0x600E, 
0x700E, 0x700E, 0x701C, 0x381C, 0x3838, 0x1FF8, 0x0FF0, 0x03C0
}}, 
{{8, 23, 3, 0}, {
0x0300, 0x0300, 0x0700, 0x0700, 0x3F00, 0xFF00, 0xFF00, 0x0700, 
0x0700, 0x0700, 0x0700, 0x0700, 0x0700, 0x0700, 0x0700, 0x0700, 
0x0700, 0x0700, 0x0700, 0x0700, 0x0700, 0x0700, 0x0700
}}, 
{{15, 23, 1, 0}, {
0x0380, 0x0FF0, 0x3FFC, 0x381C, 0x700E, 0x700E, 0x700E, 0x600E, 
0x000E, 0x000E, 0x001C, 0x007C, 0x00F8, 0x03E0, 0x0F80, 0x1E00, 
0x3C00, 0x7800, 0x7000, 0x6000, 0xFFFE, 0xFFFE, 0xFFFE
}}, 
{{15, 24, 1, -1}, {
0x03C0, 0x1FF0, 0x3FF8, 0x783C, 0x701C, 0x701C, 0x600E, 0xE00C, 
0x001C, 0x003C, 0x03F8, 0x03F0, 0x03FC, 0x001C, 0x000E, 0x000E, 
0xE00E, 0xE00E, 0xE00E, 0x700E, 0x783C, 0x3FF8, 0x1FF0, 0x0380
}}, 
{{16, 23, 1, 0}, {
0x0030, 0x0070, 0x0070, 0x00F0, 0x01F0, 0x03F0, 0x0370, 0x0770, 
0x0E70, 0x0C70, 0x1C70, 0x3870, 0x7070, 0x6070, 0xE070, 0xFFFF, 
0xFFFF, 0xFFFF, 0x0070, 0x0070, 0x0070, 0x0070, 0x0070
}}, 
{{15, 24, 1, -1}, {
0x3FFC, 0x3FFC, 0x3FFC, 0x3800, 0x3800, 0x3000, 0x3000, 0x3000, 
0x73E0, 0x7FF8, 0x7FFC, 0x781C, 0x700E, 0x000E, 0x000E, 0x0006, 
0x000E, 0xE00E, 0xE00E, 0x701E, 0x783C, 0x3FF8, 0x1FF0, 0x03C0
}}, 
{{15, 24, 1, -1}, {
0x01C0, 0x0FF0, 0x1FF8, 0x3C3C, 0x381C, 0x700E, 0x7000, 0x7000, 
0x6000, 0x63E0, 0xEFF8, 0xFF7C, 0xF81E, 0xF00E, 0xF00E, 0x600E, 
0x600E, 0x700E, 0x700E, 0x780E, 0x3C3C, 0x1FF8, 0x0FF0, 0x03C0
}}, 
{{16, 23, 1, 0}, {
0xFFFF, 0xFFFF, 0xFFFF, 0x000E, 0x000C, 0x001C, 0x0038, 0x0030, 
0x0070, 0x0060, 0x00E0, 0x01C0, 0x01C0, 0x0180, 0x0380, 0x0300, 
0x0700, 0x0700, 0x0600, 0x0E00, 0x0E00, 0x0E00, 0x0E00
}}, 
{{15, 24, 1, -1}, {
0x0180, 0x0FF0, 0x3FF8, 0x383C, 0x701C, 0x700E, 0x700E, 0x700E, 
0x781C, 0x3C3C, 0x1FF0, 0x1FF0, 0x3E7C, 0x701C, 0x700E, 0xE00E, 
0xE006, 0xE00E, 0xE00E, 0x700E, 0x781C, 0x3FFC, 0x1FF0, 0x03C0
}}, 
{{15, 24, 1, -1}, {
0x0380, 0x1FF0, 0x3FF8, 0x783C, 0x701C, 0x700E, 0xE00E, 0xE00E, 
0xE00E, 0xE00E, 0x701E, 0x701E, 0x3EFE, 0x1FFE, 0x0FCE, 0x000E, 
0x000E, 0x000E, 0x701C, 0x701C, 0x3838, 0x3FF0, 0x1FE0, 0x0380
}}, 
{{3, 17, 3, 0}, {
0xE000, 0xE000, 0xE000, 0xE000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xE000, 0xE000, 
0xE000}}
};

#endif	//FREE_SANS_24_H
