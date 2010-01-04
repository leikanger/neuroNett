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







#define DEF_FORHOLD_FRI_VS_MEM_SV 0.2





#define cDebug std::cerr


/***************** mykje bås her, no : *******************/

//#define DEF_OPPLADINGSFART_FOR_SYN_VESICLES 	DEF_startANTALL_SYN_V*0.01
#define DEF_OVERFOERINGSFAKTOR_FOR_SYN   	1
#define DEF_startANTALL_SYN_V 		    	1000
#define DEF_OPPLADING_AV_S_V_DIVISOR 		35
#define MAKS_OPPLADNINGSFART_FOR_S_V 		1000
#define OPPDATERINGS_GRENSE_SYN_V 		1//7 	//Grense får når syn. skal legges i pNesteSynapseSomIkkjeErFerdigOppdatert_Koe 
					//XXX skalværeMeir
#define MAX_NEG_SYNTESE_AV_SV 			-3
#define MAX_SYNTESE_AV_SV 			15

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





#ifndef SYNAPSE_H
  #define SYNAPSE_H
  
  #include "neuroEnhet.h"



/*******************************************************************************************************
 ********************    class synapse                                               ******************* synapse( neuron*, neuron*, double v =DEF_OVER..);
 ********************        - output fra neuron. ala biologiske neuron.             *******************
 ********************        - LTP, LTD (lavnivås læring) skal skje her.             *******************
 ********************                                                                *******************
 ******************************************************************************************************/
class synapse {
	protected:
	
		const bool bInhibitorisk_effekt;
		
		// MA-effekt på reproduksjon, Nei. Men ta med forrige. Tenk meir.

		unsigned uAntallSynapticV_sluppet; // Er denne i bruk? XXX
		// for systemtreighet. ???
		//Eller finn først ut om dette er veien å gå..

		int nBestilltSynteseAvSVFraForrigeIter;
		int nBestilltReproduksjonAvSvFraForrigeIter;

		std::ofstream utskriftsFilLogg;


/* 	Gammelt drit. Bedre med simulering av hypotese.
 *
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
			co ut 	<<"**** * * LTP * * **** \t\t\t\t\t* * * * * LTP * * * * Øker med " 
				<<LTP_hastighet_endringAvAntall_POSTSYN_RECEPTOR <<" * " <<nVerdi <<"% = "
				<<LTP_hastighet_endringAvAntall_POSTSYN_RECEPTOR * nVerdi/100
				<<" \tNye verdier er " <<fGlutamatReceptoreIPostsynMem <<" per synaptic vesicle.\t\t\t*" ;

		}

		void homoLTD(){
	 		// Minker med vektene med litt litt litt.
			// TODO Foreløpig er det konstant endring ved LTD. Gjør det til at det øker men påfølgende LTD ?
			fGlutamatReceptoreIPostsynMem -= (LTD_hastighet_endringAvAntall_POSTSYN_RECEPTOR / 100);
			co ut<<"litt homoLTD. => minker vekta litt litt litt. \t\t\t\t\t\t\t\t\t\t*\n";

			//if( uGlutamatReceptoreIPostsynMem == 0) døøø Fjærn synapsen, om den er forr lita vekt på.
		}
*/


		//unsigned long ulTimestampForrigeSignal; 	// For ?? 
		unsigned long ulTimestampForrigeOppdatering; /// Hovedsaklig for å unngå å oppdatere fleire ganger kvar iterasjon..

		/*********** For synaptic short-term depression: ***********/
		
		// Desse er vedtatt som variable i neurolog-milø. (FAKTA):
		unsigned long ulAntallSynapticVesiclesAtt; 		// -For synaptic (short term) depression. 
	       									// Gjør også slepp av s.v. prosentvis (som eit diff-lign. sys.)
		unsigned long ulSynapticVesicles_i_membran; 		// skal ha mulighet for å reproduseres.

