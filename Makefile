CFLAGS=-march=core2 -g -O2 -pthread -Wall 
CPPFLAGS=${CFLAGS}

neuroNett.out: main.o neuroEnhet.o
	g++ ${CFLAGS} main.o neuroEnhet.o -o neuroNett.out

main.o: main.cpp main.h
	g++ ${CFLAGS} -c main.cpp 

neuroEnhet.o: neuroEnhet.cpp neuroEnhet.h neuroSensor.h synapse.h
	g++ ${CFLAGS} -c neuroEnhet.cpp 
c:
	make clean; echo "\n"; make

all: neuroNett.out

clean:
	rm -f *.out *.o

# *storbox og *litabox (med stjerne på slutten) lager dette, etter mi .vimrc . (med stjerne på slutten og.
