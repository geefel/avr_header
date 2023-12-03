#include "punktfont.h"
#include "paint.h"
#include "uart_print.h"



/*
   Zeichen ist max 5 Punkte breit (x-Richtung) und immer 7 Punkte hoch (y-Richtung).
   0-Punkt ist oben links.
   Abstand zwischen Zeichen ist 1 Punkt.
   Leerzeichen wird gesondert behandelt werden, weil Zeichenabstand in drawChar() berechnet wird
   Byte-Array zum festhalten, welcher Punkt pro Zeichen dargestellt werden soll.
*/

void drawStringPF(uint16_t x, uint16_t y, uint8_t r, uint16_t color, char *str) {
  uint8_t strIndex = 0;
  int16_t vers = 0;
  while (str[strIndex] != '\0') {
    vers += drawCharPF(x + vers, y, r, color, str[strIndex]);
    ++strIndex;
  }
}
int16_t drawCharPF(uint16_t x, uint16_t y, uint8_t r, uint16_t color, char ch) {
  uint16_t xVers, yVers;
  uint8_t punktZeile = 0;
  uint8_t zeichenVers = 0;
  uint8_t r2 = r + r / 2;
  uint8_t d = r * 2;                //Durchmesser
  ch -= 32;                         //ASCII fängt mit 32 = 'Leerzeichen' an
  if (ch) {                  //ist ch kein Leerzeichen?
    for (uint8_t i = 0; i < 7; ++i) {
      yVers = y + i * d;                                        //Zeilenversatz
      punktZeile = pgm_read_byte_near(&punkt[(uint8_t)ch][i]);  //i-te Zeile von Zeichen holen,
      if (punktZeile) {                                         //wenn 0b0: dann eben nicht
        for (uint8_t j = 0; j < 5; ++j) {
					xVers = x + j * d;                              			//Spaltenversatz
          if ((punktZeile << j) & 0b10000)                    //Spalte der Zeile von links nach rechts untersuchen ob ein Punkt gezeichnet werden muss
            drawFilledCircle(xVers + r, yVers + r, r, color);
          if ((j + 1) > zeichenVers)                          //ZeichenVersatz ermitteln
              zeichenVers = j + 1;
        }
      }
    }
		
  }
  if ((ch != (46 - 32)) && (ch != (58 -32)) && (ch != 0))
		zeichenVers = (zeichenVers + 1) * d;   //falls kein Leerzeichen
  else if ((ch == (46 - 32)) || (ch == (58 - 32)))
    zeichenVers = (zeichenVers - 3) * d;											//falls . oder :
  else if (ch == 0)
    zeichenVers = (zeichenVers + 4) * d; //falls Leerzeichen
  return zeichenVers;
}


