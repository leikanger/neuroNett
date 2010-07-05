//KANN/SANN

//#include "main.h" XXX
#include <iostream>
#include <fstream> 	// filstreams.
#include <iomanip>
#include <stdlib.h> 	// for exit()
#include <list> 	// for list
#include <vector>

using std::list;
using std::cout;
using std::endl;





 //	i KANN trenger vi ikkje å ta hensyn til sosing med SV i synapse. Alt dette kan føres inn i K_j sin propagering til K_i : K_ij
//		K_ij = f(periode_j) * w_ij
//	i funksjonen f(-) kan "short time--plasticity" modelleres inn.



// klasse-deklarasjoner:
class synapse;
class neuron;


/* Extern-deklarasjoner */
extern unsigned long ulTidsiterasjoner;


// Denne trengs ikkje lenger (?)
//extern list<neuroSensor*> pNeuroSensorListe; // extern for at den kan deklareres her.





#ifndef SYNAPSE_H
  #define SYNAPSE_H
  
  #include "neuron.h"



/*******************************************************************************************************
 ********************    class synapse                                               ******************* synapse( neuron*, neuron*, double v =DEF_OVER..);
 ********************        - output fra neuron. ala biologiske neuron.             *******************
 ********************        - LTP, LTD (lavnivås læring) skal skje her.             *******************
 ********************                                                                *******************
 ******************************************************************************************************/
class synapse {
	protected:
	
		const bool bInhibitorisk_effekt; // true for inhibitoriske synapser.
		
		std::ofstream utskriftsFilLogg;


		float fSynaptiskVekt;


		unsigned long ulTimestampForrigeEndingAvParam; // Her kan det være ulTimestampForrigeEndingAvParamAvAlpha_ij

		//unsigned long ulTimestampForrigeEndring_eller_noke;
		
		


		/*********** For synaptic short-term depression: ***********/
		
	
		// oppdaterSyn() :
		// 	arg: 	void
		// 	retur: 	?
		// Oppdaterer alpha for synapsen. Alpha_ij er denne gange med synapsens relative innvirkning på postsynaptisk Kappa.
		// 	dvs. K_ij / K_i

		virtual int oppdaterSyn()
		{ //{ ... }
			//X Dersom den er kalt fra pNesteSynapseSomIkkjeErFerdigOppdatert_Koe (eller første element er denne..)
			//X if( this == synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front() ){
			//X	// så fjærner det første element fra oppdateringsliste:
			//X	synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.pop_front();   
			//X}		


			// Sjekker om den har oppdatert denne iterasjonen: Dette er kanskje lurt også for Kappa.
			if( ulTimestampForrigeEndingAvParam == ulTidsiterasjoner ){
	 			cout<<"\ntimestamp for oppdatering er allerede tatt (har redan oppdatert denne iter).\n";
	 			return 1; // eller kanskje noke anna. Bare ikkje 0 som returverdi.
			}
			

		
		
		


// Utskrift:
//cout<<"Repro: " <<nBestilltReproduksjonAvSvFraForrigeIter <<" synth. " <<nBestilltSynteseAvSVFraForrigeIter <<"\t";
			
	



		
			// Oppdaterer timestamp for oppdatering av syn.
			ulTimestampForrigeEndingAvParam = ulTidsiterasjoner;
			

			// For S.V. - logg: 	XXX lag en logg for Kappa, alpha, osv...
			//utskriftsFilLogg<<"\t" <<ulTidsiterasjoner <<"\t" <<ulAntallSynapticVesiclesAtt <<"\t1000\t" <<"\t" 	osv....
			//utskriftsFilLogg.flush();
	
			
			return 1;
		} //}
			

