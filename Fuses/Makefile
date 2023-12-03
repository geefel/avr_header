#Programmer: Arduino nano 
#Pins (arduino/ATtiny): Pin10/Reset, Pin11/PB0, Pin12/PB1, Pin13/PB2, GND/GND, 5V/VCC

SRCS=main.c
TARGET=fuseBits

MCU=attiny45
AVRDUDE_MCU=ATtiny45
#F_CPU=8000000
PROGR=arduino
PORT=/dev/ttyUSB0
BAUD=9600

CC=avr-gcc
OBJCOPY=avr-objcopy

CFLAGS=-g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
#nächste Zeile geht nicht mit asambler-Befehlen!
#CFLAGS+=-std=C99
CFLAGS+=-Wall
#wenns nervt, dann das nächste auskommentieren
#CFLAGS+=-Wno-unused
CFLAGS+=-Wextra
AVRFUSE+=-U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h

all:
	$(CC) ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex	
	avrdude -c ${PROGR} -p ${AVRDUDE_MCU} -P ${PORT} -b ${BAUD} ${AVRFUSE} -U flash:r:${TARGET}.hex
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
	avrdude -c ${PROGR} -p ${AVRDUDE_MCU} -P ${PORT} -n -v 
	#-U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h
	
exe:
	gcc -Wall -o ${TARGET} ${SRCS}
	rm -f *.bin *.hex *.s *.o

clean:
	rm -f *.bin *.hex *.s *.lst *.atxt *.o