	 	float fGlutamatReceptoreIPostsynMem;				// -Beskriver antall neuroReceptore i postsynaptisk membran.
		// Det finnes en uTotaltAntallReceptoreIPostsynNeuron_setpunkt; som skal være med å styre variabelen over, når postsyn fyrer.
		
		// Følgande er variable ifølge mi tankerekke (HYPOTESE):
		//double dOppladingsFartForSynVesicles; 			// -Er variabel for å ta høgde for potentiation og augmentation..
		unsigned long ulAntallSynV_setpunkt; 			// -For å variere antall syn.vesicles. Min teori om facilitation/ 
										// augmentation / korttids potentiation.
		//double dReproduksjonAvSynV; 		// -For å ha membranstørrelsen variabel.. Sjå (min teori). Trenger også mem. areal-var.
	

		// neste: skal ha litt meir treighet i systemet for opplading. Kanskje eit MA-filter med 5-6 ledd.. Eller membran-hypotesen, og regen.


		/********* Hjelpefunksjon for sending av syn.v. Tar hånd om det som skal. *******************/
		void mergeSynapticVesicles(int antallArg =0);


	
		// oppdater() :
		// 	arg: 	void
		// 	retur: 	Returnerer 1 ved suksess. I arva klasser brukes det til annet (f.eks. i synSkilleElement betyr -47 retur at 
		// 		  den er kalt i eit synSkilleElement. Dette har muliggjør : while( oppdateringskø.front()->oppdater() );
		// Oppdaterer neuron for ny klokkeiterasjon: (lader opp igjen synaptic vesicles i synapse)