	public:
		// Constructor for arv i synapse. Potensiellt farlig? Legger inn char, og sjekk om det er f.eks. 't'. ellers; feilmld.
		synapse(char c) : bInhibitorisk_effekt(false) { if(c!='t'){ cDebug<<"\n\n\n\nERROR: l 264 i nexxxxxx\n\n\n"; exit(0); } } 
		//XXX endre fra float fSynaptiskVekt til int eller unsigned eller større: long..  For effektivitet!
		synapse( neuron* pPreN_arg, neuron* pPostN_arg, bool argInhibitorisk_effekt =false, float vekt =1 );
		// Denne legger seg automagisk til i postsyn. si innsyn.liste, og presyn. utsyn. liste.

		// Destructor, for å fjærne seg sjølv fra postsyn.inn-syn-liste, og presyn:utsyn-liste.
		~synapse();

		neuron* pPreNode;
		neuron* pPostNode;

		//static list<synapse*>  pNesteSynapseSomIkkjeErFerdigOppdatert_Koe;	
		
		


	
	
		// trur ikkje eg trenger returverdi:
		virtual void aktiviserOgRegnUt()
		//void synapse::aktiviserOgRegnUt()
		{ //{

		// KAPPA: her bør kappa_ij regnes ut og altha oppdateres, og alpha_ij beregnes fra kappa_ij/kappa_i.

cout<<"jeje";
			/******* oppdaterer timestamp for tidspkt for signal ******** UNØDVENDIG? */
			//ulTimestampForrigeSignal = ulTidsiterasjoner;
		} //}	
		
		
		friend std::ostream & operator<< (std::ostream & ut, synapse );
		friend std::ostream & operator<< (std::ostream & ut, neuron );
		friend int initArbeidskoer();
		friend class synSkilleElement;

		// For å gi postsyn. neuron mulighet til å initiere LTP/LTD: (protected funksjoner)
		friend class neuron; // Aktuell for kappa? veit ikkje..

		// Bare mens eg drive å utvikler.
		friend int main(int, char**);
};














/*******************************************************************************************************
 ********************    class synSkilleElement                                     *******************
 ********************        - Eit tidsskille for neurale nettet. skal iterere       *******************
 ********************          tid, og legge til nytt tidselement i arbeidskøa.      *******************
 ********************        - Alt dette i overloada funskjon, for å sleppe å        *******************
 ********************          endre på der vi  kaller funskjonane i køa.            *******************
 ******************************************************************************************************/
class synSkilleElement : public synapse {
	
	public:
		synSkilleElement() : synapse('t') { cout<<"\tinne i konstruktor\n"; }


		// arbeidsliste - kø
		static list<synapse*>  pNesteSynapseUtregningsKoe;


		// gjør heilt andre ting enn synapse. Heiter det samme pga overloading.
		void aktiviserOgRegnUt(); // Utleda i neuroEnhet.cpp


		// Legger seg sjølv til sist i lista, og returnerer 0 (symboliserer at liste er ferdig (det før skilleelementet) )
/*
		int oppdaterSyn(){ 
			// kan være lurt å for sikkerhetsskuld ta med if(pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front() == this )  - Men treigare..
			if( synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front() != this ){ cout<<"FEIL synapse.h_52"; exit(-1); }
			/ * IKKJE LØYSING: feilsjekk..  * /

			// legg seg til på slutt:
			synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.push_back(this);   	
			// Fjærner dette element fra oppdateringsliste:
			synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.pop_front();   

			return 0; // Signaller tilbake att dette ikkje er vanlig synapse::oppdaterSyn() (viktig for funksjonallitet til programmet)
		}
*/	       	/* return 0 er heile poenget med int-returverdi her. Skal ret. 0 ved synSkilleElement, og 1 ved vanlig ok
		 * 	Dersom det er vanlig oppdatering av vanlig synapse, vil den returnere 1, her returnerer den 0, Dette gjør at eg kan kjøre
		 * 	while( ! (kø++)->oppdaterSyn() );  // trur ikkje noke skal gjøres. Kanskje utskrift eller noke..
		 */

};





    #define SYNAPSE_H
#endif
