SRCS=main.c segment14Font.c digicoFont.c paint.c tft.c
TARGET=soso

MCU=atmega328p
AVRDUDE_MCU=m328p
F_CPU=16000000
PROGR=arduino
PORT=/dev/ttyUSB1
BAUD=19200


CC=avr-gcc
OBJCOPY=avr-objcopy

CFLAGS=-g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
#nächste Zeile geht nicht mit inline-asambler-Befehlen!
CFLAGS+=-std=c99	
CFLAGS+=-Wall 
#wenns nervt, dann das nächste auskommentieren
CFLAGS+=-Wno-unused
CFLAGS+=-Wextra

all:
	$(CC) ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex	
	avrdude -c ${PROGR} -p ${AVRDUDE_MCU} -P ${PORT} -b ${BAUD} -U flash:w:${TARGET}.hex
	rm -f *.bin *.hex *.s

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
	rm -f *.bin *.hex *.s *.lst *.atxt *.o
