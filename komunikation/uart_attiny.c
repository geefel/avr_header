#include "uart_attiny.h"

volatile uint8_t dataReceived;
static volatile uint8_t newData;
static volatile uint16_t maskeSend;
static volatile uint16_t maskeReceive;
volatile uint8_t isPCINT_Interrupt;
volatile uint8_t isTimer_Interrupt;
static uint16_t orcval;
uint16_t setOrcval();
static inline void setupTimer();
static inline void startTimer();
static inline void stopTimer();
static inline void clearTimer();
static inline void setOCRTimer();
static inline void setOCRshortTimer();
static inline void setupPCINT0();
static inline void enablePCINT0();
static inline void disablePCINT0();
static inline void resetPCINT0();

uint16_t setOrcval() {
  orcval = F_CPU / BAUD_ / PRESCALE;
  return orcval;
}

void setupTimer() { 
#if defined (TIMER_1)
	TCCR1 = 0;                //Timer/Counter Comparator A disconnected from output pin OC1A
	TIMSK |= 1 << OCIE1A;			//Interruptfreigabe
  OCR1A = setOrcval();			//uart_attiny.h
#elif defined (TIMER_0)
  TCCR0A |= (1 << WGM01);		// Timer in CTC mode. Interrupt bei erreichen von TCNT0 == OCR0A
  TIMSK |= 1 << OCIE0A;
  OCR0A = setOrcval();
#endif
}

static inline void setOCRTimer() {
#if defined (TIMER_1)
	OCR1A = orcval;
#elif defined (TIMER_0)
  OCR0A = orcval;
#endif
}

static inline void setOCRshortTimer() {
#if defined (TIMER_1)
	OCR1A = orcval / 2;
#elif defined (TIMER_0)
  OCR0A = orcval / 2;
#endif
}

static inline void clearTimer() {
#if defined (TIMER_1)
	TCNT1 = 0;
#elif defined (TIMER_0)
  TCNT0 = 0;
#endif
}

static inline void stopTimer() {
#if defined (TIMER_1)
	TCCR1 = 0;
#elif defined (TIMER_0)
  TCCR0B = 0;
#endif
}

static inline void startTimer() {
#if defined (TIMER_1)
	TCCR1 = PRESCALE_T1;
#elif defined (TIMER_0)
 	TCCR0B = PRESCALE_T0;
#endif
}

static inline void setupPCINT0() {
  PCMSK = 1 << PCINTn;
}

static inline void enablePCINT0() {
  GIMSK |= (1 << PCIE);
}

static inline void disablePCINT0() {
  GIMSK &= ~(1 << PCIE);
}

static inline void resetPCINT0() {
  GIFR |= 1<< PCIF;
}

void initUartSoftATtiny() {
	maskeReceive = 1;
	maskeSend = 1;
	setupTimer();
  setOutput(TX_PIN);
  setInputPullup(RX_PIN);
  setPin(TX_PIN);
	setupPCINT0();
	clearNewData();
	enablePCINT0();
}

//Es wird TIMER an/aus gemacht, sonst keiner (auch global)
void sendDat(uint8_t dat) {
	uint16_t datum = (dat << 1) + 512;
	stopTimer();
	clearTimer();
	startTimer();
	for (uint8_t i = 0; i < 10; i++) {	
		if (datum & maskeSend) 
			setPin(TX_PIN);
		else
			clrPin(TX_PIN);
		maskeSend <<= 1;
		isTimer_Interrupt = 0;
		while (!isTimer_Interrupt) {asm volatile ("nop" ::);}
		clearTimer();		// !!!!!!!
	}
  stopTimer();
  maskeSend = 1;
}

uint8_t isNewData() {
	return newData;
}

void clearNewData() {
	newData = 0;
	dataReceived = 0;
}

uint8_t getNewData() {
	return dataReceived;
}

//Timer1 interrupt service routine.
#if defined (TIMER_1)
ISR(TIMER1_COMPA_vect) {
#elif defined (TIMER_0)
ISR(TIMER0_COMPA_vect) {
#endif
	isTimer_Interrupt = 1;
}

//PCINT0 interrupt service routine für reseiveData
ISR(PCINT0_vect) {
	disablePCINT0();
	clearTimer();
	startTimer();
	
	for (uint8_t i = 0; i < 9; ++i) {
		isTimer_Interrupt = 0;
		while (!isTimer_Interrupt) {asm volatile ("nop" ::);}
		if(getPin(RX_PIN))
			dataReceived |= maskeReceive;
		maskeReceive <<= 1;
	}
	
	newData = 1;
	isPCINT_Interrupt = 0;
	maskeReceive = 1;
	resetPCINT0();		//Interrupt-Flag löschen
	enablePCINT0();
}
