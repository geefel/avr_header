
/*
 *Für ST7789
*/

#include <stdint.h>
#include <util/delay.h>
#include "tft.h"
#include "pin.h"

#include "print.h"

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

#if defined(SPI_hard) 	//in tft.h
uint8_t send_LCD_Command_Data(uint8_t is_command_data, uint8_t val);
#else
void send_LCD_Command_Data(uint8_t command_data, uint8_t val);
#endif

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
	setInput(MISO_PIN);
	setOutput(MOSI_PIN);
	setOutput(LCD_CS_PIN);
	setOutput(LCD_BL_PIN);
	setOutput(LCD_DC_PIN);
	setOutput(SD_CS_PIN);
	setOutput(TP_CS_PIN);
	setInput(TP_IRQ_PIN);
}

#if defined(SPI_hard)	//in tft.h
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
	SPCR = (1<<SPE)|(1<<MSTR)/*|(1<<SPR1)|(1<<SPR0)*/;
	//SPSR = 1<<SPI2X;
}
#endif

/*
	Memory Access Control MADCTE
	D7 MY Page Address Order: 0 = Top to Botton, 1 = umgekehrt
	D6 MX Column Address Order: 0 Left to Right, 1 = umgekehrt
	D5 MV Page/Column Order: 0 = normal, 1 = revers
	D4 ML Line Address Order: 0 = Refresh top to Bottom,1 = umgekehrt
	D3 RGB RGB/BGR Order: 0 = RGB,1 = BGR
	D2 MH Display Data Latch Order: 0 = Refresh Left to Right, 1 = umgekehrt
	MY MX MV 
	0  0  0  normal				
	0  1  1  90° drehen
	1  1  0  180° drehen
	1  0  1  270° drehen
	send_LCD_Register(0x36, 0b01100000);
	*/
void init_TFT() {	//Für ST7789
	set_Data_Command(isData);
	set_LCD_CS(high);
	set_LCD_BL(low);
	set_Data_Command(isCommand);
	set_LCD_BL(high);
	
	send_LCD_Command_Data(isCommand, 0x11);	//<Sleep out	ST7789/ILI9341
	_delay_ms(6);
	send_LCD_Command_Data(isCommand, 0x21); //Revers color, muss sein, keine Ahnung warum
	send_LCD_Command_Data(isCommand, 0x29);	//Display on	ST7789/ILI9341
	send_LCD_Register(0x3a, 0x55);	//< COLMOD: Pixel Format Set	ST7789/ILI9341
//	send_LCD_Register(0x36, 0x08);	//< Memory Access Control Drehrichtung und RGB	ILI9341
//	send_LCD_Register(0xc0, 0b00111100);	//< Power Control   ILI9341 0b00111100
//	send_LCD_Register(0xc2,0x01);	//< Power Control 3 ILI9341
	set_TFT_rotation(270);
	set_LCD_BL(high);
}

//def ist Rotation = 0°
void set_TFT_rotation(uint16_t grad) {
	dispRotation = grad;
	switch (grad) {
		case 0: 	
			send_LCD_Register(0x36, 0b00000000);
			xDispMax = 240;
			yDispMax = 320; 
			break;
		case 90: 	
			send_LCD_Register(0x36, 0b01100000);
			xDispMax = 320;
			yDispMax = 240; 
			break;
		case 180: 
		send_LCD_Register(0x36, 0b11000000);
			xDispMax = 240;
			yDispMax = 320; 
			break;
		case 270: 
		send_LCD_Register(0x36, 0b10100000);
			xDispMax = 320;
			yDispMax = 240;  
			break;
		default: break;
	};
}

/*This command causes the LCD module to enter the minimum power consumption mode. 
 * In this mode e.g. the DC/DC converter is stopped, Internal oscillator is stopped, 
 * and panel scanning is stopped.
 * MCU interface and memory are still working and the memory keeps its contents.
 * Sleep In Mode can only be left by the Sleep Out Command (11h).
 * Wait 120msec after sending Sleep Out command (when in Sleep In Mode) before Sleep In command can be sent.
 */
void set_Sleep_On_TFT() {
	send_LCD_Command_Data(isCommand, 0x10);
	set_LCD_BL(low);
}
/*
 * Wenn TFT aufwacht braucht es 5 ms um default values zu laden, dann erst werden andere Befehle
 * anerkannt. 
 */
void set_Sleep_Off_TFT() {
	init_TFT();
}

void init_Touch() {
	set_TP_CS(high);
	set_TP_IRQ(high);
}

inline uint8_t get_MISO() {
	return getPin(MISO_PIN);
}

inline uint8_t get_TP_IRQ() {
	return getPin(TP_IRQ_PIN);
}

inline void set_MOSI(uint8_t mosi) {
	if (mosi)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
}

inline void set_Data_Command(uint8_t lcd_dc) {
	if (lcd_dc)
		setPin(LCD_DC_PIN);
	else
		clrPin(LCD_DC_PIN);
}

inline void set_LCD_CS(uint8_t lcd_cs) {
	if (lcd_cs)
		setPin(LCD_CS_PIN);
	else
		clrPin(LCD_CS_PIN);
}

inline void set_LCD_BL(uint8_t lcd_bl) {
	if (lcd_bl)
		setPin(LCD_BL_PIN);
	else
		clrPin(LCD_BL_PIN);
}

inline void set_SD_CS(uint8_t sd_cs) {
	if (sd_cs)
		setPin(SD_CS_PIN);
	else
		clrPin(SD_CS_PIN);
}

inline void set_TP_CS(uint8_t tp_cs) {
	if (tp_cs)
		setPin(TP_CS_PIN);
	else
		clrPin(TP_CS_PIN);
}

