/* noch nicht für 4-Draht-Protokoll ausgelegt
 * d.h.: TFTs haben noch einen Pin für die Command/Data-Unterscheidungserkennung,
 * der immer vor dem CS_PIN eingestellt wird.
 */

#include "spi_soft.h"

void initSPI() {
  setOutput(SCK_PIN);
	setInput(MISO_PIN);
	setOutput(MOSI_PIN);
	setOutput(CS_PIN);
}

void setCommData(uint8_t comm) {
	if(comm)
		setPin(CD_PIN);
	else
		clrPin(CD_PIN);
}

uint8_t sendDat(uint8_t dat) {
  uint8_t maske = 0b10000000;
  uint8_t erg = 0;

  clrPin(SCK_PIN); //hier: CLK negative Logic (clrPin();)
  if (dat & maske)
    setPin(MOSI_PIN);
  else
    clrPin(MOSI_PIN);
  setPin(SCK_PIN); //hier: CLK negative Logic (setPin();)
  if (getPin(MISO_PIN))
    erg |= maske;
  maske >>= 1;

  clrPin(SCK_PIN); //hier: CLK negative Logic (clrPin();)
  if (dat & maske)
    setPin(MOSI_PIN);
  else
    clrPin(MOSI_PIN);
  setPin(SCK_PIN); //hier: CLK negative Logic (setPin();)
  if (getPin(MISO_PIN))
    erg |= maske;
  maske >>= 1;

  clrPin(SCK_PIN); //hier: CLK negative Logic (clrPin();)
  if (dat & maske)
    setPin(MOSI_PIN);
  else
    clrPin(MOSI_PIN);
  setPin(SCK_PIN); //hier: CLK negative Logic (setPin();)
  if (getPin(MISO_PIN))
    erg |= maske;
  maske >>= 1;

  clrPin(SCK_PIN); //hier: CLK negative Logic (clrPin();)
  if (dat & maske)
    setPin(MOSI_PIN);
  else
    clrPin(MOSI_PIN);
  setPin(SCK_PIN); //hier: CLK negative Logic (setPin();)
  if (getPin(MISO_PIN))
    erg |= maske;
  maske >>= 1;

  clrPin(SCK_PIN); //hier: CLK negative Logic (clrPin();)
  if (dat & maske)
    setPin(MOSI_PIN);
  else
    clrPin(MOSI_PIN);
  setPin(SCK_PIN); //hier: CLK negative Logic (setPin();)
  if (getPin(MISO_PIN))
    erg |= maske;
  maske >>= 1;

  clrPin(SCK_PIN); //hier: CLK negative Logic (clrPin();)
  if (dat & maske)
    setPin(MOSI_PIN);
  else
    clrPin(MOSI_PIN);
  setPin(SCK_PIN); //hier: CLK negative Logic (setPin();)
  if (getPin(MISO_PIN))
    erg |= maske;
  maske >>= 1;

  clrPin(SCK_PIN); //hier: CLK negative Logic (clrPin();)
  if (dat & maske)
    setPin(MOSI_PIN);
  else
    clrPin(MOSI_PIN);
  setPin(SCK_PIN); //hier: CLK negative Logic (setPin();)
  if (getPin(MISO_PIN))
    erg |= maske;
  maske >>= 1;

  clrPin(SCK_PIN); //hier: CLK negative Logic (clrPin();)
  if (dat & maske)
    setPin(MOSI_PIN);
  else
    clrPin(MOSI_PIN);
  setPin(SCK_PIN); //hier: CLK negative Logic (setPin();)
  if (getPin(MISO_PIN))
    erg |= maske;
  maske >>= 1;

  return erg;
}

uint8_t sendData8(uint8_t dat) {
  clrPin(CS_PIN);
  dat = sendDat(dat);
  setPin(CS_PIN);
  return dat;
}

uint16_t sendData16(uint16_t dat) {
  clrPin(CS_PIN);
  uint8_t msByte = dat >> 8;
  uint8_t lsByte = dat & 0xFF;
  msByte = sendDat(msByte);
  lsByte = sendDat(lsByte);
  setPin(CS_PIN);
  return (msByte << 8) | lsByte;
}

void sendDataAr(uint8_t *datReseive, uint8_t *datSend, uint8_t size_of_datSend) {
  clrPin(CS_PIN);
  for (uint8_t i = 0; i < size_of_datSend; ++i) {
    datReseive[i] = sendDat(datSend[i]);
  }
  setPin(CS_PIN);
}
