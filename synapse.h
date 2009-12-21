//#include "main.h" TODO
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

//#define DEF_OPPLADINGSFART_FOR_SYN_VESICLES 	DEF_startANTALL_SYN_V*0.01
#define DEF_OVERFOERINGSFAKTOR_FOR_SYN   	1
#define DEF_startANTALL_SYN_V 		    	1000
#define DEF_OPPLADING_AV_S_V_DIVISOR 		35
#define MAKS_OPPLADNINGSFART_FOR_S_V 		1000
#define OPPDATERINGS_GRENSE_SYN_V 		15 	//Grense får når syn. skal legges i pNesteSynapseSomIkkjeErFerdigOppdatert_Koe 
					//XXX skalværeMeir

#define LTP_hastighet_endringAvAntall_POSTSYN_RECEPTOR  0.05 // Ved LTP...
#define LTD_hastighet_endringAvAntall_POSTSYN_RECEPTOR  0.07 // Ved LTD...

#define LTP_INITIALISERINGS_TERSKEL 			750 

// klasse-deklarasjoner:
class synapse;
class neuron;
class neuroSensor;
class arbeidsHistorieElement;



/* Extern-deklarasjoner */
extern unsigned long ulTidsiterasjoner;

//extern list<synapse*> 		    pNesteSynapseUtregningsKoe;
extern list<arbeidsHistorieElement*> pArbeidsHistorieListe;

extern list<neuroSensor*> pNeuroSensorListe; // extern for at den kan deklareres her.







/* ------------------------------------------------------------------------------------------------------
 *  --------                           PLAN:                                                  -----------
 *  --------  Gjør slik at ladingsfarta for synaptic vesicles er bra. No går han rett opp over-----------
 *  --------      til 113 (%). Uten provosering. Skal gå til 100. I tilfelle augmentation,    -----------
 *  --------      skal den gå over.                                                           -----------
 *  -----------------------------------------------------------------------------------------------------*/



/*******************************************************************************************************
 ********************    class synapse                                               ******************* synapse( neuron*, neuron*, double v =DEF_OVER..);
 ********************        - output fra neuron. ala biologiske neuron.             *******************
 ********************        - LTP, LTD (lavnivås læring) skal skje her.             *******************
 ********************                                                                *******************
 ******************************************************************************************************/
class synapse {
	private:
		const bool bInhibitorisk_effekt;
		// overvåkinga skjer annen plass. Her skjer bare initiering (og kanskje vedlikehold).
		
		// uForrigeAntallSynapticV_slept
		unsigned uAntallSynapticV_sluppet; // Er denne i bruk? XXX
		//XXX FINN MÅTE Å LAGE 5-6 ELEMENT-LANG MA-filter for systemtreighet. ???


		void LTP( int nVerdi){
			// XXX Finn ut korleis dette funker. Når du finner ut dette, publiser artikkel, og skriv om denne funskjonen.
			
			// Finn ut om vektendring er omvendt proposjonalt avhengig av tidsintervallet.
		
			// Veit ikkje korleis, men trur at effekta er at 
			// 					- antall synaptic vesicles øker?
			// 					- presyn. membran-areal øker
			// 					- receptorer i posts. øker (veit at responsen til en bestemt mengde n.t. øker..)
			


			// Begynner med å implementere øking av dGlutamatReceptoreIPostsynMem.
			// Vil lage en slags "sigama-funk for dette. også som arg: kor mykje den har økt nylig (type to input på rappen, med depolarisering
			// på 88 og 92 % skal ikkje føre til to store hopp, men ... Eller skal det kanskje det? Dette viser at denne signalveien er viktig.
			fGlutamatReceptoreIPostsynMem += (LTP_hastighet_endringAvAntall_POSTSYN_RECEPTOR * nVerdi/100) ; //Kvifor /100 ?
			cout 	<<"**** * * LTP * * **** \t\t\t\t\t* * * * * LTP * * * * Øker med " 
				<<LTP_hastighet_endringAvAntall_POSTSYN_RECEPTOR <<" * " <<nVerdi <<"% = "
				<<LTP_hastighet_endringAvAntall_POSTSYN_RECEPTOR * nVerdi/100
				<<" \tNye verdier er " <<fGlutamatReceptoreIPostsynMem <<" per synaptic vesicle.\t\t\t*" ;

		}

