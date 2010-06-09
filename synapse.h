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





/*

	// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
	//
	// 	RYDD OPP I DETTE! Det er alt for mykje sosing med S.V. i synapse. Greit nok med testing for S-V-fabrikk, men funker dårlig for computational.Rydd
	//
	// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 






#define DEF_FORHOLD_FRI_VS_MEM_SV 0.2





#define cDebug std::cerr


/ ***************** mykje bås her, no : ******************* /

//#define DEF_OPPLADINGSFART_FOR_SYN_VESICLES 	DEF_startANTALL_SYN_V*0.01
#define DEF_OVERFOERINGSFAKTOR_FOR_SYN   	1
#define DEF_startANTALL_SYN_V 		    	1000
#define DEF_OPPLADING_AV_S_V_DIVISOR 		35
#define MAKS_OPPLADNINGSFART_FOR_S_V 		1000

#define LTP_hastighet_endringAvAntall_POSTSYN_RECEPTOR  0.05 // Ved LTP...
#define LTD_hastighet_endringAvAntall_POSTSYN_RECEPTOR  0.07 // Ved LTD...

#define LTP_INITIALISERINGS_TERSKEL 			750 
*/
#define DEF_startANTALL_SYN_V 	    	1000
#define OPPDATERINGS_GRENSE_SYN_V 		1//0.0001*DEF_startANTALL_SYN_V 	//Grense får når syn. skal legges i pNesteSynapseSomIkkjeErFerdigOppdatert_Koe 
#define MAX_NEG_SYNTESE_AV_SV 		-3
#define MAX_SYNTESE_AV_SV 			8

