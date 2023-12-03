#ifndef DRAW_LETTER_H
#define DRAW_LETTER_H

#include <stdint.h>

void drawChar(int8_t x, int8_t y, uint16_t color, char letter);
void drawText(int8_t x, int8_t y, uint16_t color, char *txt);

#endif	//DRAW_LETTER_H
