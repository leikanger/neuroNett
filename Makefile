CFLAGS=-march=core2 -g -O2 -Wall #-pthread
CPPFLAGS=${CFLAGS}

all: neuroNett.out

neuroNett.out: main.o neuroEnhet.o
	g++ ${CFLAGS} main.o neuroEnhet.o -o neuroNett.out

main.o: main.cpp main.h
	g++ ${CFLAGS} -c main.cpp 

neuroEnhet.o: neuroEnhet.cpp neuron.h neuroSensor.h synapse.h
	g++ ${CFLAGS} -c neuroEnhet.cpp 
c:
	make clean; echo "\n"; make


clean:
	rm -f *.out *.o

# *storbox og *litabox (med stjerne på slutten) lager dette, etter mi .vimrc . (med stjerne på slutten og.
