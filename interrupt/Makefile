#Programmer: Arduino nano 
#Pins (arduino/ATtiny): Pin10/Reset, Pin11/PB0, Pin12/PB1, Pin13/PB2, GND/GND, 5V/VCC

SRCS=main.c int0.c print.c uart_attiny.c
TARGET=interrupt

#========Board/µC===============
#ATmega48A/PA/88A/PA/168A/PA/328/P
#MCU=atmega328p
#AVRDUDE_MCU=m328p
#F_CPU=16000000
#BAUD=115200
#ende ATmega

#ATtiny25/45/85
MCU=attiny45
AVRDUDE_MCU=ATtiny45
F_CPU=1000000
BAUD=9600
#ende ATtiny

#pro mini
#MCU=atmega328p
#AVRDUDE_MCU=m328p
#F_CPU=16000000
#BAUD=57600
#ende pro mini

#=========Programmer=============
#Arduinoboard
#PROGR=arduino
#PORT=/dev/ttyACM0
#Ende Arduinoboard

#Arduino as ISP
#PROGR=arduino
#PORT=/dev/ttyACM0
#Ende Arduino as ISP

#FT232 Break Out
PROGR=arduino
PORT=/dev/ttyUSB0
#Ende FT232 Break Out

#================================

CC=avr-gcc
OBJCOPY=avr-objcopy

CFLAGS=-g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
#nächste Zeile geht nicht mit asambler-Befehlen!
#CFLAGS+=-std=C99
CFLAGS+=-Wall
#wenns nervt, dann das nächste auskommentieren
#CFLAGS+=-Wno-unused
CFLAGS+=-Wextra

#fuse!!!!
#LFUSE=0x62 ist default, LFUSE=0xe2 ist mit Vorteiler = 0 
LFUSE=0x62
HFUSE=
EFUSE=

all:
	$(CC) ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex	
	avrdude -c ${PROGR} -p ${AVRDUDE_MCU} -P ${PORT} -b ${BAUD} -U flash:w:${TARGET}.hex
	rm -f *.bin *.hex *.o

file:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex
	avr-size --format=avr --mcu=${MCU} ${TARGET}.bin
	
asm:
	${CC} ${CFLAGS} -fno-asynchronous-unwind-tables -fno-exceptions -fno-rtti -fverbose-asm \
	tft.c > tft.s -S -o- | less
 
flash:
	avrdude -c ${PROGR} -p ${AVRDUDE_MCU} -P ${PORT} -b ${BAUD} -U flash:w:${TARGET}.hex

fuselesen:
	avrdude -c ${PROGR} -p ${AVRDUDE_MCU} -P ${PORT} -b ${BAUD} -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h
	
lfuseschreiben:
	avrdude -c ${PROGR} -p ${AVRDUDE_MCU} -P ${PORT} -b ${BAUD} -U lfuse:w:${LFUSE}:m
	
hfuseschreiben:
	avrdude -c ${PROGR} -p ${AVRDUDE_MCU} -P ${PORT} -b ${BAUD} -U hfuse:w:${HFUSE}:m

efuseschreiben:
	avrdude -c ${PROGR} -p ${AVRDUDE_MCU} -P ${PORT} -b ${BAUD} -U efuse:w:${EFUSE}:m
	
exe:
	gcc -Wall -o ${TARGET} ${SRCS}
	rm -f *.bin *.hex *.s *.o

clean:
	rm -f *.bin *.hex *.s *.lst *.atxt *.o
