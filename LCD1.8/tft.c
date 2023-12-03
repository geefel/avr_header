
/*
 *Für ST7735
*/

#include <stdint.h>
#include <util/delay.h>
#include "tft.h"

uint16_t xDispMax;
uint16_t yDispMax;
uint16_t dispRotation;

uint8_t get_TP_IRQ();
uint8_t get_MISO();
void set_MOSI(uint8_t mosi);
void set_Data_Command(uint8_t lcd_dc);
void set_LCD_CS(uint8_t lcd_cs);
void set_LCD_BL(uint8_t lcd_bl);
void set_SD_CS(uint8_t sd_cs);
void set_TP_CS(uint8_t tp_cs);
void set_TP_IRQ(uint8_t tp_irq);

void send_LCD_Register(uint8_t reg, uint8_t dat);
uint8_t get_LCD_Val(uint8_t command_data, uint8_t bef);

//#if defined(SPI_hard) 	//in tft.h
uint8_t send_LCD_Command_Data(uint8_t command_data, uint8_t val);
//~ #else
//~ void send_LCD_Command_Data(uint8_t command_data, uint8_t val);
//~ #endif

static const uint8_t isData = 1;
static const uint8_t isCommand = 0;
static const uint8_t isRegister = 0;
static const uint8_t high = 1;
static const uint8_t low = 0;
static const uint16_t xrawmax = 2096,	//aus Versuchen ermitteln
											yrawmax = 2096,
											xrawmin = 176,
											yrawmin = 224;
					   
void init_Pins() {
	setOutput(SCK_PIN);
	//setInput(MISO_PIN);
	setOutput(RESET_PIN);
	setOutput(DIN_PIN);
	setOutput(CS_PIN);
	setOutput(BL_PIN);
	setOutput(DC_PIN);
	//~ setOutput(SD_CS_PIN);
	//~ setOutput(TP_CS_PIN);
	//~ setInput(TP_IRQ_PIN);
}

//~ #if defined(SPI_hard)	//in tft.h
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
	 * SPRX (im Register SPSR) Cloxk * 2
	 */
void init_SPI() {
	//ZUERST die Pins definieren!!!!!
	//SPCR = (1<<SPE)|(1<<MSTR);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPI2X)|(1<<CPOL)|(1<<CPHA);
	//SPSR = 1<<SPI2X;
}
//~ #endif

/*
 * Beschriftung unten
	Memory Access Control MADCTE
	D7 MY Page Address Order: 0 = Top to Botton, 1 = umgekehrt
	D6 MX Column Address Order: 0 Left to Right, 1 = umgekehrt
	D5 MV Page/Column Order: 0 = normal, 1 = revers
	D4 ML Line Address Order: 0 = Refresh top to Bottom,1 = umgekehrt
	D3 RGB RGB/BGR Order: 0 = RGB,1 = BGR
	D2 MH Display Data Latch Order: 0 = Refresh Left to Right, 1 = umgekehrt
	MY MX MV 
	0  0  0  normal							x von li nach re, y von oben nach unten
	0  1  1  90° drehen					x von re nach li, y von oben nach unten
	1  1  0  180° drehen				x von un nach ob, y von rech nach links
	1  0  1  270° drehen
	send_LCD_Register(0x36, 0b);
	*/