		virtual int oppdater()
		{
	//		cout<<"\tOPPDATER()";
	
			// Dersom den er kalt fra pNesteSynapseSomIkkjeErFerdigOppdatert_Koe (eller første element er denne..)
			if( this == synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front() ){
				// så fjærner det første element fra oppdateringsliste:
				synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.pop_front();   
			}		


			// Sjekker om den har oppdatert denne iterasjonen:
			if( ulTimestampForrigeOppdatering == ulTidsiterasjoner ){
	 			cout<<"\ntimestamp for oppdatering er allerede tatt (har redan oppdatert denne iter).\n";
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

			// SYNTESE:
			ulAntallSynapticVesiclesAtt += nBestilltSynteseAvSVFraForrigeIter;
			// (dersom den er negativ, skal denne også sørge for å ta vekk litt syn. v.(sjå slutt av denne funk) )

			// REPRODUKSJON:
			// reproduksjon av S.V.
			ulAntallSynapticVesiclesAtt  += nBestilltReproduksjonAvSvFraForrigeIter;
			ulSynapticVesicles_i_membran -= nBestilltReproduksjonAvSvFraForrigeIter;

			
/* //{ utskrift			
			co ut 	<<"\n\t\tS.V. " <<ulAntallSynapticVesiclesAtt <<" / " <<ulAntallSynV_setpunkt
			       	<<"\t| membran:\t" <<ulSynapticVesicles_i_membran <<"\t\t|=|  " <<nBestilltReproduksjonAvSvFraForrigeIter <<" regen. og " 
				<<nBestilltSynteseAvSVFraForrigeIter <<" er nye. \n"; 
				//<<(int)(nBestilltReproduksjonAvSvFraForrigeIter+((signed)ulAntallSynV_setpunkt-(signed)ulAntallSynapticVesiclesAtt)*0.05) 

			cDebug 	<<" ulAntallSynapticVesiclesAtt :        \t" <<ulAntallSynapticVesiclesAtt <<" / " <<ulAntallSynV_setpunkt <<endl;
*/ //}
	
			if( 		(ulSynapticVesicles_i_membran < OPPDATERINGS_GRENSE_SYN_V)  	&& 
					( abs(ulAntallSynapticVesiclesAtt-ulAntallSynV_setpunkt) < 3*OPPDATERINGS_GRENSE_SYN_V )   ){
	 			// nullstill avviket.
				cout<<"\n\n\t\tFERDIG\n\n";
				ulAntallSynapticVesiclesAtt = ulAntallSynV_setpunkt;
				ulSynapticVesicles_i_membran = 0; /// ?? XXX Kanskje dette skal vekk, etterkvart.

			}else{
				//co ut<<"\t\tikkje ferdig.\n\n";
	 			pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.push_back( this );
			}

			/*for å la facilitation "ebbe ut", kjører eg konstant "lekking" av syn.v. (sjekker først om den er ulik 0)
			if(ulAntallSynapticVesiclesAtt)
				ulAntallSynapticVesiclesAtt -= 3 ;
			flytta til syntese-plassen, for å ha %-vis effekt.*/
			
			// Oppdaterer timestamp for oppdatering av syn.
			ulTimestampForrigeOppdatering = ulTidsiterasjoner;
			
			// Neste iterasjons syntese av S.V. : 	( Lita MA-effekt for å smoothe det ut litt.)
  			nBestilltSynteseAvSVFraForrigeIter += ((signed)ulAntallSynV_setpunkt - (signed)ulAntallSynapticVesiclesAtt ) * 0.25;
			nBestilltSynteseAvSVFraForrigeIter /= 2;

			// Dersom den er negativ, halver den. Andre effekter som styrer sletting av s.v. ...
			if(nBestilltSynteseAvSVFraForrigeIter<0){
			 	nBestilltSynteseAvSVFraForrigeIter/=3;

				// setter tak for sletting av s.v.
//		 		XXX For å sette maks fjerning av nBestilltSynteseAvSVFraForrigeIter
		 		if(nBestilltSynteseAvSVFraForrigeIter<MAX_NEG_SYNTESE_AV_SV )	
					nBestilltSynteseAvSVFraForrigeIter = MAX_NEG_SYNTESE_AV_SV;
			}else{
				if(nBestilltSynteseAvSVFraForrigeIter> MAX_SYNTESE_AV_SV ) nBestilltSynteseAvSVFraForrigeIter=MAX_SYNTESE_AV_SV;
			}

			// Nester iterasjons reproduksjon av S.V. :
			nBestilltReproduksjonAvSvFraForrigeIter = (ulSynapticVesicles_i_membran * 0.3   +0.5); //(+0.5) for å runde opp til int over.

			
			utskriftsFilLogg<<"\t" <<ulTidsiterasjoner <<"\t" <<ulAntallSynapticVesiclesAtt <<"\t1000\t" <<"\t" <<ulSynapticVesicles_i_membran <<";\n";
			utskriftsFilLogg.flush();


			return 1;
		}
			

	public:
		// Constructor for arv i synapse. Potensiellt farlig? Legger inn char, og sjekk om det er f.eks. 't'. ellers; feilmld.
		synapse(char c) : bInhibitorisk_effekt(false) { if(c!='t'){ cDebug<<"\n\n\n\nERROR: l 264 i neuroEnhet.h\n\n\n"; exit(0); } } 
		synapse( neuron* pPreN_arg, neuron* pPostN_arg, bool argInhibitorisk_effekt =false, float v =1 );
		// Denne legger seg automagisk til i postsyn. si innsyn.liste, og presyn. utsyn. liste.

		// Destructor, for å fjærne seg sjølv fra postsyn.inn-syn-liste, og presyn:utsyn-liste.
		~synapse();

		neuron* pPreNode;
		neuron* pPostNode;

		static list<synapse*>  pNesteSynapseUtregningsKoe;
		static list<synapse*>  pNesteSynapseSomIkkjeErFerdigOppdatert_Koe;	
		
		



		// ikkje i bruk enda:
		//double LTP_og_LTD; 	// for å legge til til signal (synaptisk vekt).
					// Finn ut korleis LTD og LTP sletter kvarandre. Regner med at det er gradvis, og at eg kan bruke kontinuerlig uts.
	







	
// 		FOR Når eg begynner på LTP og LTD:	void sjekkSTDP( int valg ) //Spike Time-Dependent(synaptisk) Plastisitet (s 200 i boka) 
		// trur ikkje eg trenger returverdi:
		virtual void aktiviserOgRegnUt();
		
		friend std::ostream & operator<< (std::ostream & ut, synapse );
		friend std::ostream & operator<< (std::ostream & ut, neuron );
		friend int initArbeidskoer();
		friend class synSkilleElement;

		// Bare mens eg drive å utvikler.
		friend int main(int, char**);
};













/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

/* *********************************************************************
 * *****     class synapse_likevekt : public synapse               *****
 * *****         - for å teste teorien om likevekt mellom s.v.     *****
 * *****           i mem., og frie s.v.                            *****
 * *********************************************************************/ 
class synapse_likevekt : public synapse{
	private:
 		//float f Forhold_frie_vs_membran_SV; Skal ikkje ha..
		int nSynapticSkall_tommeSV;
		int nBestiltRefillAvTommeSV;

