CFLAGS=-march=core2 -g -O2 -pthread -Wall 
CPPFLAGS=${CFLAGS}

neuroNett.out: main.o neuroEnhet.o main.h neuroEnhet.h neuroEnhet.cpp main.cpp
	g++ ${CFLAGS} main.o neuroEnhet.o -o neuroNett.out
c:
	make clean; make neuroNett.out

all: neuroNett.out

clean:
	rm -f *.out *.o
