#include "uart_soft.h"

void receiveDat();

uint8_t ergReceive;
static uint8_t newData;

void initUartSoft(uint16_t baud) {
	mask = 0x01;
	baud *= 1;
	INTERRUPT_ALL_DISABLE;
	INTERRUPT_PCINT_DISABLE;
	INTERRUPT_TIMER0_DISABLE;
	
	//Ports
  setOutput(TX_PIN);
  setInputPullup(RX_PIN);
  setPin(TX_PIN);  
	setOutput(LED_PIN);
  //setPin(RX_PIN);
  
  // Timer0
  TCCR0A = 0x00;    					//Init.	
  TCCR0B = 0x00;    					//Init. Vorteiler
  TCCR0A |= (1 << WGM01);			// Timer in CTC mode. Interrupt bei erreichen von TCNT0 == 0CR0A
	
	//Vergleichsregister
	//ocrVal = (int8_t)((uint32_t)F_CPU / (uint32_t)BAUD_ / getPrescaleVal((uint32_t)BAUD_)) - 1;
	OCR0A = ORCVAL;
	//PCINT von B4 Freigabe
	PCINT_PORT_SELECT;	
	//External PCINT0 interrupt
  //MCUCR = (1 << ISC01);    // Interrupt sense control: Eine fallende Flanke am INT0-Pin löst Interrupt aus 
  
  isTimerInterrupt = 0;
	isINT0_Interrupt = 0;
	clearNewData();
	
	INTERRUPT_PCINT_ENABLE;
	INTERRUPT_TIMER0_ENABLE;
  INTERRUPT_ALL_ENABLE;
}

//Es wird TIMER0 an/aus gemacht, sonst keiner (auch global)
void uart_Transmit_Soft(uint8_t dat) {
	mask = 0x01;
	uint16_t d = (dat << 1) + 512;
	TIMER0_STOP;
	TIMER0_RESET;
	OCR0A = ORCVAL - 40;
	INTERRUPT_PCINT_DISABLE;
	PCINT_RESET;
	INTERRUPT_TIMER0_ENABLE;
	INTERRUPT_ALL_ENABLE;
	isTimerInterrupt = 0;
	TIMER0_START;

	for (uint8_t i = 0; i < 10; i++) {	
		if (d & mask) 
			setPin(TX_PIN);
		else
			clrPin(TX_PIN);
		mask <<= 1;
		isTimerInterrupt = 0;
		while (!isTimerInterrupt) {asm volatile ("nop" ::);}
		OCR0A = ORCVAL;
	}
  TIMER0_STOP;
  INTERRUPT_PCINT_ENABLE;
  mask = 1;
}

uint8_t getDat() {
	return ergReceive;
}
uint8_t isNewData() {
	return newData;
}
void clearNewData() {
	newData = 0;
}


//Timer0 interrupt service routine.
ISR(TIMER0_COMPA_vect) {
	isTimerInterrupt = 1;
}

//PCINT0 interrupt service routine.
ISR(PCINT0_vect) {
	INTERRUPT_PCINT_DISABLE;
	INTERRUPT_ALL_ENABLE;
	INTERRUPT_TIMER0_ENABLE;
	isTimerInterrupt = 0;
	mask = 1;
	ergReceive = 0;
	TIMER0_STOP;
	TIMER0_RESET;
	TIMER0_START;
	
	for (uint8_t i = 0; i < 8; ++i) {
		while (!isTimerInterrupt) {asm volatile ("nop" ::);}
		if(getPin(RX_PIN))
			ergReceive |= mask;
		mask <<= 1;
		isTimerInterrupt = 0;
	}
	newData = 1;
	isINT0_Interrupt = 0;
	PCINT_RESET;		//Interrupt-Flag löschen
	INTERRUPT_PCINT_ENABLE;
}

