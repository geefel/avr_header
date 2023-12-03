#ifndef ZEITMESSEN_H
#define ZEITMESSEN_H


#define atmega328p
//#define attiny45

void setupTimer();
void startTimer();
void stopTimer();
uint16_t getTime();
void resetTime();

#endif  //ZEITMESSEN_H
