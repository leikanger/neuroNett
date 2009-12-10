
#include <iostream>
#include <iomanip>
#include <stdlib.h> 	// for exit()
#include <list> 	// for list
#include <vector>

using std::list;
using std::cout;
using std::endl;

// Seier fra om at kode er kjørt:
#ifndef SYNAPSE
    #define SYNAPSE
#endif



#define cDebug std::cerr

#define DEF_OPPLADINGSFART_FOR_SYN_VESICLES 	5
#define DEF_OVERFOERINGSFAKTOR_FOR_SYN   	1
#define DEF_startANTALL_SYN_V 		    	100

#define LTP_HASTIGHET_endringAvAntall_POSTSYN_RECEPTOR 2 // Ved LTP...

// klasse-deklarasjoner:
class synapse;
class neuron;
class neuroSensor;
class arbeidsHistorieElement;



/* Extern-deklarasjoner */
extern unsigned long ulTidsiterasjoner;

extern list<synapse*> 		    pNesteSynapseUtregningsKoe;
extern list<arbeidsHistorieElement*> pArbeidsHistorieListe;

extern list<neuroSensor*> pNeuroSensorListe; // extern for at den kan deklareres her.






/**************************************************************************************************
***** TODO gjør slik at ladingsfarta for synaptic vesicles er bra. No går han rett opp over *******
***** 		til 113 (%). Uten provosering. Skal gå til 100. I tilfelle augmentation,    *******
***** 		skal den gå over. XXX 							    *******
**************************************************************************************************/






/*******************************************************************************************************
 ********************    class synapse                                               *******************
 ********************        - output fra neuron. ala biologiske neuron.             *******************
 ********************        - LTP, LTD (lavnivås læring) skal skje her.             *******************
 ********************                                                                *******************
 ******************************************************************************************************/
class synapse {
	private:
		// overvåkinga skjer annen plass. Her skjer bare initiering (og kanskje vedlikehold).
		void LTP(){
			// XXX Finn ut korleis dette funker. Når du finner ut dette, publiser artikkel, og skriv om denne funskjonen.
			
			// Finn ut om vektendring er omvendt proposjonalt avhengig av tidsintervallet.
		
			// Veit ikkje korleis, men trur at effekta er at 
			// 					- antall synaptic vesicles øker?
			// 					- presyn. membran-areal øker
			// 					- receptorer i posts. øker (veit at responsen til en bestemt mengde n.t. øker..)
			


			// Begynner med å implementere øking av dGlutamatReceptoreIPostsynMem.
			dGlutamatReceptoreIPostsynMem += LTP_HASTIGHET_endringAvAntall_POSTSYN_RECEPTOR;

		}


		// endre dette slik at vektendring skjer vha. kontinuerlige (veldig små) vektendringer. :
		//int antallIterasjonerSidenSistVektendring; // blir oppdatert i regnUtVerdi() og når vekt blir endra.
		unsigned long ulTimestampForrigeSignal; // for LTP.

		/*********** For synaptic short-term depression: ***********/
		
		// Desse er vedtatt som variable i neurolog-milø. (FAKTA):
		unsigned long ulAntallSynapticVesiclesAtt; 		// -For synaptic (short term) depression. 
	       									// Gjør også slepp av s.v. prosentvis (som eit diff-lign. sys.)
	 	double dGlutamatReceptoreIPostsynMem;			// -Beskriver antall neuroReceptore i postsynaptisk membran.

		// Følgande er variable ifølge mi tankerekke (HYPOTESE):
		double dOppladingsFartForSynVesicles; 			// -Er variabel for å ta høgde for potentiation og augmentation..
		unsigned long ulAntallSynV_setpunkt; 			// -For å variere antall syn.vesicles. Min teori om facilitation/ 
										// augmentation / korttids potentiation.
		//double dReproduksjonAvSynV; 		// -For å ha membranstørrelsen variabel.. Sjå (min teori). Trenger også mem. areal-var.
		// 
	public:
		// Constructor for arv i synapse. Potensiellt farlig? Legger inn char, og sjekk om det er f.eks. 't'. ellers; feilmld.
		synapse(char c){ if(c!='t'){ cDebug<<"\n\n\n\nERROR: l 264 i neuroEnhet.h\n\n\n"; exit(0); } } 
		synapse( neuron* pPreN_arg, neuron* pPostN_arg, double v =0 ) : 
				ulAntallSynapticVesiclesAtt  (DEF_startANTALL_SYN_V),
				dOppladingsFartForSynVesicles(DEF_OPPLADINGSFART_FOR_SYN_VESICLES),
				ulAntallSynV_setpunkt 	     (DEF_startANTALL_SYN_V),
				pPreNode(pPreN_arg), pPostNode(pPostN_arg)
		{
			// finn ut om andre måten:
			//   dGlutamatReceptoreIPostsynMem = v : v ? DEF_OVERFOERINGSFAKTOR_FOR_SYN; eller noke
			if( v )
				dGlutamatReceptoreIPostsynMem = v;
			else
				dGlutamatReceptoreIPostsynMem = DEF_OVERFOERINGSFAKTOR_FOR_SYN;
		}

