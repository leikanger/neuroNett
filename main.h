//KANN/SANN
//#include <pthread.h>

#include <iostream> 	// cout, osv
#include <fstream>  	// for file-streams..
//#include <iomanip>  	// Er denne i bruk? XXX
#include <string>   	// string
#include <ostream>  	// ?
#include <vector>   	// vector
#include <stdlib.h>   	// rand
#include <algorithm> 	// for_each
#include <list> 	// for list
#include <map>    	// map

#include <cmath> 	// for pow( , )




#include "kappa_tillegg.h"

class synapse;
class synapse_likevekt;
class neuron;
class neuroParametre;


// globale variabelDeklarasjoner:
/*
extern list<synapse*> arbeidsKoe_for_utregningAvNeuralnett_nesteSynapseP__A; 	// to køer for å holde styr på tida. Først gjøres kø A ferdig, og samtidig
extern list<synapse*> arbeidsKoe_for_utregningAvNeuralnett_nesteSynapseP__B; 	// økes kø B med alle nye element. Derretter går vi vidare;
									  	// (itererer tid, tømmer aktiv kø, bytter mellom de to køene, fortsetter med 
extern list<synapse*> *pAktivArbeidsKoe_synapseP;				//  å regne ut (tidligere side-) kø, øker (nye) sidekøa, osv.. )
extern list<synapse*> *pSideKoe_Inaktiv_synapseP;		 		// Alt dette for å ha systemtid for neurona.
*/
extern unsigned long ulTidsiterasjoner;


// globale funksjonsDeklarasjoner:
extern std::ostream & operator<< (std::ostream & ut, synapse synArg );
extern std::ostream & operator<< (std::ostream & ut, neuron neuroArg );
//extern void switchAktivArbeidsKoe_og_itererTid();


using std::ofstream;
using std::cout;
using std::endl;

using std::vector;






#ifndef MAIN_H
  #define MAIN_H
  #include "neuron.h"




#endif

// vim:fdm=marker:fmr=//{,//} : fdl=3