inline void set_TP_IRQ(uint8_t tp_irq) {
	if (tp_irq)
		setPin(TP_IRQ_PIN);
	else
		clrPin(TP_IRQ_PIN);
}

void send_LCD_Register(uint8_t reg, uint8_t dat) {
	send_LCD_Command_Data(isRegister, reg);
	send_LCD_Command_Data(isData, dat); 
}

#if defined(SPI_hard)	//in tft.h
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

void send_LCD_Command_NrOfData(uint8_t command, uint8_t dat[6], uint8_t numOfData) {
	uint8_t val = 0;
	++val;
	set_LCD_CS(low);
	set_Data_Command(isCommand);
	SPDR = command;
  while (!(SPSR & (1<<SPIF)));
	val = SPSR;
	val = SPDR;
	
	for (uint8_t i = 0; i < numOfData; ++i) {
		SPDR = dat[i];
		while (!(SPSR & (1<<SPIF)));
		val = SPSR;
		val = SPDR;
	}
	set_LCD_CS(high);
}

/*
 * CS und Command/Data werden von SPI nicht gesetzt, deshalb selber machen.
 * SPSR und SPDR müssen ausgelesen werden, sonst wird SPIF nicht gelöscht.
 */
uint8_t get_LCD_Val(uint8_t is_command_data, uint8_t val) {
	set_LCD_CS(low);
	set_Data_Command(is_command_data);
  SPDR = val;
//send command
  while (!(SPSR & (1<<SPIF)));
	val = SPSR;
	SPDR = 0;
//reseive data
	while (!(SPSR & (1<<SPIF)));
	val = SPSR;
	set_LCD_CS(high);
	return SPDR;
}

uint16_t get_Touch_Val(uint16_t command) {
	uint16_t erg = 0;
	set_TP_CS(low);
  SPDR = command;
  asm volatile("nop");
  while (!(SPSR & (1<<SPIF)));
  erg = SPDR;
  SPDR = 0;
  asm volatile("nop");
  while (!(SPSR & (1<<SPIF)));
  erg = SPDR << 4;
  SPDR = 0;
  asm volatile("nop");
  while (!(SPSR & (1<<SPIF)));
  erg += SPDR;
  set_TP_CS(high); 
	return erg;
}

void getTouchPos(uint16_t *x, uint16_t *y, uint16_t *z) {
	int16_t xraw = 0, yraw = 0;
	switch (dispRotation) {
		case 0:
			xraw = get_Touch_Val(0xD1);
			yraw = get_Touch_Val(0x91);
			break;
		case 90:
			xraw = get_Touch_Val(0x91);
			yraw = yrawmax - get_Touch_Val(0xD1);
			break;
		case 180:																	//?????????? Testen!
			xraw = xrawmax - get_Touch_Val(0xD1);
			yraw = yrawmax - get_Touch_Val(0x91);
			break;
		case 270:																	//?????????? Testen!
			xraw = xrawmax - get_Touch_Val(0x91);
			yraw = get_Touch_Val(0xD1);
			break;
		default: break;
	}
	*z = get_Touch_Val(0xB0);	//>200
	//*z2 = get_Touch_Val(0xC0);

	*x = (int32_t)(xraw - xrawmin) * xDispMax / (int32_t)(xrawmax - xrawmin);
	*y = (int32_t)(yraw - yrawmin) * yDispMax / (int32_t)(yrawmax - yrawmin);
	*x = xDispMax - *x;
	*y = yDispMax - *y;
}

#else
/*
 * Software-SPI
 * is_command_data: ob Command (auch bei Register-Nr.) oder Data gesendet wird
 * val: Command/Register oder Data
 */
void send_LCD_Command_Data(uint8_t is_command_data, uint8_t val) {	
	
	set_LCD_CS(low);
	clrPin(SCK_PIN);
	set_Data_Command(is_command_data);

	if (val & 0b10000000)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);
	
	if (val & 0b01000000)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);
	
	if (val & 0b00100000)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);
	
	if (val & 0b00010000)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);
	
	if (val & 0b00001000)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);
	
	if (val & 0b00000100)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);
	
	if (val & 0b00000010)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);
	
	if (val & 0b00000001)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);

	set_LCD_CS(high);
}

uint8_t get_LCD_Val(uint8_t is_command_data, uint8_t val) {
	uint8_t maske = 0b10000000;
	uint8_t erg = 0;
	
	set_LCD_CS(low);
	set_Data_Command(is_command_data);
	
	do {
		if (val & maske)
			setPin(MOSI_PIN);
		else
			clrPin(MOSI_PIN);
		setPin(SCK_PIN);
		clrPin(SCK_PIN);
		maske >>= 1;
	} while (maske > 0);
	
	setPin(MOSI_PIN);		//ist besser
	maske = 0b10000000;
	
	do {
		setPin(SCK_PIN);
		if (getPin(MISO_PIN))
			erg |= maske;
		clrPin(SCK_PIN);
		maske >>= 1;
	} while(maske > 0);
	
	set_LCD_CS(high);
	return erg;
}

#endif


void drawWindow(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, uint16_t color) {
	uint32_t width = x1 - x + 1;
	uint32_t hight = y1 - y + 1;
	
	send_LCD_Command_Data(isCommand, CASET);	//colum address set
	send_LCD_Command_Data(isData, (x >> 8) & 0xFF);
	send_LCD_Command_Data(isData, x & 0xFF);
	send_LCD_Command_Data(isData, (x1 >> 8) & 0xFF);
	send_LCD_Command_Data(isData, x1 & 0xFF);
	send_LCD_Command_Data(isCommand, PASET);	//row address set
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
	send_LCD_Command_Data(isCommand, PASET);	//row address set
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