void init_TFT() {
	_delay_ms(200);
	clrPin(RESET_PIN);
	_delay_ms(200);
	setPin(RESET_PIN);
	_delay_ms(200);

	send_LCD_Command_Data(isCommand, 0x36);		//MADCTL
	send_LCD_Command_Data(isData, 0b11000000); 

	send_LCD_Command_Data(isCommand, 0x3A); 	//COLMOD: Pixel Format Set
	send_LCD_Command_Data(isData, 0x05);

	send_LCD_Command_Data(isCommand, 0x21); 	//INVON Display Inversion On

	send_LCD_Command_Data(isCommand, 0x2A);		//CASET Column Address Set
	send_LCD_Command_Data(isData, 0x00);
	send_LCD_Command_Data(isData, 0x01);
	send_LCD_Command_Data(isData, 0x00);
	send_LCD_Command_Data(isData, 0x80);

	send_LCD_Command_Data(isCommand, 0x2B);		//RASET Row Address Set
	send_LCD_Command_Data(isData, 0x00);
	send_LCD_Command_Data(isData, 0x00);
	send_LCD_Command_Data(isData, 0x00);
	send_LCD_Command_Data(isData, 0xA0);

	send_LCD_Command_Data(isCommand, 0xB2);		//FRMCTR2 In Idle Mode (8-colors)
	send_LCD_Command_Data(isData, 0x0C);
	send_LCD_Command_Data(isData, 0x0C);
	send_LCD_Command_Data(isData, 0x00);
	send_LCD_Command_Data(isData, 0x33);
	send_LCD_Command_Data(isData, 0x33);

	send_LCD_Command_Data(isCommand, 0xB7);		//
	send_LCD_Command_Data(isData, 0x35); 

	send_LCD_Command_Data(isCommand, 0xBB);		//
	send_LCD_Command_Data(isData, 0x1F);

	send_LCD_Command_Data(isCommand, 0xC0);		//PWCTR1 Power Control Setting
	send_LCD_Command_Data(isData, 0x2C);

	send_LCD_Command_Data(isCommand, 0xC2);		//PWCTR3 In Normal Mode (Full Clors)	(???)
	send_LCD_Command_Data(isData, 0x01);

	send_LCD_Command_Data(isCommand, 0xC3);		//PWCTR4 In Idle Mode (8-colors)
	send_LCD_Command_Data(isData, 0x12);   

	send_LCD_Command_Data(isCommand, 0xC4);		//PWCTR5 In Partial Mode + Full colors
	send_LCD_Command_Data(isData, 0x20);

	send_LCD_Command_Data(isCommand, 0xC6);		//
	send_LCD_Command_Data(isData, 0x0F); 

	send_LCD_Command_Data(isCommand, 0xD0);		//
	send_LCD_Command_Data(isData, 0xA4);
	send_LCD_Command_Data(isData, 0xA1);

	send_LCD_Command_Data(isCommand, 0xE0);		//GAMCTRP1 Gamma adjustment
	send_LCD_Command_Data(isData, 0xD0);
	send_LCD_Command_Data(isData, 0x08);
	send_LCD_Command_Data(isData, 0x11);
	send_LCD_Command_Data(isData, 0x08);
	send_LCD_Command_Data(isData, 0x0C);
	send_LCD_Command_Data(isData, 0x15);
	send_LCD_Command_Data(isData, 0x39);
	send_LCD_Command_Data(isData, 0x33);
	send_LCD_Command_Data(isData, 0x50);
	send_LCD_Command_Data(isData, 0x36);
	send_LCD_Command_Data(isData, 0x13);
	send_LCD_Command_Data(isData, 0x14);
	send_LCD_Command_Data(isData, 0x29);
	send_LCD_Command_Data(isData, 0x2D);

	send_LCD_Command_Data(isCommand, 0xE1);		//GAMCTRN1 Gamma adjustment
	send_LCD_Command_Data(isData, 0xD0);
	send_LCD_Command_Data(isData, 0x08);
	send_LCD_Command_Data(isData, 0x10);
	send_LCD_Command_Data(isData, 0x08);
	send_LCD_Command_Data(isData, 0x06);
	send_LCD_Command_Data(isData, 0x06);
	send_LCD_Command_Data(isData, 0x39);
	send_LCD_Command_Data(isData, 0x44);
	send_LCD_Command_Data(isData, 0x51);
	send_LCD_Command_Data(isData, 0x0B);
	send_LCD_Command_Data(isData, 0x16);
	send_LCD_Command_Data(isData, 0x14);
	send_LCD_Command_Data(isData, 0x2F);
	send_LCD_Command_Data(isData, 0x31);
	
	//send_LCD_Command_Data(isCommand, 0x21); 	//INVON Display Inversion On (2 mal?)

	send_LCD_Command_Data(isCommand, 0x11);		//SLPOUT Sleep Out & Booster On
	//send_LCD_Command_Data(isCommand, 0x10);		//SLPIN Sleep In & Booster Off

	send_LCD_Command_Data(isCommand, 0x29);		//DISPON Main screen turn on
	//send_LCD_Command_Data(isCommand, 0x28);		//Main screen turn off
	set_LCD_BL(high);
}




