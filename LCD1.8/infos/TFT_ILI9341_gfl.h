/*
 * SCLK 	SPI clock
 * MISO 	SPI Master In Slave Out
 * MOSI 	SPI Master Out Slave In
 * LCD_CS 	LCD chip select signal
 * LCD_BL 	LCD back light control signal
 * LCD_DC 	LCD digital command signal
 * SD_CS 	SD chip select signal
 * TP_CS 	XPT2046 chip select signal
 * TP_IRQ 	XPT2046 interrupt output signal
 * 5V 	5V power supply
 * GND 	Ground
 * 
 * Es gibt Chipselect-Pin, einen Reset-Pin und einen Pin für (LOW) Comand und (HIGH) Data
 * //These define the ports and port bits used for the chip select (CS) and data/command (DC) lines
 * #define TFT_CS   10  // Chip select control pin
 * #define TFT_DC   7  // Data/Command control pin
 * #define TFT_RST  0 //Reset pin (could connect to Arduino RESET pin)
 * #define MOSI
 * #define MISO
 * #define TFT_CS_L		FastPin<TFT_CS>::lo()
 * #define TFT_CS_H		FastPin<TFT_CS>::hi()
 * #define TFT_DC_C 	FastPin<TFT_DC>::lo()	//Comand
 * #define TFT_DC_D 	FastPin<TFT_DC>::hi()	//Data
 * 
 * _____ILI9341 vs. ST7789VW_______
 * offenbar nur kleinste unterschiede: Rotation und RGB/BGR
 * Rotation ist in MADCTL[4...7] 0x0b
 * RGB/BGR ist in MADCTL[3]			 0x0b
 * Commands sind:
 * 0x2A xStartPos 2 Bytes xEndPos 2 Bytes MSB first (für Punkt reicht xStartPos)
 * 0x2B	yStartPos 2 Bytes yEndPos 2 Bytes MSB first (für Punkt reicht yStartPos)
 * 0x2C Speicher für Farbe 2 Bytes
 */

#ifndef TFT_ILI9341_GFL_H
#define TFT_ILI9341_GFL_H