// klasse-deklarasjoner:
class synapse;
class synapse_likevekt;
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
  
  #include "neuron.h"



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

		unsigned uAntallSynapticV_sluppet; 
		// for systemtreighet. ???
		//Eller finn først ut om dette er veien å gå..

		int nBestilltSynteseAvSVFraForrigeIter;
		int nBestilltReproduksjonAvSvFraForrigeIter;

		std::ofstream utskriftsFilLogg;
		std::ofstream PSP_loggFil;







		/* ******************************************************************************************************
		 * ********                                                                                    **********
		 * ********   void longTimeSynPlasticity_VektEndring( int )                                **********
		 * ********        - Arg: depolarisering for postsyn. neuron.                                  **********
		 * ********        Kalles ved overføring i synapse. Alternativ syn.plasticity er når syn. har  **********
		 * ********          vore inaktiv ei stund => LTD.                                             **********
		 * ********          Dette kan forresten føres inn her også. Eller sjekkes ved overføring...   **********
		 * ********        Bør kanskje også ha en postsyn. effekt ved heteroLTD.                       **********
		 * ********                                                                                    **********
		 * ********        Argumentet deles på TERSKEL_DEFAULT, og dette gir grad av LTP/LTD.          **********
		 * ********                                                                                    **********
		 * ******************************************************************************************************/





		//XXX Skal være avhengig av DA-mengde i postsyn.(?) neuron. Se XXXX... .cpp
		void longTimeSynPlasticity_VektEndring( int nPostsynDepolArg ){ 	// Presyn. vektendring i synapse. For eksempel ved 
														// Endring av setpkt. for antall SV,  eller setpkt. for mem.A.
													//  - Gjelder både auke og minke i vekt 
	
	
			/* 
			 * 	- argument gir om det er vektauke eller vektminke, og størrelsen.
			 * 	  Argument kommer som regel fra returverdi fra sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal(), eller kor depol postsyn. 
			 * 	  neuron er.
			 *
			 * 	- homoLTD skjer når synapsen får returverdi fra sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal(), om kor depolarisert
			 * 	  postsyn. neuron er. Dette sendes videre hit. ( longTimeSynPlasticity( argument ). argument er returverdi fra sendInnPostsy...()
			 * 
			 * 	- LTP skjer med bakgrunn i samme effekt som homoLTD. Dersom postsyn. neuron er sterkt depolarisert, vil LTP initieres (vektauke)
			 * 	
			 * 	- heteroLTD skjer 
			 * 		- når postsyn fyrer, sjekkes timestamp til alle insynapsene, og en verdi blir laga med basis i tidsavviket. Denne verdien
			 * 		sendes inn hit, som argument.
			 * 		ELLER (eller kanskje begge?)
			 * 		- gradvis, når en synapse ikkje er i bruk ? Dette kan ordnes ved at kvar gang en synapse skal brukes, sjekkes timestamp, og
			 * 		dersom det er lenge siden, skal litt LTD påføres.
			 * 	
			 *
			 * 
			 * Spørsmål er om det skal påføres umiddelbart, eller om vi skal vente til tidsiterasjonen er ferdig.
			 * Kanskje ikkje. Dersom eit element er tidligare, fyrte det også tidligare.
			 *  	Tidlige element, er også tidligare utført, nærmare eit sannt asynkront sys...
			 *
			 * STDP - er en direkte følge av at eg bruker grad av depolarisering til å avgjør om vektauke/minke og grad.
			 */

			// Etabler en standard for argumentet. F.eks. 0-100, eller 0-???. Ved -1, fyrer postsyn. (maks LTP)
			// Den går mellom 0 og TERSKEL_DEFAULT. Har også definert en define med HALV_TERSKEL_DEFAULT. 

	/*
	 *
	 * Kanksje eg bare skal lage en faktor som er ca lik 1.
	 * Denne faktor ganges med antall SV:
	 * 	ulAntallSynV_setpunkt *= faktor;
	 * Faktor varierer med grad av depolarisering i postsyn. neuron etter overføring i synapsen.
	 *  	- dersom liten depolarisering: minker vekt litt. Faktor er under 1.
	 *  	- dersom stor  depolarisering: øker faktor litt. Vekta øker da ulineært i forhold til grad av depolarisering.
	 *
	 * */
			int nTempFaktor;
#define DEF_DIVIDENT_VEKTENDRING_FAKTOR 5
//  4 => faktor mellom [0.875  1.125] 
//  5 => faktor mellom [0.9    1.1  ]  		KAN GJØRES MINDRE MED HØGARE FAKTOR.





			/*
			 *
			 *
			 *
			 *
			 * 	I I I I                 I I I I I I I I I I I I I I I I I             I I I I I I I I I 
			 * 	I I I I                 I I I I I I I I I I I I I I I I I             I I I I I I I I I I
			 * 	I I I I                 I I I I I I I I I I I I I I I I I             I I I I I I I I I I
			 * 	I I I I  					I I I I                           I I I         I I I 
			 * 	I I I I  					I I I I                           I I I         I I I 
			 * 	I I I I  					I I I I                           I I I         I I I 
			 * 	I I I I  					I I I I                           I I I I I I I I I I 
			 * 	I I I I  					I I I I                           I I I I I I I I I I 
			 * 	I I I I  					I I I I                           I I I I I I I I I  
			 * 	I I I I  					I I I I                           I I I I
			 * 	I I I I  					I I I I                           I I I I
			 * 	I I I I  					I I I I                           I I I I 
			 * 	I I I I I I I I I                   I I I I                           I I I I 
			 * 	I I I I I I I I I                   I I I I                           I I I I 
			 * 	I I I I I I I I I                   I I I I                           I I I I 
			 *
			 *
			 *
			 *
			 * 	Dynamisk LTP (og dermed synaptisk vekt / minke) er styrt herifra, siden vi har LTD også, kvar overføring (pluss tidsfratrekk av vekt)
			 * 		- LTD er foreløpig statisk, men det er mulig dette er styrt i biologiske system. Mulig FORENKLING: Statisk LTD!
			 * 			-> også mulig det varierer med depol.verdien. Ved høg depol., vil lik del av det forsvinne, m.a.o. større netto verdi-tap
			 * 	
			 * 	Denne skal styrest ved at vi ganger postsyn. depolarisering med en faktor gitt av mengde dopamin i neuronet, til LTP-argument.
			 *
			 * 	DA-mengde kan tenkes på som eit kar med dopamin, som blir fyllt fra eit DA-neuron med tonisk egenfyring. Denne fyller på DA-mengden
			 * 	  i neuronet. Ved pause i DA-leveransen, vil reservoiret tømmes, og for ei stund vil LTP minke, og vi får netto vektminke i in/output
			 * 	(DA påvirker vekt i inn-syn. OG ut-syn.? Dette vil endre når det fyrer, men også kven det fyrer til.. Tenk meir på dette)
			 *
			 * 	XXX XXX XXX Kor skal vi sjå på mengde DA?  DA-mengde i presyn., eller postsyn. neuron?
			 *
			 * 	TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
			 * 	TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
			 * 	TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
			 * 	TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
			 * 	TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
			 * 	TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
			 * 	TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
			 *
			 */
			if( nPostsynDepolArg == -1 ){
	 			// Maks LTP
	 		 	nTempFaktor = 	( HALV_TERSKEL_DEFAULT 				/ (DEF_DIVIDENT_VEKTENDRING_FAKTOR*TERSKEL_DEFAULT)) +1; 
										//Samme som absolutt maks, lenger ned..
				// Kommer etterkvart:
				///* Postsyn.:*/ 	fGlutamatReceptoreIPostsynMem *= ...
			}else{
				nTempFaktor = 	((nPostsynDepolArg-HALV_TERSKEL_DEFAULT) / (DEF_DIVIDENT_VEKTENDRING_FAKTOR*TERSKEL_DEFAULT) ) +1; 
										// dersom arg = HALV_TERSKEL_DEFAULT -> faktor=1
			}

			// nTempFaktor er no mellom (ved DEF_DIVIDENT_VEKTENDRING_FAKTOR  5)  :   0.9 og 1.1.
			ulAntallSynV_setpunkt *= nTempFaktor;
			
 			// Denne er > 1 dersom nPostsynDepolArg > HALV_TERSKEL_DEFAULT, og under ellers. Kan droppe if-setning..





			// Skal eg vidare dele det opp i :
				// Presyn. effekt: 			**************
			
				// Postsyn. effekt: 			**************


			// XXX ANGÅENDE VEKTENDRING:
			// Veit ikkje korleis, men trur at effekta er at 
			// 					- antall synaptic vesicles øker?
			// 					- presyn. membran-areal øker
			// 					- receptorer i posts. øker (veit at responsen til en bestemt mengde n.t. øker..)

		}

			// Finn ut om vektendring er omvendt proposjonalt avhengig av tidsintervallet.
	














