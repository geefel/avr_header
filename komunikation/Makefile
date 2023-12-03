SRCS=main.c uart_attiny.c print.c
TARGET=test

#MCU=atmega328p
#AVRDUDE_MCU=m328p
#F_CPU=16000000
#PROGR=arduino uart_attiny.c
#PORT=/dev/ttyUSB0
#BAUD=115200

#ATtiny25/45/85
MCU=attiny45
AVRDUDE_MCU=ATtiny45
F_CPU=1000000
BAUD=9600
#ende ATtiny
#=========Programmer=============
#Arduinoboard
PROGR=arduino
PORT=/dev/ttyUSB0
#Ende Arduinoboard

CC=avr-gcc
OBJCOPY=avr-objcopy

CFLAGS=-g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
#nächste Zeile geht nicht mit asambler-Befehlen!
#CFLAGS+=-std=C99
CFLAGS+=-Wall
#wenns nervt, dann das nächste auskommentieren
#CFLAGS+=-Wno-unused
CFLAGS+=-Wextra



all:
	$(CC) ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex	
	avrdude -c ${PROGR} -p ${AVRDUDE_MCU} -P ${PORT} -b ${BAUD} -U flash:w:${TARGET}.hex
	rm -f *.bin *.hex

file:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
#${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex
	avr-size --format=avr --mcu=${MCU} ${TARGET}.bin
	
asm:
	${CC} ${CFLAGS} -fno-asynchronous-unwind-tables -fno-exceptions -fno-rtti -fverbose-asm \
	tft.c > tft.s -S -o- | less
 
flash:
	avrdude -c ${PROGR} -p ${AVRDUDE_MCU} -P ${PORT} -b ${BAUD} -U flash:w:${TARGET}.hex

clean:
	rm -f *.bin *.hex *.s *.lst *.atxt