		int nRaavarer_til_SV_syntese;
		
		// denne skal ha funksjon å sette mål for antall SV i mem. Skal oppdateres kontinuerlig, som eit MA-filter av antall SV i mem. 
		unsigned long ulSynapticVesicles_setpunkt_for_membran;

	public:
		synapse_likevekt( neuron* pPreN_arg, neuron* pPostN_arg, bool argInhibitorisk_effekt =false, float v =1 )
			: synapse(pPreN_arg, pPostN_arg, argInhibitorisk_effekt , v ), 
	//		f Forhold_frie_vs_membran_SV(DEF_FORHOLD_FRI_VS_MEM_SV), 
			nRaavarer_til_SV_syntese(0),
	     		nBestiltRefillAvTommeSV(0)	
			{
				//ulAntallSynapticVesiclesAtt  = (1-DEF_FORHOLD_FRI_VS_MEM_SV) * ulAntallSynapticVesiclesAtt;
				ulSynapticVesicles_i_membran = ulAntallSynapticVesiclesAtt * /*f Forhold_frie_vs_membran_SV*/DEF_FORHOLD_FRI_VS_MEM_SV ;
				ulSynapticVesicles_setpunkt_for_membran = ulSynapticVesicles_i_membran;

				nSynapticSkall_tommeSV = ulAntallSynapticVesiclesAtt * 0.1;

				cout<<"\nLager likevekts-synapse, med:\t\tS.V. i mem.:\t" <<ulSynapticVesicles_i_membran <<"\t, frie S.V.: "
					<<ulAntallSynapticVesiclesAtt <<" osv.\n";
		
			}
	protected:
		// oppdater() :
		// 	arg: 	void
		// 	retur: 	Returnerer 1 ved suksess. I arva klasser brukes det til annet (f.eks. i synSkilleElement betyr -47 retur at 
		// 		  den er kalt i eit synSkilleElement. Dette har muliggjør : while( oppdateringskø.front()->oppdater() );
		// Oppdaterer neuron for ny klokkeiterasjon: (lader opp igjen synaptic vesicles i synapse)


#define DEF_PLASS_TIL_AV_SV 1000
		void fabrikk_av_SV( ){

			// Kanskje denne skal være konst stor, og moderert nedover som funk av råvarer?
			
#define MAXPRODUKSJON_SV 10
			int nProduserAntall = MAXPRODUKSJON_SV;
 			
			int tempPlass(DEF_PLASS_TIL_AV_SV - (nSynapticSkall_tommeSV+(signed)ulAntallSynapticVesiclesAtt) );
			if( tempPlass < 0 ){
	 			// det er ingen plass igjen til nye S.V. :  reduser produksjonen.
				nProduserAntall += 0.5 * tempPlass; //tempPlass er negativ.
			}	

			//min-produksjon er 0:
			// if( nProduserAntall < 0 ) nProduserAntall = 0; Ikkje lenger mulig. Blir gjordt i oppdater()
			//max-produksjon ?:
	 	 	//if( nProduserAntall > MAXPRODUKSJON_SV ) 	nProduserAntall = MAXPRODUKSJON_SV;
#define MAX_SYNTESE_gitt_av_RAAVARER_PROSENT 0.3			 // For eksempel 60% av råvarene.
			int max_syntese_gitt_av_mengde_raavarer( nRaavarer_til_SV_syntese * MAX_SYNTESE_gitt_av_RAAVARER_PROSENT ) ;
			if( nProduserAntall > max_syntese_gitt_av_mengde_raavarer ){
				// Trekk fra halvparten av overskudd over max:
			 	nProduserAntall -= 0.5 * (nProduserAntall - max_syntese_gitt_av_mengde_raavarer) ;
			}

			//
		
			ulAntallSynapticVesiclesAtt 	+= nProduserAntall;
			// XXX XXX XXX XXX XXX XXX XXX XXX X Skal være (maksimalt?)  prosent av gjenværande.. : TODO 			TODO
			nRaavarer_til_SV_syntese 	-= nProduserAntall;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
			 * Bør ha det slik at råvarene blir litt mindre med reproduksjon. Dette gjøres for at eg må ha tilbakemelding fra syn.kløft
			 * om at postsyn. fyrer. Da går ikkje råvarene tom. Ellers gjør de de, etter potentiation-forløpet.
			 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



#define OVERGANG_TOMME_SV_TIL_FABRIKK_PROSENT 0.2
			// Korleis får eg nye råvarer.
			int tempOvergangTommeSV_til_fabrikk = nSynapticSkall_tommeSV * OVERGANG_TOMME_SV_TIL_FABRIKK_PROSENT; 
			nSynapticSkall_tommeSV  -= tempOvergangTommeSV_til_fabrikk;
			nRaavarer_til_SV_syntese += tempOvergangTommeSV_til_fabrikk;

			cout 	<<"råvarer til syntese i fabrikk:\t" <<nRaavarer_til_SV_syntese 
				<<"\tSV skall:\t" <<nSynapticSkall_tommeSV 
				<<endl;
		}