/* 
 *
 *
 *  			FUNKER, MEN VIL GJØRE BEDRE! :
 *
 *
 *
 *
 */
		// Skal dette være konst., eller skal presyn. effekt variere med kor mange SV som er merga med mem.? (i såfall fra mergeSynapticVesicles()
		// Dette kan en legge inn ved eit argument, seinare TODO
		void homoLTD( int nArgAntallSV_i_mem =0 )
		{ 	
			if( !nArgAntallSV_i_mem ){ cout<<"Feil. l136 synapse.h\n"; exit(0); } //nArgAntallSV_i_mem er lik "default presyn LTD";

	 		// Minker med vektene med litt litt litt.
			// TODO Foreløpig er det konstant endring ved LTD. Gjør det til at det øker men påfølgende LTD ?
			//if( uGlutamatReceptoreIPostsynMem == 0) døøø Fjærn synapsen, om den er forr lita vekt på.


#define ANTALL_SV_til_LTD_faktor (1/100000)
//			200 -> * 0.995 => (1-500/100.000)



	 		// Minker vektene litt. (Skal kalles ved kvar signaloverføring. Evt. kalles også LTP() om postsyn fyrer)

		// To aspekt: presyn.(S.V.) og postsyn.(antall receptorer).
#define LTD_HASTIGHET_PRESYN_FAKTOR 0.995 
			// Presyn. :
			ulAntallSynV_setpunkt *= LTD_HASTIGHET_PRESYN_FAKTOR; // TODO gjør om slik at argument kan takast inn. Sjå ANTALL_SV_til_LTD_faktor over.

#define LTD_HASTIGHET_POSTSYN_FAKTOR 0.99
			// Postsyn. :
			fGlutamatReceptoreIPostsynMem *= LTD_HASTIGHET_POSTSYN_FAKTOR;

		}




		/****************************************************************************
		 *
		 *
		 *
		 *                  BESTEM OM EG SKAL OPPDATERE ALLE KVAR ITERASJON, ELLER OM EG SKAL OPPDATERE SOM I NESTE FUNK
		 *                       (oppdatere alle kvar iterasjon, eller oppdatere på pow-måten når den skal sende signal. Dette er ei utfordring,
		 *                       men vil gå raskere å simulere i datamaskina..)
		 *
		 *
		 *
		 *
		 * XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX 
		 * XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX 
		 *
		 *
		 * 				DOD TODO TODO
		 *
		 *
		 * ********************************************************************************/
		// Dersom det er lenge siden sist, skal LTD initialiseres.
		/*void LTD( unsigned int tidSidenSistSignallering ){
		
			ulAntallSynV_setpunkt *= pow( 0.9997, tidSidenSistSignallering );
		}*/



		unsigned long ulTimestampForrigeOppdatering; /// Hovedsaklig for å unngå å oppdatere fleire ganger kvar iterasjon.. XXX Nødvendig? ZX XXX

		unsigned long ulTimestampForrigeSignal;

		/*********** For synaptic short-term depression: ***********/
		
		// Desse er vedtatt som variable i neurolog-milø. (FAKTA):
		unsigned long ulAntallSynapticVesiclesAtt; 		// -For synaptic (short term) depression. Gjør også slepp av s.v. prosentvis
	      unsigned long ulAntallSynapticVesiclesAtt_forrige; 	// - for derivasjon. 					//  (som eit diff-lign. sys.)
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


		const int getKorMangeSVsleppesNo()
		{ //{ ... }
			// sluppet trengs ikkje. Kan for så vidt ha lokal variabel her. Initielt var tanken å ha en liten treighet her. type MA..
			/*uAntallSynapticV_sluppet*/
			return  (0.1 * ulAntallSynapticVesiclesAtt); /// TODO TODO TODO sjå under.
				//  | f.eks. *0.07 TODO Dette skal være en variabel, som kan også endres ved LTP (f.eks. membranareal kan øke).
				//  VARIERER kanskje MED MEM. AREAL!
		    /*uAntallSynapticV_sluppet = 0.2 * ulAntallSynapticVesiclesAtt; /// TODO TODO TODO sjå under.*/
		} //}

	
		// oppdaterSyn() :
		// 	arg: 	void
		// 	retur: 	Returnerer 1 ved suksess. I arva klasser brukes det til annet (f.eks. i synSkilleElement betyr -47 retur at 
		// 		  den er kalt i eit synSkilleElement. Dette har muliggjør : while( oppdateringskø.front()->oppdaterSyn() );
		// Oppdaterer neuron for ny klokkeiterasjon: (lader opp igjen synaptic vesicles i synapse)

		virtual int oppdaterSyn()
		{ //{ ... }
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
			
			// for derivasjon: dAntall = noVerdi - forrigeVerdi
			ulAntallSynapticVesiclesAtt_forrige = ulAntallSynapticVesiclesAtt;

			// SYNTESE:
			ulAntallSynapticVesiclesAtt += nBestilltSynteseAvSVFraForrigeIter;
			// (dersom den er negativ, skal denne også sørge for å ta vekk litt syn. v.(sjå slutt av denne funk) )

			// REPRODUKSJON:
			// reproduksjon av S.V.
			ulAntallSynapticVesiclesAtt  += nBestilltReproduksjonAvSvFraForrigeIter;
			ulSynapticVesicles_i_membran -= nBestilltReproduksjonAvSvFraForrigeIter;

// Utskrift:
//cout<<"Repro: " <<nBestilltReproduksjonAvSvFraForrigeIter <<" synth. " <<nBestilltSynteseAvSVFraForrigeIter <<"\t";
			
	// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
	// DENNE FUNKER IKKJE. FIX!	
		if( 		//(ulSynapticVesicles_i_membran < OPPDATERINGS_GRENSE_SYN_V)  	&& 
			     	( abs(ulAntallSynapticVesiclesAtt-ulAntallSynV_setpunkt) < 3*OPPDATERINGS_GRENSE_SYN_V ) &&
			     	( abs(ulAntallSynapticVesiclesAtt-ulAntallSynapticVesiclesAtt_forrige) < 0.5*OPPDATERINGS_GRENSE_SYN_V +0.5 ) ){ //derivert liten.
	 			// nullstill avviket.
				cout<<"\n\n\t\tFERDIG\n\n";
				//sleep(1);
				ulAntallSynapticVesiclesAtt = 		ulAntallSynV_setpunkt;
				ulAntallSynapticVesiclesAtt_forrige = 	ulAntallSynV_setpunkt;
				ulSynapticVesicles_i_membran = 0; /// ?? XXX Kanskje dette skal vekk, etterkvart.

			}else{
	 			pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.push_back( this );
			}

			
			// Oppdaterer timestamp for oppdatering av syn.
			ulTimestampForrigeOppdatering = ulTidsiterasjoner;
			
			// Neste iterasjons syntese av S.V. : 	( Lita MA-effekt for å smoothe det ut litt.)
#define MA_EFFEKT_SYNTESE_SV_FILTERSTOERRELSE 2
			// XXX XXX XXX MA-effekt fører til at syntese aldri blir 0 igjen, etter å vore meir enn null.

  //			nBestilltSynteseAvSVFraForrigeIter = ( (MA_EFFEKT_SYNTESE_SV_FILTERSTOERRELSE-1) * nBestilltSynteseAvSVFraForrigeIter
  //									+ ((signed)ulAntallSynV_setpunkt - (signed)ulAntallSynapticVesiclesAtt ) +0.5 ) ;
  //			nBestilltSynteseAvSVFraForrigeIter /= MA_EFFEKT_SYNTESE_SV_FILTERSTOERRELSE; //asdfasdf

			// Blir faktisk en del bedre med float:
			float tempTestFloat = ( (MA_EFFEKT_SYNTESE_SV_FILTERSTOERRELSE-1) * nBestilltSynteseAvSVFraForrigeIter
									+ /*faktor?*/0.5* ((signed)ulAntallSynV_setpunkt - (signed)ulAntallSynapticVesiclesAtt ) 
									+0.5 )
							/ MA_EFFEKT_SYNTESE_SV_FILTERSTOERRELSE ;


			// Dersom den er negativ, halver den. Andre effekter som styrer sletting av s.v. ...
			if(nBestilltSynteseAvSVFraForrigeIter>0){
				nBestilltSynteseAvSVFraForrigeIter = tempTestFloat /*+0.5*/;
				
				if(nBestilltSynteseAvSVFraForrigeIter> MAX_SYNTESE_AV_SV ) nBestilltSynteseAvSVFraForrigeIter=MAX_SYNTESE_AV_SV;
			}else{
#define DIVISOR_VED_NEG_SYNTESE 8
			 	nBestilltSynteseAvSVFraForrigeIter = ( tempTestFloat / DIVISOR_VED_NEG_SYNTESE ) -0.5; //runder opp (negativt)

				// setter tak for sletting av s.v.
		 		if(nBestilltSynteseAvSVFraForrigeIter < MAX_NEG_SYNTESE_AV_SV )
					{	nBestilltSynteseAvSVFraForrigeIter = MAX_NEG_SYNTESE_AV_SV; 	}
			}

			// Nester iterasjons reproduksjon av S.V. :
#define REPROFAKTOR_FRA_MEMBRAN_TIL_SV 0.1
			nBestilltReproduksjonAvSvFraForrigeIter = (ulSynapticVesicles_i_membran * REPROFAKTOR_FRA_MEMBRAN_TIL_SV 	 +0.5);

			

			//{ For S.V. - logg:
			utskriftsFilLogg<<"\t" <<ulTidsiterasjoner <<"\t" <<ulAntallSynapticVesiclesAtt <<"\t1000\t" <<"\t" <<ulSynapticVesicles_i_membran <<";\n";
			utskriftsFilLogg.flush();
	
			// PSP - effekt-logg:									//Kor stor er membran?:
			PSP_loggFil<<"\t" <<getKorMangeSVsleppesNo()*fGlutamatReceptoreIPostsynMem*(1+ ulSynapticVesicles_i_membran/1000) 
				<<" ;   \t#[potensiellt antallSV slept] * [receptore i postsyn] * [1+ ulSynapticVesicles_i_membran/1000]\n" ;
			PSP_loggFil.flush();
			//} 
			
			return 1;
		} //}
			

	public:
		// Constructor for arv i synapse. Potensiellt farlig? Legger inn char, og sjekk om det er f.eks. 't'. ellers; feilmld.
		synapse(char c) : bInhibitorisk_effekt(false) { if(c!='t'){ cDebug<<"\n\n\n\nERROR: l 264 i nexxxxxx\n\n\n"; exit(0); } } 
		synapse( neuron* pPreN_arg, neuron* pPostN_arg, bool argInhibitorisk_effekt =false, float v =1 );
		// Denne legger seg automagisk til i postsyn. si innsyn.liste, og presyn. utsyn. liste.

		// Destructor, for å fjærne seg sjølv fra postsyn.inn-syn-liste, og presyn:utsyn-liste.
		~synapse();

		neuron* pPreNode;
		neuron* pPostNode;

		static list<synapse*>  pNesteSynapseSomIkkjeErFerdigOppdatert_Koe;	
		
		


	



	
	
		// trur ikkje eg trenger returverdi:
		virtual void aktiviserOgRegnUt()
		//void synapse::aktiviserOgRegnUt()
		{ //{
			// Implementerer short-term depression ved å legge inn (int dProsentSynapticVesiclesAtt) i neuron-klassa og 
			//    slepping av maksimalt synVesicles er proposjonalt med denne ( antall sluppetSynV < konst * dAntallSyn... )
			//    og lineær opplading av dProsentSynapticVesiclesAtt. Denne lineære oppladinga kan kanskje være en del av 
				// den synaptiske plastisiteten.
			
			/*
			 * HAR Gått ut fra at det er konstant antall neurotransmittore inni syn.vesicles.
			 * 	Dette kan være feilaktig antagelse..
			*/



			/******* Synaptisk depression: ******/ //For å begrense signalet til presyn->maksSignal. Denne er for short-time synaptic depression.
			/*synapse::*/oppdaterSyn();

			/******* LongTimeSynDepression: *****/ // Ved lite bruk, vil heteroLTD initieres.
		//XXX	LTD( ulTidsiterasjoner-ulTimestampForrigeSignal ); // Minker vekt på grunnlag av kor lenge det er siden sist signal.

			// Signaloverføring er avhengig av antall receptorer for aktuelle neurotransmittor. Ganger med en variabel
			// 	definert for synapse, og som kan variere med LTP/LTD.

			// XXX TA VEKK uAntallSynapticV_sluppet ??? (og inn som argument)
			uAntallSynapticV_sluppet = getKorMangeSVsleppesNo();
			mergeSynapticVesicles(); // tar int-argument. Alternativt leser den ut fra klassevariabelen: u AntallSynapticV_sluppet (satt litt lenger opp.)
			
			// Legger til synapse i pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.
			synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.push_back( this );
			

			/******* oppdaterer timestamp for tidspkt for signal ********/
			ulTimestampForrigeSignal = ulTidsiterasjoner;
		} //}	
		
		
		friend std::ostream & operator<< (std::ostream & ut, synapse );
		friend std::ostream & operator<< (std::ostream & ut, neuron );
		friend int initArbeidskoer();
		friend class synSkilleElement;

		// For å gi postsyn. neuron mulighet til å initiere LTP/LTD: (protected funksjoner)
		friend class neuron;

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
		int oppdaterSyn(){ 
			// kan være lurt å for sikkerhetsskuld ta med if(pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front() == this )  - Men treigare..
			if( synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front() != this ){ cout<<"FEIL synapse.h_52"; exit(-1); }
			/* IKKJE LØYSING: feilsjekk..  */

			// legg seg til på slutt:
			synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.push_back(this);   	
			// Fjærner dette element fra oppdateringsliste:
			synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.pop_front();   

			return 0; // Signaller tilbake att dette ikkje er vanlig synapse::oppdaterSyn() (viktig for funksjonallitet til programmet)
		}
	       	/* return 0 er heile poenget med int-returverdi her. Skal ret. 0 ved synSkilleElement, og 1 ved vanlig ok
		 * 	Dersom det er vanlig oppdatering av vanlig synapse, vil den returnere 1, her returnerer den 0, Dette gjør at eg kan kjøre
		 * 	while( ! (kø++)->oppdaterSyn() );  // trur ikkje noke skal gjøres. Kanskje utskrift eller noke..
		 */

};





    #define SYNAPSE_H
#endif
