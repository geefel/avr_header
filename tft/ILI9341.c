#include "ILI9341.h"

void init_TFT_ILI9341() {	//für ILI9341
	set_Data_Command(isData);
	set_LCD_CS(high);
	set_LCD_BL(low);
	set_Data_Command(isCommand);
	set_LCD_BL(high);
	
	  _delay_ms(500);
    send_LCD_Command_Data(isCommand, 0x01);
    _delay_ms(200);

    send_LCD_Command_Data(isCommand, 0xCF);
    send_LCD_Command_Data(isData, 0x00);
    send_LCD_Command_Data(isData, 0x8B);
    send_LCD_Command_Data(isData, 0X30);

    send_LCD_Command_Data(isCommand, 0xED);
    send_LCD_Command_Data(isData, 0x67);
    send_LCD_Command_Data(isData, 0x03);
    send_LCD_Command_Data(isData, 0X12);
    send_LCD_Command_Data(isData, 0X81);

    send_LCD_Command_Data(isCommand, 0xE8);
    send_LCD_Command_Data(isData, 0x85);
    send_LCD_Command_Data(isData, 0x10);
    send_LCD_Command_Data(isData, 0x7A);

    send_LCD_Command_Data(isCommand, 0xCB);
    send_LCD_Command_Data(isData, 0x39);
    send_LCD_Command_Data(isData, 0x2C);
    send_LCD_Command_Data(isData, 0x00);
    send_LCD_Command_Data(isData, 0x34);
    send_LCD_Command_Data(isData, 0x02);

    send_LCD_Command_Data(isCommand, 0xF7);

    send_LCD_Command_Data(isCommand, 0xEA);
    send_LCD_Command_Data(isData, 0x00);
    send_LCD_Command_Data(isData, 0x00);

    send_LCD_Command_Data(isCommand, 0xC0);                                                      /* Power control                */
    send_LCD_Command_Data(isData, 0x1B);                                                   /* VRH[5:0]                     */

    send_LCD_Command_Data(isCommand, 0xC1);                                                      /* Power control                */
    send_LCD_Command_Data(isData, 0x10);                                                   /* SAP[2:0];BT[3:0]             */

    send_LCD_Command_Data(isCommand, 0xC5);                                                      /* VCM control                  */
    send_LCD_Command_Data(isData, 0x3F);
    send_LCD_Command_Data(isData, 0x3C);

    send_LCD_Command_Data(isCommand, 0xC7);                                                      /* VCM control2                 */
    send_LCD_Command_Data(isData, 0XB7);

    send_LCD_Command_Data(isCommand, 0x36);                                                      /* Memory Access Control        */
    send_LCD_Command_Data(isData, 0x08);

    send_LCD_Command_Data(isCommand, 0x3A);
    send_LCD_Command_Data(isData, 0x55);

    send_LCD_Command_Data(isCommand, 0xB1);
    send_LCD_Command_Data(isData, 0x00);
    send_LCD_Command_Data(isData, 0x1B);

    send_LCD_Command_Data(isCommand, 0xB6);                                                      /* Display Function Control     */
    send_LCD_Command_Data(isData, 0x0A);
    send_LCD_Command_Data(isData, 0xA2);

//    send_LCD_Command_Data(isRegister, 0xF2);                                                      /* 3Gamma Function Disable      */
//    send_LCD_Command_Data(isData, 0x00);
//
//    send_LCD_Command_Data(isRegister, 0x26);                                                      /* Gamma curve selected         */
//    send_LCD_Command_Data(isData, 0x01);
//
//    send_LCD_Command_Data(isRegister, 0xE0);                                                      /* Set Gamma                    */
//    send_LCD_Command_Data(isData, 0x0F);
//    send_LCD_Command_Data(isData, 0x2A);
//    send_LCD_Command_Data(isData, 0x28);
//    send_LCD_Command_Data(isData, 0x08);
//    send_LCD_Command_Data(isData, 0x0E);
//    send_LCD_Command_Data(isData, 0x08);
//    send_LCD_Command_Data(isData, 0x54);
//    send_LCD_Command_Data(isData, 0XA9);
//    send_LCD_Command_Data(isData, 0x43);
//    send_LCD_Command_Data(isData, 0x0A);
//    send_LCD_Command_Data(isData, 0x0F);
//    send_LCD_Command_Data(isData, 0x00);
//    send_LCD_Command_Data(isData, 0x00);
//    send_LCD_Command_Data(isData, 0x00);
//    send_LCD_Command_Data(isData, 0x00);
//
//    send_LCD_Command_Data(isRegister, 0XE1);                                                      /* Set Gamma                    */
//    send_LCD_Command_Data(isData, 0x00);
//    send_LCD_Command_Data(isData, 0x15);
//    send_LCD_Command_Data(isData, 0x17);
//    send_LCD_Command_Data(isData, 0x07);
//    send_LCD_Command_Data(isData, 0x11);
//    send_LCD_Command_Data(isData, 0x06);
//    send_LCD_Command_Data(isData, 0x2B);
//    send_LCD_Command_Data(isData, 0x56);
//    send_LCD_Command_Data(isData, 0x3C);
//    send_LCD_Command_Data(isData, 0x05);
//    send_LCD_Command_Data(isData, 0x10);
//    send_LCD_Command_Data(isData, 0x0F);
//    send_LCD_Command_Data(isData, 0x3F);
//    send_LCD_Command_Data(isData, 0x3F);
//    send_LCD_Command_Data(isData, 0x0F);

    send_LCD_Command_Data(isRegister, 0x11);                                                      /* Exit Sleep                   */
    _delay_ms(120);
    send_LCD_Command_Data(isRegister, 0x29);                                                      /* Display on                   */
		set_LCD_BL(high);
}
