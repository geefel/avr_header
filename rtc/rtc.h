/*
 * CONTROL REGISTER
 * The DS1307/DS1308 Control Register is used to control the operation of the SQW/OUT pin.
 * 	BIT 7	BIT 6	BIT 5 BIT 4	BIT 3	BIT 2	BIT 1	BIT 0
 * 	OUT		x			x			SQWE	x			x			RS1		RS0
 * 
 * OUT (Output control): This bit controls the output level of the SQW/OUT pin when the square wave
 * 	output is DISABLED. If SQWE=0, the logic level on the SQW/OUT pin is 1 if OUT=1 and is 0 if OUT=0.
 * SQWE (Square Wave Enable): This bit, when set to a logic 1, will enable the oscillator output. The
 * 	frequency of the square wave output depends upon the value of the RS0 and RS1 bits.
 * RS (Rate Select): These bits control the frequency of the square wave output when the square wave
 * 	output has been enabled. Table 1 lists the square wave frequencies that can be selected with the RS bits.
 * 
 * 	RS1	RS0	SQW OUTPUTFREQUENCY
 * 	0		0		1 Hz
 * 	0		1		4.096 kHz
 * 	1		0		8.192 kHz
 * 	1		1		32.768 kHz
 */
 
/*Umstellung von Winterzeit auf Sommerzeit:
Am letzten Sonntagmorgen im März werden die Uhren von 02:00 auf 03:00
Uhr vorgestellt. Man verliert eine Stunde.

Umstellung von Sommerzeit auf Winterzeit:
Am letzten Sonntagmorgen im Oktober werden die Uhren von 03:00 auf
02:00 Uhr zurückgestellt. Man gewinnt eine Stunde.
*/

/*
 * DS1307 muß zuerst mit setClock() angemacht werden (Zeit ist egal), sonst läuft er nicht.
 * SQW/OUT, SCL, SDA brauchen Pullup!
 
#ifndef RTC_H
#define RTC_H

#include <stdint.h>

#define DS1307_ADDRESS 0x68 // I2C address for DS1307
#define DS1307_CONTROL 0x07 // Control register
#define DS1307_NVRAM 	 0x08 // Start of RAM registers - 56 bytes, 0x08 to 0x3f

#define SQ_ON  			0b00010000	 //siehe Erklärung oben
#define SQ_OFF 			0b00000000
#define SQ_PIN_HIGH 0b10000000

uint8_t getYear();
uint8_t getMonth();
uint8_t getDay();
uint8_t getHour();
uint8_t getMinute();
uint8_t getSecond();
void setYear(uint8_t yy);
void setMonth(uint8_t m);
void setDay(uint8_t d);
void setHour(uint8_t hh);
void setMinute(uint8_t mm);
void setSecond(uint8_t ss);

void setSquarOn(int8_t freq);
void setSquarOff();
void setSquarPinHigh();

/*
 * zuerst setSecond(int), setMinute(int), usw. setzen
 */
void setClock();
void getNow();
uint8_t getSecondFast();

#endif //RTC_H
