/*
 * von http://www.eckart-winkler.de/computer/c/varargs.htm
 * 
 * Formatzeichen:
 * %c = Char
 * %s = String
 * %d = Integer
 * char *txt
 * 
 * *txt ist der Text in dem Formatzeichen eingebettet sein können,
 * deren Parameter hinter *txt, durch Kommatas separiert, angegeben werden.
 * Beispiel:
 * out("test %c, %s, %d", 'c', "String", 123);
 */

#include <stdint.h>
#include "print.h"
#include "uart_attiny.h"

void send(uint8_t b) {
	sendDat(b);
}

void out(char *txt, ...) {
  va_list args;
  char *pt;
  char ch;
  char *str;
  int n;
  char back[10];
	char *ph = back;
	int h;

  va_start(args, txt);  //Parameterabfrage initialisieren
  for (pt = txt; *pt; pt++) {
    if (*pt != '%') {		//Alle Zeichen ausser % ausgeben 
			send(*pt);
			continue;
		}
    switch (*(++pt)) {	//Formatkennzeichner abarbeiten
      case 'c':
        ch = (char)va_arg(args, int);
        send(ch); 
        break;
      
      case 's':
        str = (char*)va_arg(args, int*);
        while(*str) {
					send(*str);
					str++;
				} 
        break;
      
      case 'd':
        n = (int)va_arg(args, int);
				if(!n) {					//Gleich null?
					send('0'); 
					return; 
				}
				if(n < 0) {				//Vorzeichen?
					send('-'); 
					n = -n; 
				}
				while(n) { 				//Ziffern rueckwaerts nach back schreiben
					h = n % 10; 
					n = n / 10; 
					*ph++ = (char)('0' + h); 
				}
				while(ph > back) 	//Nun Ziffern ausgeben
					send(*(--ph));
        break;
      case 0:
        return;
      default:
        send(*pt); 
        break;
    }
  }
  va_end(args);						//Parameterabfrage beenden und Schluss
  return;
}