		neuron* pPreNode;
		neuron* pPostNode;

		// FINN på en måte å lage skille mellom eksiterende og inhibiterende synapser. Kanskje bool for eksitatorisk?
		// Dette er viktig når eg begynner med læring / vektendring...
		

		// ikkje i bruk enda:
		//double LTP_og_LTD; 	// for å legge til til signal (synaptisk vekt).
					// Finn ut korleis LTD og LTP sletter kvarandre. Regner med at det er gradvis, og at eg kan bruke kontinuerlig uts.
	




		// Oppdaterer neuron for ny klokkeiterasjon: (lader opp igjen synaptic vesicles i synapse)
		void oppdater()
		{
			// For synaptic depression. Når dProsentSynapticVesiclesAtt tømmes, skal signalet i synapse minke.

			// JOBB: GJØR DENNE DYNAMISK. IKKJE BRUK DEFINES, MEN VARIABLER.
			// 	- Skal ha en variabel for derivert antall syn.v. att. Altså gro-rate.
			// 	- Skal øke dProsentSynapticVesiclesAtt med denne variabelen.
			// 	- Skal endre gro-raten basert på mengde synaptic vesicles igjen. Lag en god regel her.

			unsigned long ulKlokketikkSidenForrigeFyring = ulTidsiterasjoner - ulTimestampForrigeSignal     ;
			
			// 	begynner med variabel faktor-opplading av dProsentSynapticVesiclesAtt.
				// - Det blir litt meir regnekrevande med %-vis opplading (loop): 
					//for(unsigned int i=0; i<ulKlokketikkSidenForrigeFyring; i++ )
			if( ulAntallSynapticVesiclesAtt < ulAntallSynV_setpunkt )
				ulAntallSynapticVesiclesAtt += dOppladingsFartForSynVesicles * (ulKlokketikkSidenForrigeFyring -2) ; 
												// har en tidsforsinkelse på 2

			// Endre gro-raten. Denne skal ha treighet i seg (type syn. depression og potentiation/augmentation)
		//	dOppladingsFartForSynVesicles =  XXX

			
		}



	
/* 		FOR Når eg begynner på LTP og LTD:

		// valg == 0  => kalt fra presyn.
		// valg == 1  => kalt fra postsyn.
		void sjekkSTDP( int valg ) //Spike Time-Dependent(synaptisk) Plastisitet (s 200 i boka)
		{
			// LTP og LTD. ( eller kanskje bare LTP ) (sjå i neuroboka s 200 )
			// 	Forresten. Kan bruke denne funksjonen også fra funk. under. Da med valg 1..

			if( ulTidsiterasjoner == pPostNode->ulTimestampFyring ){
				// presyn fører til at postsyn fyrer. Ikkje gjør noke som resultat av dette.
			}
 
			// Sjekker LTD (om postsyn nylig ( innen 100 ms, siden ) har fyrt )
			if( / * 0 < * /       (ulTidsiterasjoner - pPostNode->ulTimestampFyring) < KONST_FOR_LTP_OG_LTD_TIDSGRENSE ){
				if( valg == 0 ){// når presyn kaller denne.
						// Presyn fyrte, (når valg = 0)
					// LTD:
					LTP_og_LTD -= KONST_FOR_LTP_LTD_ENDRING;
					// TODO Tenk meir på korleis lage til LTP og LTD. Skal også ta med høg-frekvens-over-ei-tid


					// XXX Sjekk om den blir under en viss verdi. I så fall skal synapsta mellom neurona forsvinne.
					// XXX Sjekk om den blir over  en viss verdi. I så fall skal det gro fram nye synapser mellom neurona.
				
				}else // valg == 1
					;
					
			}
			
		}
*/ 
		// trur ikkje eg trenger returverdi:
		virtual void regnUt();
	
		friend std::ostream & operator<< (std::ostream & ut, synapse );
};


#ifndef NEURON
    #include "neuroEnhet.h"
#endif





/*******************************************************************************************************
 ********************    class tidsSkilleElement                                     *******************
 ********************        - Eit tidsskille for neurale nettet. skal iterere       *******************
 ********************          tid, og legge til nytt tidselement i arbeidskøa.      *******************
 ********************        - Alt dette i overloada funskjon, for å sleppe å        *******************
 ********************          endre på der vi  kaller funskjonane i køa.            *******************
 ******************************************************************************************************/
class tidsSkilleElement : public synapse {
	
	public:
		tidsSkilleElement() : synapse('t') { cout<<"\tinne i konstruktor\n"; }

		// gjør heilt andre ting enn synapse. Heiter det samme pga overloading.
		void regnUt(); // Utleda i neuroEnhet.cpp
};





