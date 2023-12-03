/*
Umstellung von Winterzeit auf Sommerzeit:
Am letzten Sonntagmorgen im März werden die Uhren von 02:00 auf 03:00
Uhr vorgestellt. Man verliert eine Stunde.

Umstellung von Sommerzeit auf Winterzeit:
Am letzten Sonntagmorgen im Oktober werden die Uhren von 03:00 auf
02:00 Uhr zurückgestellt. Man gewinnt eine Stunde.
*/

#include "rtc.h"
#include "i2c.h"

static uint8_t bcd2bin(uint8_t val);
static uint8_t bin2bcd(uint8_t val);
int dayofweek(int day, int month, int year);

uint8_t yy; 	// Year 0 - 99
uint8_t m;    // Month 1-12
uint8_t d;    // Day 1-31
uint8_t day;	// Day: 0 = Sonntag, 1 = Montag, ... 6 = Samstag
uint8_t hh;   // Hours 0-23
uint8_t mm;   // Minutes 0-59
uint8_t ss;   // Seconds 0-59

const char *dayName[] = {"SONNTAG", "MONTAG", "DIENSTAG", "MITTWOCH", "DONNERSTAG", "FREITAG", "SAMSTAG"};

uint8_t getYear() {
	return yy;
}
uint8_t getMonth() {
	return m;
}
uint8_t getDay() {
	return d;
}
void getDayName(const char *dName, uint8_t day) {
	dName = dayName[day];
}
uint8_t getHour() {
	return hh;
}
uint8_t getMinute() {
	return mm;
}
uint8_t getSecond() {
	return ss;
}
void setYear(uint8_t yy_) {
	yy = yy_;
}
void setMonth(uint8_t m_) {
	m = m_;
}
void setDay(uint8_t d_) {
	d = d_;
}
void setHour(uint8_t hh_) {
	hh = hh_;
}
void setMinute(uint8_t mm_) {
	mm = mm_;
}
void setSecond(uint8_t ss_) {
	ss = ss_;
}

//Convert a binary coded decimal value to binary. RTC stores time/date values as BCD.
static uint8_t bcd2bin(uint8_t val) {
	return val - 6 * (val >> 4);
}

//Convert a binary value to BCD format for the RTC registers
static uint8_t bin2bcd(uint8_t val) {
	return val + 6 * (val / 10);
}

/*
 *  Variation of Sillke for the Gregorian calendar.
 *  With 0=Sunday, 1=Monday, ... 6=Saturday
 */
int dayofweek(int day, int month, int year) {
	if ((month -= 2) <= 0) {
		month += 12;
		year--;
	}
	return (83 * month / 32 + day + year + year / 4 - year / 100 + year / 400) % 7;
}

/*
 * Startet Oszilator mit der Frequenz freq.
 * Freq: 0 = 1 Hz, 1 = 4096 Hz, 2 = 8192 Hz, 3 = 32768 Hz 
 */
void setSquarOn(int8_t freq) {
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 0);
	send_I2C_Data(DS1307_CONTROL);
	send_I2C_Data(SQ_ON + freq);
	stop_I2C();
}

/*
 * stoppt Oszilator, setzt SquarPin auf low
 */
void setSquarOff() {
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 0);
	send_I2C_Data(DS1307_CONTROL);
	send_I2C_Data(SQ_OFF);
	stop_I2C();
}

/*
 * setzt SquarPin auf High, startet NICHT den Oszilator
 */
void setSquarPinHigh() {
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 0);
	send_I2C_Data(DS1307_CONTROL);
	send_I2C_Data(SQ_PIN_HIGH);
	stop_I2C();
}

/*
 * zuerst setSecond(int), setMinute(int), usw. setzen
 */
void setClock() {
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 0);
	send_I2C_Data(0);
	send_I2C_Data(bin2bcd(getSecond()));
	send_I2C_Data(bin2bcd(getMinute()));
	send_I2C_Data(bin2bcd(getHour()));
	send_I2C_Data(bin2bcd(0));
	send_I2C_Data(bin2bcd(getDay()));
	send_I2C_Data(bin2bcd(getMonth()));
	send_I2C_Data(bin2bcd(getYear()));
	stop_I2C();
}

void getNow() {
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 0);
	send_I2C_Data(0);													//Registerplatz von dem gelesen werden soll angeben
	//stop_I2C();															//brauchen wir hier nicht, weil es weiter geht
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 1);
	ss = bcd2bin(read_I2C_Data_ACK() & 0x7F);	//aus Register 0x00 holen. Chip geht jetzt automatisch nach Register 0x01
	mm = bcd2bin(read_I2C_Data_ACK());				//aus Register 0x01 holen. Chip geht jetzt automatisch nach Register 0x02. usw.
	hh = bcd2bin(read_I2C_Data_ACK());
	day = bcd2bin(read_I2C_Data_ACK());
	d  = bcd2bin(read_I2C_Data_ACK());
	m  = bcd2bin(read_I2C_Data_ACK());
	yy = bcd2bin(read_I2C_Data_NACK());				//Daten holen MUSS bei letztem Byte mit NACK erfolgen 
	stop_I2C();
}

uint8_t getSecondFast() {  //die Sekunden separat holen
	uint8_t erg;
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 0);
	send_I2C_Data(0);
	//stop_I2C();
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 1);
	erg = bcd2bin(read_I2C_Data_NACK() & 0x7F);
	stop_I2C();
	return erg;	
}