		virtual int oppdater()
		{
			//{ likt det fra synapse.
			cout<<"\tOPPDATER()";
			cout<<"  LIKEVEKT: \tSV i mem: " <<ulSynapticVesicles_i_membran <<" og frie: " <<ulAntallSynapticVesiclesAtt <<"\n";
			// Dersom den er kalt fra pNesteSynapseSomIkkjeErFerdigOppdatert_Koe (eller første element er denne..)
			if( this == synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front() ){
				// så fjærner det første element fra oppdateringsliste:
				synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.pop_front();   
			}		


			// Sjekker om den har oppdatert denne iterasjonen:
			if( ulTimestampForrigeOppdatering == ulTidsiterasjoner ){
	 			cout<<"\ntimestamp for oppdatering er allerede tatt (har redan oppdatert denne iter).\n";
	 			return 1; // eller kanskje noke anna. Bare ikkje 0 som returverdi.
			}

			// alltid legge til synapse tilbake i oppdateringsliste. Endre dette..?.
			pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.push_back( this );

			// Oppdaterer timestamp for oppdatering av syn.
			ulTimestampForrigeOppdatering = ulTidsiterasjoner;
			

			// legger til ekstra steg: tomme SV.
			nSynapticSkall_tommeSV 		+= nBestilltReproduksjonAvSvFraForrigeIter;  	// TODO fiks MA-filter på smart måte, slik at
			// gjør dette på anna måte, slik at neste ligning også kan trekke fra tilsvarande: SynapticSkall_tommeSV 	/= 2; // : MA-komponent.
			ulSynapticVesicles_i_membran 	-= nBestilltReproduksjonAvSvFraForrigeIter;

			// fyller n% av de tomme S.V. med neurotransmittore:
			ulAntallSynapticVesiclesAtt 	+= nBestiltRefillAvTommeSV;
			nSynapticSkall_tommeSV 		-= nBestiltRefillAvTommeSV;


			fabrikk_av_SV( );

/*
// bl'æææ:
			// Neste iterasjons syntese av S.V. : 	( Lita derivat-effekt for å smoothe det ut litt.)
  			n BestilltSynteseAvSVFraForrigeIter += ((signed)ulAntallSynV_setpunkt - (signed)(ulAntallSynapticVesiclesAtt) ) * 0.25;
			n BestilltSynteseAvSVFraForrigeIter /= 2;

*/


			/**********************************************************************************************************************************
		
			*/
#define ENDRINGSFART_FAKTOR  300 
/*
			// XXX XXX XXX XXX XXX XXX XXX 
			// XXX Skal ikkje være slik, men eg veit ikkje korleis eg vil ha det. Sliten i hodet, no. XXX
			float fForhold = (float)ulSynapticVesicles_i_membran / ((float)ul AntallSynapticVesiclesAtt + (float)ul SynapticSkall_tommeSV) ;
			
			// Tull: (kan takes inn i if-setn., der tempInt står..)
			int tempInt =  (int)(( (fForhold - DEF_FORHOLD_FRI_VS_MEM_SV ) * ENDRINGSFART_FAKTOR) + 0.5 ) ;

			// Legg begge desse inn i mergeSynapticVesicles(), bare at funk skal også ta hand om signed int. Dette for å gjøre dette:
 			if( fForhold > DEF_FORHOLD_FRI_VS_MEM_SV ){
				//XXX her skal det variere med antall sv totalt. og med forhold. = bare med ulSynapticVesicles_i_membran ???
				n BestilltReproduksjonAvSvFraForrigeIter = tempInt;
			
			}else {
				cout<<"skal gjøre: mergeSynapticVesicles( ... ) M.E.P.P. \n";
				mergeSynapticVesicles( -tempInt ); // M.E.P.P.
				// ta den direkte: n BestilltReproduksjonAvSvFraForrigeIter [+=/=] forhold * faktor
			// faktor skal variere ved LTP/LTD.
	//XXX sleep(1); // bare for debug..
			}

*/
#define REPROD_av_MEM_FART 0.1
			/* Bestilling blir laga her, og moderert i if(nGjenvaerendePlassTilSV<0) - Eller mao. når det er overskudd av S.V. i presyn. plasma. */
			nBestilltReproduksjonAvSvFraForrigeIter 	+= ulSynapticVesicles_i_membran * REPROD_av_MEM_FART ; // En viss prosent av membran.
			nBestilltReproduksjonAvSvFraForrigeIter 	/= 2; 								// MA-effekt, for å smoothe..
				// TULL: (signed)(ulSynapticVesicles_setpunkt_for_membran-ulSynapticVesicles_i_membran)   * 	REPROD_av_MEM_FART;

			/*
			 * Fiks nBestilltSynteseAvSVFraForrigeIter. Gjøre slik at det blir overshoot i antall SV. (presyn. potentiation-effekt)
			 *	- kanskje ved at nGjenvaerendePlassTilSV bare virker dempande (når den er negativ(overskudd) - i else lenger nede )
			 *	  => ellers: ikkje med i beregninga.
			 */
					
								//  XXX += ??? XXX eller bare =
#define SYNTESE_FART_av_SV 0.3
			int nGjenvaerendePlassTilSV 	=  DEF_PLASS_TIL_AV_SV - (nSynapticSkall_tommeSV + (signed)ulAntallSynapticVesiclesAtt) ;
			
			//DETTE ER BARE MAKSVERDIEN. Blir moderert i fabrikk.
			//nBestilltSynteseAvSVFraForrigeIter 	= 20;  //Setter den til konst høg. Blir moderert i fabrikk()
					//var: //nGjenvaerendePlassTilSV * SYNTESE_FART_av_SV;
			cout 	<<"noke..\t\t\t" <<",\trepro: \t\t" <<nBestilltReproduksjonAvSvFraForrigeIter 
				<<"\t( av mem.:  " <<ulSynapticVesicles_i_membran <<" )\n";

			if( nGjenvaerendePlassTilSV > 0 ){
				// plass til meir SV.
			}else{
				// fullt for SV i presyn.
				
				//if( nBestilltSynteseAvSVFraForrigeIter>1 ) nBestilltSynteseAvSVFraForrigeIter += nGjenvaerendePlassTilSV * SYNTESE_FART_av_SV;
				//nBestilltSynteseAvSVFraForrigeIter *= 0.8; 
				
				nBestilltReproduksjonAvSvFraForrigeIter += /* XXX-> */   0.2    *   nGjenvaerendePlassTilSV; //denne er negativ. 
															// Fører til mindre syntese pga SV-overskudd
				

 				// M.E.P.P. ? :
				//mergeSynapticVesicles( -nGjenvaerendePlassTilSV / 100 );
				cout<<"Overskudd av SV. No burde vi kanskje også hatt MEPP.\n";
			     	//nGjenvaerendePlassTilSV = 0; ikkje nødvendig..
			}








			//XXX Stabilisering mot likevektspkt. for antall S.V. :

#define FART_PAA_FYLLING_AV_SV 0.10    // 10% 		
			nBestiltRefillAvTommeSV = FART_PAA_FYLLING_AV_SV * nSynapticSkall_tommeSV;
		// her er sikkert andre faktore også. Som antall neurotransmittore i plasmaen, osv.. Forenkler.

//*********************************************************************************************************************************************************
			
	


			// XXX XXX XXX XXX XXX XXX XXX
#define GRENSE_MEM_PRODUKSJON 0.10 
			if( (unsigned)nBestilltReproduksjonAvSvFraForrigeIter > ( GRENSE_MEM_PRODUKSJON  * ulSynapticVesicles_i_membran) ){
				//Denne er feil: Bør heller redusere med prosenter av overproduksjonen..
#define MINKINGS_PROSENT 0.05
				// trekker fra % av overskuddet over GRENSE_MEM_PRODUKSJON*ulSynapticVesicles_i_membran
				nBestilltReproduksjonAvSvFraForrigeIter -= MINKINGS_PROSENT * (nBestilltReproduksjonAvSvFraForrigeIter 
														- ( GRENSE_MEM_PRODUKSJON*(signed)ulSynapticVesicles_i_membran) 	 );
				cout<<"\n\n\n\n\n\n\n\nrepro, mem : " <<nBestilltReproduksjonAvSvFraForrigeIter <<" - " <<ulSynapticVesicles_i_membran <<endl;
			}
			


			//     - %-vis tap av frie S.V. (ta bort de gamle) ( i biosys. , er det kanskje %vis tap - etter tidsforsinkelse, men dette er vanskelig.?)

			// => tar vekk 1 % av S.V.: XXX XXX XXX XXX XXX XXX XXX XXX Viktig effekt! Ta med ZXX XXXX XXX XXX XXX XXX
			// Størrelsen på %vis forvitring skal kanskje også variere med farta på produksjonen, og om det kommer tilbakemld. fra postsyn. om fyring.
			// (dette fører til at det tar lenger til får SV som blir produsert da, forvitrer)
			// DETTE KAN VÆRE VELDIG VIKTIG ÅRSAK TIL OVERSVING, med etterfølgande minking i antall S.V. XXX
			//ulAntallSynapticVesiclesAtt 	*= 0.99;
			//nSynapticSkall_tommeSV 	*= 0.99;
	
	




			//  TESTER om reproduskjon og refil av tomme SV, ikkje går over antall: (QUICKFIX)
			if(nSynapticSkall_tommeSV < 0){ nSynapticSkall_tommeSV = 0; cout<<"HOI HOI\n\nMINDREN NULL TOMME SV IGJEN!\n\n\n"; }







			utskriftsFilLogg 	<<"\t" <<ulTidsiterasjoner 	    		<<"\t" 
						<<ulAntallSynapticVesiclesAtt    <<"\t"    <<nSynapticSkall_tommeSV	<<"\t" <<ulSynapticVesicles_i_membran  <<";\n";
			utskriftsFilLogg.flush();

			return 1;
		}
			  
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





    #define SYNAPSE_H
#endif