//def ist Rotation = 0°
void set_TFT_rotation(uint16_t grad) {
	dispRotation = grad;
	switch (grad) {
		case 0: 	
			send_LCD_Register(0x36, 0b00000000);
			xDispMax = 128;
			yDispMax = 160; 
			break;
		case 90: 	
			send_LCD_Register(0x36, 0b01100000);
			xDispMax = 160;
			yDispMax = 128; 
			break;
		case 180: 
		send_LCD_Register(0x36, 0b11000000);
			xDispMax = 128;
			yDispMax = 160; 
			break;
		case 270: 
		send_LCD_Register(0x36, 0b10100000);
			xDispMax = 160;
			yDispMax = 128;  
			break;
		default: break;
	};
}

//~ void init_Touch() {
	//~ set_TP_CS(high);
	//~ set_TP_IRQ(high);
//~ }

//~ inline uint8_t get_MISO() {
	//~ return getPin(MISO_PIN);
//~ }

//~ static uint8_t get_TP_IRQ() {
	//~ return getPin(TP_IRQ_PIN);
//~ }

inline void set_MOSI(uint8_t mosi) {
	if (mosi)
		setPin(DIN_PIN);
	else
		clrPin(DIN_PIN);
}

inline void set_Data_Command(uint8_t lcd_dc) {
	if (lcd_dc)
		setPin(DC_PIN);
	else
		clrPin(DC_PIN);
}

inline void set_LCD_CS(uint8_t lcd_cs) {
	if (lcd_cs)
		setPin(CS_PIN);
	else
		clrPin(CS_PIN);
}

inline void set_LCD_BL(uint8_t lcd_bl) {
	if (lcd_bl)
		setPin(BL_PIN);
	else
		clrPin(BL_PIN);
}

//~ inline void set_SD_CS(uint8_t sd_cs) {
	//~ if (sd_cs)
		//~ setPin(SD_CS_PIN);
	//~ else
		//~ clrPin(SD_CS_PIN);
//~ }

//~ inline void set_TP_CS(uint8_t tp_cs) {
	//~ if (tp_cs)
		//~ setPin(TP_CS_PIN);
	//~ else
		//~ clrPin(TP_CS_PIN);
//~ }

//~ inline void set_TP_IRQ(uint8_t tp_irq) {
	//~ if (tp_irq)
		//~ setPin(TP_IRQ_PIN);
	//~ else
		//~ clrPin(TP_IRQ_PIN);
//~ }

void send_LCD_Register(uint8_t reg, uint8_t dat) {
	send_LCD_Command_Data(isRegister, reg);
	send_LCD_Command_Data(isData, dat); 
}

//~ #if defined(SPI_hard)	//in tft.h
//SPI-Version
/*
 * CS und Command/Data werden von SPI nicht gesetzt, deshalb selber machen.
 * SPSR und SPDR müssen ausgelesen werden, sonst wird SPIF nicht gelöscht.
 */
uint8_t send_LCD_Command_Data(uint8_t is_command_data, uint8_t val) {
	set_LCD_CS(low);
	set_Data_Command(is_command_data);
  SPDR = val;
  while (!(SPSR & (1<<SPIF)));
	val = SPSR;
	set_LCD_CS(high);
	return SPDR;
}

/*
 * CS und Command/Data werden von SPI nicht gesetzt, deshalb selber machen.
 * SPSR und SPDR müssen ausgelesen werden, sonst wird SPIF nicht gelöscht.
 */
//~ uint8_t get_LCD_Val(uint8_t is_command_data, uint8_t val) {
	//~ set_LCD_CS(low);
	//~ set_Data_Command(is_command_data);
  //~ SPDR = val;
//~ //send command
  //~ while (!(SPSR & (1<<SPIF)));
	//~ val = SPSR;
	//~ SPDR = 0;
