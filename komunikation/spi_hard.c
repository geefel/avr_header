#include "spi_hard.h"

void initSPI() {
	setOutput(SCK_PIN);
	setInput(MISO_PIN);
	setOutput(MOSI_PIN);
	setOutput(CS_PIN);

/* SPDR
	 * Datenregister für zu sendende und empfangende Daten
* SPCR-Register
	 * CPOL = 0, Flanke steigend, dann fallend, sonst umgekehrt
	 * CPHA = 0, steigende Flanke abtasten, fallende Flanke setzen, sonst umgekehrt
	 * SPIE = 0 (Interupt disable)
	 * SPE = 1, SPI enable
	 * DORD = 0, MSB first
	 * MSTR = 1,Master-Mode
	 * SPR0, SPR1, Teiler für clock
	 * SPRX (im Register SPSR) Clock * 2
*/
	SPCR = (1<<SPE)|(1<<MSTR)/*|(1<<SPR0)*/;
	//SPSR = 1<<SPI2X;
}

//Auskommentiert: für 4-Draht-Übertragung, hier isDC = 0: data, isDC = 1: command
uint8_t send_Data(
#if defined(DC_PIN)
  uint8_t is_data_command, 
#endif
                          uint8_t val) {
	clr(CS_PIN);
#if defined(DC_PIN)
	if(DC)
		set(DC_PIN);
	else
		clr(DC_PIN);
#endif
  SPDR = val;
  while (!(SPSR & (1<<SPIF)));
	val = SPSR;
	set(CS_PIN);
	return SPDR;
}