		void homoLTD(){
	 		// Minker med vektene med litt litt litt.
			// TODO Foreløpig er det konstant endring ved LTD. Gjør det til at det øker men påfølgende LTD ?
			fGlutamatReceptoreIPostsynMem -= (LTD_hastighet_endringAvAntall_POSTSYN_RECEPTOR / 100);
			cout<<"litt homoLTD. => minker vekta litt litt litt. \t\t\t\t\t\t\t\t\t\t*\n";

			//if( uGlutamatReceptoreIPostsynMem == 0) døøø Fjærn synapsen, om den er forr lita vekt på.
		}



		unsigned long ulTimestampForrigeSignal;
		unsigned long ulTimestampForrigeOppdatering; /// Hovedsaklig for å unngå å oppdatere fleire ganger kvar iterasjon..

		/*********** For synaptic short-term depression: ***********/
		
		// Desse er vedtatt som variable i neurolog-milø. (FAKTA):
		unsigned long ulAntallSynapticVesiclesAtt; 		// -For synaptic (short term) depression. 
	       									// Gjør også slepp av s.v. prosentvis (som eit diff-lign. sys.)
		unsigned long ulSynapticVesicles_i_membran; 		// skal ha mulighet for å reproduseres.

	 	float fGlutamatReceptoreIPostsynMem;				// -Beskriver antall neuroReceptore i postsynaptisk membran.
		// Det finnes en uTotaltAntallReceptoreIPostsynNeuron_setpunkt; som skal være med å styre variabelen over, når postsyn fyrer.
		
		// Følgande er variable ifølge mi tankerekke (HYPOTESE):
		double dOppladingsFartForSynVesicles; 			// -Er variabel for å ta høgde for potentiation og augmentation..
		unsigned long ulAntallSynV_setpunkt; 			// -For å variere antall syn.vesicles. Min teori om facilitation/ 
										// augmentation / korttids potentiation.
		//double dReproduksjonAvSynV; 		// -For å ha membranstørrelsen variabel.. Sjå (min teori). Trenger også mem. areal-var.
	
	protected:
	

		// neste: skal ha litt meir treighet i systemet for opplading. Kanskje eit MA-filter med 5-6 ledd.. Eller membran-hypotesen, og regen.



	
		// oppdater() :
		// 	arg: 	void
		// 	retur: 	Returnerer 1 ved suksess. I arva klasser brukes det til annet (f.eks. i synSkilleElement betyr -47 retur at 
		// 		  den er kalt i eit synSkilleElement. Dette har muliggjør : while( oppdateringskø.front()->oppdater() );
		// Oppdaterer neuron for ny klokkeiterasjon: (lader opp igjen synaptic vesicles i synapse)