//~ //reseive data
	//~ while (!(SPSR & (1<<SPIF)));
	//~ val = SPSR;
	//~ set_LCD_CS(high);
	//~ return SPDR;
//~ }

//~ uint16_t get_Touch_Val(uint16_t command) {
	//~ uint16_t erg = 0;
	//~ set_TP_CS(low);
  //~ SPDR = command;
  //~ asm volatile("nop");
  //~ while (!(SPSR & (1<<SPIF)));
  //~ erg = SPDR;
  //~ SPDR = 0;
  //~ asm volatile("nop");
  //~ while (!(SPSR & (1<<SPIF)));
  //~ erg = SPDR << 4;
  //~ SPDR = 0;
  //~ asm volatile("nop");
  //~ while (!(SPSR & (1<<SPIF)));
  //~ erg += SPDR;
  //~ set_TP_CS(high); 
	//~ return erg;
//~ }

//~ void getTouchPos(uint16_t *x, uint16_t *y, uint16_t *z) {
	//~ int16_t xraw = 0, yraw = 0;
	//~ switch (dispRotation) {
		//~ case 0:
			//~ xraw = get_Touch_Val(0xD1);
			//~ yraw = get_Touch_Val(0x91);
			//~ break;
		//~ case 90:
			//~ xraw = get_Touch_Val(0x91);
			//~ yraw = yrawmax - get_Touch_Val(0xD1);
			//~ break;
		//~ case 180:																	//?????????? Testen!
			//~ xraw = xrawmax - get_Touch_Val(0xD1);
			//~ yraw = yrawmax - get_Touch_Val(0x91);
			//~ break;
		//~ case 270:																	//?????????? Testen!
			//~ xraw = xrawmax - get_Touch_Val(0x91);
			//~ yraw = get_Touch_Val(0xD1);
			//~ break;
		//~ default: break;
	//~ }
	//~ *z = get_Touch_Val(0xB0);	//>200
	//~ //*z2 = get_Touch_Val(0xC0);

	//~ *x = (int32_t)(xraw - xrawmin) * xDispMax / (int32_t)(xrawmax - xrawmin);
	//~ *y = (int32_t)(yraw - yrawmin) * yDispMax / (int32_t)(yrawmax - yrawmin);
	//~ *x = xDispMax - *x;
	//~ *y = yDispMax - *y;
//~ }

//~ #else
//~ /*
 //~ * Software-SPI
 //~ * is_command_data: ob Command (auch bei Register-Nr.) oder Data gesendet wird
 //~ * val: Command/Register oder Data
 //~ */
//~ void send_LCD_Command_Data(uint8_t is_command_data, uint8_t val) {	
	
	//~ set_LCD_CS(low);
	//~ clrPin(SCK_PIN);
	//~ set_Data_Command(is_command_data);

	//~ if (val & 0b10000000)
		//~ setPin(DIN_PIN);
	//~ else
		//~ clrPin(DIN_PIN);
	//~ setPin(SCK_PIN);
	//~ clrPin(SCK_PIN);
	
	//~ if (val & 0b01000000)
		//~ setPin(DIN_PIN);
	//~ else
		//~ clrPin(DIN_PIN);
	//~ setPin(SCK_PIN);
	//~ clrPin(SCK_PIN);
	
	//~ if (val & 0b00100000)
		//~ setPin(DIN_PIN);
	//~ else
		//~ clrPin(DIN_PIN);
	//~ setPin(SCK_PIN);
	//~ clrPin(SCK_PIN);
	
	//~ if (val & 0b00010000)
		//~ setPin(DIN_PIN);
	//~ else
		//~ clrPin(DIN_PIN);
	//~ setPin(SCK_PIN);
	//~ clrPin(SCK_PIN);
	
	//~ if (val & 0b00001000)
		//~ setPin(DIN_PIN);
	//~ else
		//~ clrPin(DIN_PIN);
	//~ setPin(SCK_PIN);
	//~ clrPin(SCK_PIN);
	
	//~ if (val & 0b00000100)
		//~ setPin(DIN_PIN);
	//~ else
		//~ clrPin(DIN_PIN);
	//~ setPin(SCK_PIN);
	//~ clrPin(SCK_PIN);
	
	//~ if (val & 0b00000010)
		//~ setPin(DIN_PIN);
	//~ else
		//~ clrPin(DIN_PIN);
	//~ setPin(SCK_PIN);
	//~ clrPin(SCK_PIN);
	
	//~ if (val & 0b00000001)
		//~ setPin(DIN_PIN);
	//~ else
		//~ clrPin(DIN_PIN);
	//~ setPin(SCK_PIN);
	//~ clrPin(SCK_PIN);

	//~ set_LCD_CS(high);
