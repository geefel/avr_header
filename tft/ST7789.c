#include "ST7789.h"

void init_TFT_ST7789() {	//Für ST7789
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
