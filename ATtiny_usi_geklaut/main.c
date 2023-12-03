// Serial Transmission Example for the ATtiny85
// by Ido Gendel, 2015
// https://www.idogendel.com/en/archives/406
// Share and enjoy!

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile struct {
  uint8_t dataByte, bitsLeft, pin, done;    
} txData = {0, 0, 0, 0};

//---------------------------------------------------------
void sendBySerial(const uint8_t data) {
  txData.dataByte = data;
  txData.bitsLeft = 10;
  txData.done = 0;
  // Reset counter
  TCNT0 = 0;
  // Activate timer0 A match interrupt
  TIMSK = 1 << OCIE0A;
}

//---------------------------------------------------------
// Blocking 
void sendStrBySerial(char *p) {
  while (*p != '\0') {
    sendBySerial(*p++);
    while (!txData.done)
			;
  }
}

//---------------------------------------------------------
void initSerial(const uint8_t portPin) {
  txData.pin = portPin;
  // Define pin as output
  DDRB = 1 << txData.pin;
  // Set it to the default HIGH
  PORTB |= 1 << txData.pin;
  // Set top value for counter 0
  OCR0A = 51;
  // No A/B match output; just CTC mode
  TCCR0A = 1 << WGM01;
  // Set prescaler to clk/8
  TCCR0B = (1 << CS01);
}

//---------------------------------------------------------
int main(void) {
  char myStr[] = "~Hello Tiny Serial!";
  initSerial(2);
  // Enable interrupts
  sei();
  sendStrBySerial("~");
  while(1) {
    _delay_ms(5000);
    sendStrBySerial(myStr);  
    sendBySerial(10); // new line
  }
}
//---------------------------------------------------------
// Timer 0 A-match interrupt
ISR(TIMER0_COMPA_vect) {
  uint8_t bitVal;
  switch (txData.bitsLeft) {
    case 10: bitVal = 0; break;
    case  1: bitVal = 1; break;
    case  0: TIMSK &= ~(1 << OCIE0A); 
						 txData.done = 1; 
						 return;
    default: bitVal = txData.dataByte & 1;
						 txData.dataByte >>= 1;
       
  }
  if (bitVal) 
		PORTB |= (1 << txData.pin); 
  else 
		PORTB &= ~(1 << txData.pin);
  --txData.bitsLeft;
}