//~ }

//~ uint8_t get_LCD_Val(uint8_t is_command_data, uint8_t val) {
	//~ uint8_t maske = 0b10000000;
	//~ uint8_t erg = 0;
	
	//~ set_LCD_CS(low);
	//~ set_Data_Command(is_command_data);
	
	//~ do {
		//~ if (val & maske)
			//~ setPin(DIN_PIN);
		//~ else
			//~ clrPin(DIN_PIN);
		//~ setPin(SCK_PIN);
		//~ clrPin(SCK_PIN);
		//~ maske >>= 1;
	//~ } while (maske > 0);
	
	//~ setPin(DIN_PIN);		//ist besser
	//~ maske = 0b10000000;
	
	//~ do {
		//~ setPin(SCK_PIN);
		//~ if (getPin(MISO_PIN))
			//~ erg |= maske;
		//~ clrPin(SCK_PIN);
		//~ maske >>= 1;
	//~ } while(maske > 0);
	
	//~ set_LCD_CS(high);
	//~ return erg;
//~ }

//~ #endif


void drawWindow(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, uint16_t color) {
	uint32_t width = x1 - x + 1;
	uint32_t hight = y1 - y + 1;
	
	send_LCD_Command_Data(isCommand, CASET);	//colum address set
	send_LCD_Command_Data(isData, (x >> 8) & 0xFF);
	send_LCD_Command_Data(isData, x & 0xFF);
	send_LCD_Command_Data(isData, (x1 >> 8) & 0xFF);
	send_LCD_Command_Data(isData, x1 & 0xFF);
	send_LCD_Command_Data(isCommand, RASET);	//row address set
	send_LCD_Command_Data(isData, (y >> 8) & 0xFF);
	send_LCD_Command_Data(isData, y & 0xFF);
	send_LCD_Command_Data(isData, (y1 >> 8) & 0xFF);
	send_LCD_Command_Data(isData, y1 & 0xFF);
	
	send_LCD_Command_Data(isCommand, RAMWR);	//hier speichern der Farbe
	for (uint32_t i = 0; i < width * hight; ++i) {
		send_LCD_Command_Data(isData, (color >> 8) & 0xFF);
		send_LCD_Command_Data(isData, color & 0xFF);
	}
}

void drawPix(uint16_t x, uint16_t y, uint16_t color) {
	send_LCD_Command_Data(isCommand, CASET);	//colum address set
	send_LCD_Command_Data(isData, (x >> 8) & 0xFF);
	send_LCD_Command_Data(isData, x & 0xFF);
	send_LCD_Command_Data(isCommand, RASET);	//row address set
	send_LCD_Command_Data(isData, (y >> 8) & 0xFF);
	send_LCD_Command_Data(isData, y & 0xFF);
	send_LCD_Command_Data(isCommand, RAMWR);	//hier speichern der Farbe
	send_LCD_Command_Data(isData, (color >> 8) & 0xFF);
	send_LCD_Command_Data(isData, color & 0xFF);
}

uint8_t dimLevel[] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 255};

void setBacklightDim(int8_t dim) {
	dim = dim > 9 ? 9 : dim < 0 ? 0 : dim;
	TCCR1A |= (1<<COM1A1) | (1<<WGM10);  //Set Timer Register 16-Bit Timer 1 an PB1
	TCCR1B |= (1<<WGM12) | (1<<CS11); 
	OCR1A = dimLevel[dim];
}

