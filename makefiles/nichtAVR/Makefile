SRCS=main.c print.c
TARGET=print

all:
	gcc -Wall -o ${TARGET} ${SRCS}
	rm -f *.bin *.hex *.s *.o
	./${TARGET}

file:
	gcc -Wall -c ${SRCS}

clean:
	rm -f *.bin *.hex *.s *.lst *.atxt *.o