//init-Sequenz
void TFT::lcd_init()
{
#define __LCD_CS_OUT()    DDRB |= 0x04
#define __LCD_CS_CLR()    PORTB &=~ 0x04
#define __LCD_CS_SET()    PORTB |=  0x04

#define __LCD_DC_OUT()    DDRD |= 0x80
#define __LCD_DC_CLR()    PORTD &=~ 0x80
#define __LCD_DC_SET()    PORTD |=  0x80

#define __LCD_BKL_OUT()   DDRB |= 0x02
#define __LCD_BKL_OFF()   PORTB &=~ 0x02
#define __LCD_BKL_ON()    PORTB |=  0x02

	__LCD_DC_OUT();	//LCD_DC als out	Data/Command-Pin
	__LCD_DC_SET();	//LCD_DC HIGH
	
	__LCD_CS_OUT();	//LCD_CS als out	Chipselect
  __LCD_CS_SET();	//LCD_CS HIGH

	__LCD_BKL_OUT(); //LCD_BKL als out Backlight
  __LCD_BKL_OFF(); //LCD_BKL LOW
	
	__LCD_DC_CLR();	 //LCD_DC auf Command
	__LCD_CS_CLR();	 //Chipselect
	SPI.transfer(0xDC);	//ID3 abholen
	lcd_id = SPI.transfer(0x00);	//0x00 ist NOP?
	__LCD_CS_SET();	 //Chipselect aus
	
	if(ST7789 == lcd_id){
		lcd_write_byte(0x11,LCD_CMD);	//Komando 0x11 senden
/*
 * Register 0x36 mit 0 beschreiben, d.h.:
 * - LCD_DC LOW (Command) 
 * - CS LOW
 * - Register senden
 * - CS HIGH
 * - LCD_DC HIGH (Data)
 * - CS LOW
 * - 0x00 senden
 * - CS HIGH
 */
 //void lcd_write_register(uint8_t chRegister, uint8_t chValue)
		lcd_write_register(0x36,0x00);
		lcd_write_register(0x3a,0x55);
/*
 * Command oder Data senden, d.h.:
 * - LCD_DC LOW (oder HIGH) 
 * - CS LOW
 * - Command senden (oder Data)
 * - CS HIGH
 */
		lcd_write_byte(0xb2,LCD_CMD);
		lcd_write_byte(0x0c,LCD_DATA);
		lcd_write_byte(0x0c,LCD_DATA);
		lcd_write_byte(0x00,LCD_DATA);
		lcd_write_byte(0x33,LCD_DATA);
		lcd_write_byte(0x33,LCD_DATA);
		lcd_write_register(0xb7,0x35);
		lcd_write_register(0xbb,0x28);
		lcd_write_register(0xc0,0x3c);
		lcd_write_register(0xc2,0x01);
		lcd_write_register(0xc3,0x0b);
		lcd_write_register(0xc4,0x20);
		lcd_write_register(0xc6,0x0f);
		lcd_write_byte(0xD0,LCD_CMD);
		lcd_write_byte(0xa4,LCD_DATA);
		lcd_write_byte(0xa1,LCD_DATA);
		lcd_write_byte(0xe0,LCD_CMD);
		lcd_write_byte(0xd0,LCD_DATA);
		lcd_write_byte(0x01,LCD_DATA);
		lcd_write_byte(0x08,LCD_DATA);
		lcd_write_byte(0x0f,LCD_DATA);
		lcd_write_byte(0x11,LCD_DATA);
		lcd_write_byte(0x2a,LCD_DATA);
		lcd_write_byte(0x36,LCD_DATA);
		lcd_write_byte(0x55,LCD_DATA);
		lcd_write_byte(0x44,LCD_DATA);
		lcd_write_byte(0x3a,LCD_DATA);
		lcd_write_byte(0x0b,LCD_DATA);
		lcd_write_byte(0x06,LCD_DATA);
		lcd_write_byte(0x11,LCD_DATA);
		lcd_write_byte(0x20,LCD_DATA);
		lcd_write_byte(0xe1,LCD_CMD);
		lcd_write_byte(0xd0,LCD_DATA);
		lcd_write_byte(0x02,LCD_DATA);
		lcd_write_byte(0x07,LCD_DATA);
		lcd_write_byte(0x0a,LCD_DATA);
		lcd_write_byte(0x0b,LCD_DATA);
		lcd_write_byte(0x18,LCD_DATA);
		lcd_write_byte(0x34,LCD_DATA);
		lcd_write_byte(0x43,LCD_DATA);
		lcd_write_byte(0x4a,LCD_DATA);
		lcd_write_byte(0x2b,LCD_DATA);
		lcd_write_byte(0x1b,LCD_DATA);
		lcd_write_byte(0x1c,LCD_DATA);
		lcd_write_byte(0x22,LCD_DATA);
		lcd_write_byte(0x1f,LCD_DATA);
		lcd_write_register(0x55,0xB0);
		lcd_write_byte(0x29,LCD_CMD);
		lcd_clear_screen(WHITE);
		__LCD_BKL_ON();
	}else{
		//Driving ability Setting
		lcd_write_register(0xEA,0x00); //PTBA[15:8]
		lcd_write_register(0xEB,0x20); //PTBA[7:0]
		lcd_write_register(0xEC,0x0C); //STBA[15:8]
		lcd_write_register(0xED,0xC4); //STBA[7:0]
		lcd_write_register(0xE8,0x38); //OPON[7:0]
		lcd_write_register(0xE9,0x10); //OPON1[7:0]
		lcd_write_register(0xF1,0x01); //OTPS1B
		lcd_write_register(0xF2,0x10); //GEN
		//Gamma 2.2 Setting
		lcd_write_register(0x40,0x01); //
		lcd_write_register(0x41,0x00); //
		lcd_write_register(0x42,0x00); //
		lcd_write_register(0x43,0x10); //
		lcd_write_register(0x44,0x0E); //
		lcd_write_register(0x45,0x24); //
		lcd_write_register(0x46,0x04); //
		lcd_write_register(0x47,0x50); //
		lcd_write_register(0x48,0x02); //
		lcd_write_register(0x49,0x13); //
		lcd_write_register(0x4A,0x19); //
		lcd_write_register(0x4B,0x19); //
		lcd_write_register(0x4C,0x16); //
		lcd_write_register(0x50,0x1B); //
		lcd_write_register(0x51,0x31); //
		lcd_write_register(0x52,0x2F); //
		lcd_write_register(0x53,0x3F); //
		lcd_write_register(0x54,0x3F); //
		lcd_write_register(0x55,0x3E); //
		lcd_write_register(0x56,0x2F); //
		lcd_write_register(0x57,0x7B); //
		lcd_write_register(0x58,0x09); //
		lcd_write_register(0x59,0x06); //
		lcd_write_register(0x5A,0x06); //
		lcd_write_register(0x5B,0x0C); //
		lcd_write_register(0x5C,0x1D); //
		lcd_write_register(0x5D,0xCC); //
		//Power Voltage Setting
		lcd_write_register(0x1B,0x1B); //VRH=4.65V
		lcd_write_register(0x1A,0x01); //BT (VGH~15V,VGL~-10V,DDVDH~5V)
		lcd_write_register(0x24,0x2F); //VMH(VCOM High voltage ~3.2V)
		lcd_write_register(0x25,0x57); //VML(VCOM Low voltage -1.2V)
		//****VCOM offset**///
		lcd_write_register(0x23,0x88); //for Flicker adjust //can reload from OTP
		//Power on Setting
		lcd_write_register(0x18,0x34); //I/P_RADJ,N/P_RADJ, Normal mode 60Hz
		lcd_write_register(0x19,0x01); //OSC_EN='1', start Osc
		lcd_write_register(0x01,0x00); //DP_STB='0', out deep sleep
		lcd_write_register(0x1F,0x88);// GAS=1, VOMG=00, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=0
		delay(5);
		lcd_write_register(0x1F,0x80);// GAS=1, VOMG=00, PON=0, DK=0, XDK=0, DVDH_TRI=0, STB=0
		delay(5);
		lcd_write_register(0x1F,0x90);// GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0
		delay(5);
		lcd_write_register(0x1F,0xD0);// GAS=1, VOMG=10, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
		delay(5);
		//262k/65k color selection
		lcd_write_register(0x17,0x05); //default 0x06 262k color // 0x05 65k color
		//SET PANEL
		lcd_write_register(0x36,0x00); //SS_P, GS_P,REV_P,BGR_P
		//Display ON Setting
		lcd_write_register(0x28,0x38); //GON=1, DTE=1, D=1000
		delay(40);
		lcd_write_register(0x28,0x3F); //GON=1, DTE=1, D=1100

		lcd_write_register(0x16,0x18); 
		//Set GRAM Area
		lcd_write_register(0x02,0x00);
		lcd_write_register(0x03,0x00); //Column Start
		lcd_write_register(0x04,0x00);
		lcd_write_register(0x05,0xEF); //Column End
		lcd_write_register(0x06,0x00);
		lcd_write_register(0x07,0x00); //Row Start
		lcd_write_register(0x08,0x01);
		lcd_write_register(0x09,0x3F); //Row End
		lcd_clear_screen(WHITE);
		__LCD_BKL_ON();
	}
}


#endif	//TFT_ILI9341_GFL_H