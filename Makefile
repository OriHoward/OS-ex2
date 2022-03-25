CC = gcc
AR = ar

all: sig

sig: Signals.o
	${CC} ${CFLAGS} -o sig Signals.o

Signals.o: Signals.c
	${CC} ${CFLAGS} -c Signals.c


clean:
	rm -f *.a *.o sig