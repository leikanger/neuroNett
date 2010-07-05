CFLAGS=-march=core2 -g -O2 -Wall #-pthread
CPPFLAGS=${CFLAGS}

#all:	neuroNett.out

neuroNett.out: main.o neuroEnhet.o 
	echo "\n\n"; g++ ${CFLAGS} main.o neuroEnhet.o -o neuroNett.out; echo "\n\n"

main.o: main.cpp main.h kappa_tillegg.h
	g++ ${CFLAGS} -c main.cpp 

neuroEnhet.o: neuroEnhet.cpp neuron.h neuroSensor.h synapse.h kappa_tillegg.h
	g++ ${CFLAGS} -c neuroEnhet.cpp 
c:
	 clear; make clean; echo "\n\n"; make; echo "\n\n"


clean:
	rm -f *.out *.o

# *storbox og *litabox (med stjerne på slutten) lager dette, etter mi .vimrc . (med stjerne på slutten og.
