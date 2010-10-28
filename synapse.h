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





/***********************************************************************
**                                                                    **
**    PLAN:                                                           **
**         	- lage alpha for synapsen, ved å legge inn [extracellular spenning] utafor neuronet, og dermed "drivkrafta" for depolarisasjonen. 
** 			-> Finn overgangen mellom [EC]-spenning og alpha. Andre mekanismer? Syn. plast.?
** 		- Lage tidsforløp for denne. 
** 			-> vidare lage delta(alpha)->delta(periode) -- transform. For å ha effektiv transient forløp av alpha.
		- 
**                                                                    **
**                                                                    **
**                                                                    **
**                                                                    **
***********************************************************************/

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

		
		//float kappa_ij;
		//float alpha_ij;

		// kalkuler [synapsens bidrag på postsyn. kappa, eller kappa_ij].
		void kalkulerKappa_ij(); 

		/*void kalkulerAlpha_ij()
		   Denne er vankeligare. Avhengig av EC spenning. Eller overføringshistorikken til synapsa. FIR-filter?
		     Avhengig av:
		 	- neuronets depol.  (?!?) Kanskje denne effekta er neglisjerbar? Nei? .. Blir komplekse ligninger av dette..
		 	- EC spenning.  	  Kanskje denne effekta er neglisjerbar? Aner ikkje..
		 	- opningane i neuronets membran. => her kommer LTP/LTD inn. Meir potensiell opning etter LTP..
			 	- 
		*/
		


	public:
		// Constructor for arv i synapse. Potensiellt farlig? Legger inn char, og sjekk om det er f.eks. 't'. ellers; feilmld.
		synapse(char c) : bInhibitorisk_effekt(false) { if(c!='t'){ cDebug<<"\n\n\n\nERROR: l 264 i nexxxxxx\n\n\n"; exit(0); } } 
		// Denne legger seg automagisk til i postsyn. si innsyn.liste, og presyn. utsyn. liste.
		synapse( neuron* pPreN_arg, neuron* pPostN_arg, bool argInhibitorisk_effekt =false, float vekt =0.1 );

		//endre fra float fSynaptiskVekt til int eller unsigned eller større: long..  For effektivitet!


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


};





    #define SYNAPSE_H
#endif