		virtual int oppdater()
		{
			cout<<"\tOPPDATER()\n";
	
			// Dersom den er kalt fra pNesteSynapseSomIkkjeErFerdigOppdatert_Koe (eller første element er denne..)
			if( this == synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front() ){
				// så fjærner det første element fra oppdateringsliste:
				synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.pop_front();   
			}		

///XXX HER XXX
			// Sjekker om den har oppdatert denne iterasjonen:
			if( ulTimestampForrigeOppdatering == ulTidsiterasjoner ){
	 			cout<<"\tXET503\t";
	 			return 1; // eller kanskje noke anna. Bare ikkje 0 som returverdi.
			}
			

		
		
		
			// For synaptic depression. Når dProsentSynapticVesiclesAtt tømmes, skal signalet i synapse minke.

			// 	- Skal endre gro-raten basert på mengde synaptic vesicles igjen. Lag en god regel her.


/* ******************************************************************************************************
				No vil eg at den skal bare kjøre loopane, litt lenger ned, når den ikkje var i orden før pausa.
				(dersom den er nådd referansepkt. før eit opphold i tid. )
				* Kanskje det ikkje kan være opphold i tid, siden eg oppdaterer til den er oppdatert. Da er den klar..


 * *********************************************************************************************************************************************************/ 

		// 2 element i opplading av synaptic vesicles: syntese, reproduksjon, 
			// TRENGER IKKJE loop: for(unsigned int i=0; i<ulKlokketikkSidenForrigeOppdateringTemp; i++ ){
			// (dette er effekten eg prøver på gjennom kall fra synSkilleElement.oppdater()...)

			// syntese av heilt nye S.V. reproduksjon skal skje seinare plass i koden. 	Veit ikkje om er så viktig med static - timedelay...
			static int snForrigeSynteseAvSV = 0;
			ulAntallSynapticVesiclesAtt += snForrigeSynteseAvSV;
			// (dersom den er negativ, skal denne også sørge for å ta vekk litt syn. v.(sjå slutt av denne funk) )

			// reproduksjon av S.V.
			int nEndringImembranTemp = (ulSynapticVesicles_i_membran * 0.3   +0.5); //(+0.5) for å runde opp til int over.
			ulAntallSynapticVesiclesAtt  += nEndringImembranTemp;
			ulSynapticVesicles_i_membran -= nEndringImembranTemp;

			
			
			cout 	<<"\n\t\tS.V. " <<ulAntallSynapticVesiclesAtt <<"\t| membran:\t" <<ulSynapticVesicles_i_membran
				<<"\t(    " <<(int)(nEndringImembranTemp+((signed)ulAntallSynV_setpunkt-(signed)ulAntallSynapticVesiclesAtt)*0.05) <<" S.V.)"
				<<"\t\t|=|  " <<nEndringImembranTemp <<" regen. og " 
				<<snForrigeSynteseAvSV <<" er nye. \n"; 
			cDebug<<"nEndringImembranTemp: " <<nEndringImembranTemp <<"     . ulAntallSynV_setpunkt: " <<ulAntallSynV_setpunkt 
				<<"  . ulAntallSynapticVesiclesAtt: " <<ulAntallSynapticVesiclesAtt 	<<endl;

			if( 		(ulSynapticVesicles_i_membran < 3*OPPDATERINGS_GRENSE_SYN_V)  	&& 
					( abs(ulAntallSynV_setpunkt-ulAntallSynapticVesiclesAtt) < OPPDATERINGS_GRENSE_SYN_V )   ){
	 			// nullstill avviket.
				cout<<"\t\tFERDIG\n\n";
				ulAntallSynapticVesiclesAtt = ulAntallSynV_setpunkt;
				ulSynapticVesicles_i_membran = 0; /// ?? XXX Kanskje dette skal vekk, etterkvart.

			}else{
				cout<<"\t\tikkje ferdig.\n\n";
	 			pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.push_back( this );
			}

/* forenkla måte. Funker.			
			//begynner uten MA-filter (bare eit ledd tilbake (=siste ledd) )
			//MEINEINEI: skal ha membran-effekt istadenfor. treledds: synV-mem-førFabrikkOgIfabrikk tilbake til start..
			//
			// Endre gro-raten. Denne skal ha treighet i seg (type syn. depression og potentiation/augmentation)
			long ulTempDiff;
			if( ( ulTempDiff = ulAntallSynV_setpunkt-ulAntallSynapticVesiclesAtt) > MAKS_OPPLADNINGSFART_FOR_S_V ){
				ulTempDiff = MAKS_OPPLADNINGSFART_FOR_S_V;
	 			// TTT XXX TODO ,og legg til oppdateringsjobb sist i arbeidsliste.
			}
 			dOppladingsFartForSynVesicles = ulTempDiff / DEF_OPPLADING_AV_S_V_DIVISOR;
			cout<<"-ulTempDiff: " <<ulTempDiff <<" (ulAntallSynapticVesiclesAtt-ulAntallSynV_setpunkt)" <<endl;
			cout <<"-( " <<ulAntallSynapticVesiclesAtt <<", " <<ulAntallSynV_setpunkt <<" )\n";
			//kanskje eg skal ha med anna effekt, som fører til ulineær kurve i utladinga i ovesvingen? isåfall lett å legge den her:
			if( dOppladingsFartForSynVesicles < 0 ) dOppladingsFartForSynVesicles = 0;
*/
			//for å la facilitation "ebbe ut", kjører eg konstant "lekking" av syn.v. (sjekker først om den er ulik 0)
			if(ulAntallSynapticVesiclesAtt)
				ulAntallSynapticVesiclesAtt -= 3 ;
			
			// Oppdaterer timestamp for oppdatering av syn.
			ulTimestampForrigeOppdatering = ulTidsiterasjoner;
			
			// Neste iterasjons syntese av S.V. :
  			snForrigeSynteseAvSV = ((signed)ulAntallSynV_setpunkt - (signed)ulAntallSynapticVesiclesAtt ) * 0.2;
			// Dersom den er negativ, halver den. Andre effekter som styrer sletting av s.v. ...
			if(snForrigeSynteseAvSV<0) snForrigeSynteseAvSV/=3;

			return 1;
		}
			

