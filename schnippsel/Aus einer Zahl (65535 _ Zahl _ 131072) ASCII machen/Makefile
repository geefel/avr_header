SRCS=test.c
TARGET=1test

MCU=atmega328p
AVRDUDE_MCU=m328p
F_CPU=16000000
PROGR=arduino
PORT=/dev/ttyUSB0
BAUD=115200

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
#	$(CC) ${CFLAGS} -o ${TARGET}.bin ${SRCS}
#	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex

	gcc -Wall -o ${TARGET} ${SRCS}
	rm -f *.bin *.hex *.s *.o
	./${TARGET}
#	objdump -d ${TARGET}

file:
	g++ -Wall -c ${SRCS}

clean:
	rm -f *.bin *.hex *.s *.lst *.atxt *.o