	public:
		// Constructor for arv i synapse. Potensiellt farlig? Legger inn char, og sjekk om det er f.eks. 't'. ellers; feilmld.
		synapse(char c) : bInhibitorisk_effekt(false) { if(c!='t'){ cDebug<<"\n\n\n\nERROR: l 264 i neuroEnhet.h\n\n\n"; exit(0); } } 
		synapse( neuron* pPreN_arg, neuron* pPostN_arg, bool argInhibitorisk_effekt =false, float v =1 );
		// Denne legger seg automagisk til i postsyn. si innsyn.liste, og presyn. utsyn. liste.

		neuron* pPreNode;
		neuron* pPostNode;


		static list<synapse*>  pNesteSynapseUtregningsKoe;
		static list<synapse*>  pNesteSynapseSomIkkjeErFerdigOppdatert_Koe;	
		
		



		// ikkje i bruk enda:
		//double LTP_og_LTD; 	// for å legge til til signal (synaptisk vekt).
					// Finn ut korleis LTD og LTP sletter kvarandre. Regner med at det er gradvis, og at eg kan bruke kontinuerlig uts.
	







	
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
			if(   (ulTidsiterasjoner - pPostNode->ulTimestampFyring) < KONST_FOR_LTP_OG_LTD_TIDSGRENSE ){
				if( valg == 0 ){// når presyn kaller denne.
						// Presyn fyrte, (når valg = 0)
					// LTD:
					LTP_og_LTD -= KONST_FOR_LTP_LTD_ENDRING;

					// XXX Sjekk om den blir under en viss verdi. I så fall skal synapsta mellom neurona forsvinne.
					// XXX Sjekk om den blir over  en viss verdi. I så fall skal det gro fram nye synapser mellom neurona.
				
				}else // valg == 1
					;
					
			}
			
		}
*/ 
		// trur ikkje eg trenger returverdi:
		virtual void aktiviserOgRegnUt();
	
		void skrivUt(){ cout<<"tulleutskrift. test.\n"; /**this;*/ }
		friend std::ostream & operator<< (std::ostream & ut, synapse );
		friend std::ostream & operator<< (std::ostream & ut, neuron );
		friend int initArbeidskoer();
		friend class synSkilleElement;
};


/* *********************************************************************
 * *****     Alternativ er å ha en bool som heiter inhibitorisk,   *****
 * *****       og if(inhibitorisk) signal *= (-1);                 *****
 * *****       Avhengig av kva som er raskest. (peikeroppslag (for *****
 * *****       peiker-funk.),  eller if-setning)                   *****
 * *********************************************************************/ 

#ifndef NEURON
    #include "neuroEnhet.h"
#endif





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

		// gjør heilt andre ting enn synapse. Heiter det samme pga overloading.
		void aktiviserOgRegnUt(); // Utleda i neuroEnhet.cpp

		// Legger seg sjølv til sist i lista, og returnerer 0 (symboliserer at liste er ferdig (det før skilleelementet) )
		int oppdater(){ 
			// kan være lurt å for sikkerhetsskuld ta med if(pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front() == this )  - Men treigare..
			if( synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front() != this ){ cout<<"FEIL synapse.h_52"; exit(-1); }
			/* IKKJE LØYSING: feilsjekk..  */

			// legg til på slutt:
			synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.push_back(this);   	
			// Fjærner dette element fra oppdateringsliste:
			synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.pop_front();   

			return 0;
		}
	       	/* return 0 er heile poenget med int-returverdi her. Skal ret. 0 ved synSkilleElement, og 1 ved vanlig ok
		 * 	Dersom det er vanlig oppdatering av vanlig synapse, vil den returnere 1, her returnerer den 0, Dette gjør at eg kan kjøre
		 * 	while( ! (kø++)->oppdater() );  // trur ikkje noke skal gjøres. Kanskje utskrift eller noke..
		 */

};




