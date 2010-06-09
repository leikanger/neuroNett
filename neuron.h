
//KANN
//{ Kladd:
/*
	Neste store utfordring er korleis gro eit neuralt nettverk, uten å fotfølge algoritmene?
	- En mulighet er å legge inn sted-koordinater i neuron, ELLER på annen måte dele de opp i gjupper (for kven som kan gro kor). Litt for detaljstyrende
	- lage eit grovt (ikkje detaljstyrende) overordna system, som bestemmer kva som skal gro med kven.
	- Kanskje prøve meg på eit system med atraktorer. Kan godt være veldig regnekrevande, siden det ikkje skal gro mens det kjører..
	 	(det kan godt stå og gro for seg sjølv, (offline) ei natt.. ) - derretter kommer normal læring inn...
	- online-endring gjøres kanskje med enkelte neuropeptid?
	
	- les korleis dette gjøres i kroppen.



*/
//}

//  Skal eg ha pre- og post- syn. læringseffekt? Eller er dette bare interresant vitenskapelig (årsak bak LTP).
//{ LÆRING:
/*******************************************************************************************************
 ********************   PLAN, LÆRING:                                                *******************
 ********************                                                                *******************
 ********************       - LTP :                                                  *******************
 ********************           - øker synapsevekt.                                  *******************
 ********************           - når skjer dette? Undersøk / tenk !                 *******************
 ********************           => Skal skje når postsyn. neuron fyrer               *******************
 ********************               ( ved å sjekke alle innsynapsers timestamp )     *******************
 ********************                                                                *******************
 ********************       - HomoLTD :                                              *******************
 ********************           ( Synapsen fyrer masse, men postsyn. er stille )     *******************
 ********************           - Dette kan overføres til tidsplanet med å sei at    *******************
 ********************               ved syn.sig. og ingen postsyn. sig. så           *******************
 ********************               litt litt homoLTD (minker synapsevekt).          *******************      
 ********************               Dette er info som kommer som retursignal postsyn.*******************
 ********************               Kanskje eg også skal ta med effekt av sterk      *******************
 ********************               depolarisering, ikkje bare ved fyring(i returv.) *******************
 ********************           - Dersom postsyn. depol, så litt øking (LTP over)    *******************
 ********************               HomoLTD og LTP kan dermed samles i en funksjon.  *******************
 ********************                                                                *******************
 ********************                                                                *******************
 ********************       - HeteroLTD:                                             *******************
 ********************           ( synapsen er stille, postsyn. neuron fyrer )        *******************
 ********************           - sjekkes fra neuron. Når neuronet fyrer, skal den   *******************
 ********************               sjekke om de som synapser inn til den har hatt   *******************
 ********************               signal innen ei tidsgrense. Dersom det er over   *******************
 ********************               grensa, skal LTD skje. Finn ut om det skal       *******************
 ********************               graderes. Litt litt heteroLTD..                  *******************
 ********************                                                                *******************
 ********************       - Celledød????                                           *******************
 ********************                                                                *******************
*******************************************************************************************************/

/*******************************************************************************************************
 ********************       Kanskje antall receptore / [postsyn. effekt av n.t.]     *******************
 ********************         er konstant? At det bare flyter rundt mellom synapsene.*******************
 ********************         Men kan endres ved høg/lav fyringsfrekvens.            *******************
 Med det meiner eg høgare/lavare enn en gitt egenfrekvens, så kan denne egensfrekvensen gradvis endres, og i tillegg at egenfrekvensen er med på å 
 bestemme "konstanten" (antall receptore) ved å være proposjonal med 1/E.frekvens. Når vi har høg fyringsfrekvens, vil E.frekvens øke, og dermed konst.
 antall receptore minke. Dette vil føre til stabilisering av synaptiske vekter. Nydelig. I tillegg må du huske at LTP/D er utforska for enkeltsynapser,
 og ikkje neuron som heilhet. Denne flytende-men-konstant-globalt antalle receptore (postsyn. styrke) fører til at vi får effekter som passer godt, eller
 vil føre til resultata for LTP og LTD.
 ******************************************************************************************************/

/* * * * * Med dette blir plan for læring: * * * * * * * 
 *   - Variabel for antall glutamatreceptore-totalt i postsynaptisk neuron.
 *    	- denne kan økes/minkes av frekvensen. Avhengig av om den er over eller under egenfrekvensen. Kanskje det er dette som skal være variabelen.
 *   - Kvar synapse skal ha en variabel som utgjør kor mange promille av de totale glutamatreceptorane, syn. skal ha.
 *   - ved oppdatering, eller ved bruk (avhengig av kor regnetungt systemet er) skal synaptisk vekt kalkuleres.
 *   Dette fører til at synapsene kun trenger å ta hensyn til positiv LTP-effekt. (LTD skjer som resultat). 
 *   Og læring kan skje på to nivå; synapsen kan øke i styrke, og neuron kan øke i frekvens. 
 *
 *   -Begynner med statisk E.frekvens.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * *                        KODE STARTER :                           * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <iostream>
#include <iomanip>
#include <stdlib.h> 	// for exit()
#include <list> 	// for list
#include <vector>
#include <map>    // map


using std::list;
using std::cout;
using std::endl;




#define cDebug std::cerr

// TODO gjør denne mykje mindre. En tidsiterasjon skal være veldig lite, (mykje mindre) og skal dermed føre til mindre degradering av verdi.


#define REFRACTION_TIME_NEURON 2 //eller kanskje heller 1?

#define DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON 0.96 // for eksempel..         (ganger med denne faktoren per klokketikk) 

#define TERSKEL_DEFAULT 999 
#define HALV_TERSKEL_DEFAULT TERSKEL_DEFAULT/2

#define DEF_MAKS_TID_MELLOM_OPPDATERINGER_FOR_KVART_NEURON 20 // Dette er en sikkerhet, som er bra å ha i innfasinga av estimert-fyring-optimaliseringa.
										// -- redundans!
										// Kanskje mulig å optimalisere bort etterkvart.
										// (sjå opt240@neuron.h)

// klasse-deklarasjoner:
class synapse; 		 	// grunnklasse for synapse. Kan brukes som den er.
class synapse_likevekt; 	// classe for å teste teori om likevekt, S.V. som (delvis) bakgrunn for syn.plasticitet, osv.
class neuron; 			// Hovedklasse for funksjonaliteten til neuron. Temporal leaky integrator, fyring, inneholder synapse-vektor.
class neuroSensor; 		// NeuroSensor for å oversette tilstand til neuro-språk.
class arbeidsHistorieElement; // Test for å ha en form for logg. Ikkje kommet så langt. Ta bort?

// globale variabelDeklarasjoner:
extern unsigned long ulTidsiterasjoner;
//extern list<synapse*> 		    pNesteSynapseUtregningsKoe; Flytta inn i synapse-klassa som static
extern list<arbeidsHistorieElement*> pArbeidsHistorieListe; // ikkje noke serlig i bruk. Kan ta vekk.

extern list<neuroSensor*> pNeuroSensorListe; // extern for at den kan deklareres her. //Kan puttes inn som static i klassa.


#ifndef NEUROENHET_h
  #define NEUROENHET_h
 
  #include "synapse.h"
  #include "synapse_likevekt.h"




/*
 * Ny plan for neuron: 
 * 	- oppdaterNeuron():
 * 		Skal oppdatere div. variabler som er tidsvariante. For å opitmalisere, dropper eg å oppdatere kvar iterasjon, men istaden lager eg timestamp ved
 * 		  oppdatering, og ved neste oppdatering tas differansen. Denne brukes for å regne ut endring i de ulike variablane fra forrige oppdatering.
 * 			Aktuelle variabler (så langt):
 * 			- "dopamin". - brukes til å variere grad av LTP (og som resultat LTD pga "statisk" LTD som varierer med syn.styrke.
 * 			- depolarisasjon. - endres kontinuerlig (meir depolarisasjon). Dette fører til tonisk egenfyring av neuron.
 * 				-> ~20Hz spontanfyring (i nokre neuron. Varierer i noken, statisk i andre. Lite/mykje, var/statisk frekvens, osv.
 * 	 oppdaterNeuron() skal kjøres kvar gang neuronet får DA-input og eksitatorisk/inhibitorisk input.
 * 	 	- skal holde styr på når neuronet kommer til å fyre, ved noværande rate for depolarisering, og skrive dette i fyrings-tabellen 
 * 	 		(i static map sNesteFyringForNeuro n).
 *
 * TODO START.
 * TODO Neste er å implementere den eksekutive delen av arbeids-kø: sNesteFyringForNeuro n skal føre til at neuronet fyrer på den tida.. TODO
 * 	 -> i tidsskillelement
 * 	 Eller forresten:heller at oppdaterNeuron() blir kalla, runden før (for å være sikker) og evt. lagt til pNesteSynapseUtregningsKoe til neste runden igjen
 * 	 	 (etter tidsIterasjon) for å få det rett..
 *
 * 	 - så først: få køa til å funke. (legge til ny, og oppdatere gammel) 	-- skal gjøres alle plassane depol. for neuronet blir endra.
 * 	 - Så få synSkilleElement->oppdaterNeuron() til å finne alle som skal aktiviseres ved neste tidssteg, og kjør oppdaterNeuron() på desse neurona.
 * 	 	-> dette kan føre til at de kommer over fyringsterskelen, og fyrer. Dette vil i så fall skje på vanlig måte.
 * 	 		- eventuellt vil nytt estimat for fyring, og oppdatering. Vil så legges til der. Dette bør bemerkes, siden det ikkje optimalisert bør skje
 * 	 	- Dersom den kommer over fyringsterskelen, vil den legges til på slutten av pNesteSynapseUtregningsKoe.
 * TODO SLUTT.
 */





/*******************************************************************************************************
 ********************    class neuron                                                *******************
 ********************        - Hovedklasse for Auron. Innholder alt, med unntak      *******************
 ********************          av synapse-funksjonaliteten.                          *******************
 ******************************************************************************************************/

class neuron {
	private:
		// TODO oppdaterNeuron() kjører bare i enkelte tilfeller. Neste er for å holde styr på systemet uten å oppdatere heile tida:
		// TODO FLYTT NESTE OPP IGJEN FRA public!
		//static std::map<neuron*, int> sNesteFyringForNeuron; 

	protected:
		int nVerdiForDepolarisering;
		int nVerdiForDepolariseringEndringsrate;

		unsigned long ulTimestampForrigeVerdiendring_neuronDepol;
		unsigned long ulTimestampFyring;
		unsigned long ulTimestampForrigeOppdatering; // Oppdatering av: DA(for LTP-storleik), depol., osv.

		double dMengde_DA_i_neuron; 	//(for LTP (postsyn.). Størrelsen er avhengig av mengden DA i neuronet..)
					/*  		Skal variere rundt 100.0 (double)
					 *   			Skal endres ved LTP, og i this.oppdaterNeuron();
					 */
		
		// XXX
		// Skal brukes for neuron::oppdaterNeuron(), slik at den har mulighet for å legge til fyring av neuron i arbeidskø (som er synapse).
		// 	- egen synapse for dette, for å hindre LTP av andre innsynapser, hindre tidsbruken nye synapser medfører.
		//synapse synHandtakForArbeidKoe;

		std::string navn;


		virtual void kalkulerDegraderingAvVerdiPaaGrunnlagAvTimestampSidenSist( )
		{
		 	unsigned long ulKlokketikkSidenSisteInput =   ulTidsiterasjoner - ulTimestampForrigeVerdiendring_neuronDepol;
		
			double tempFaktor = 1;
			// optimalisering_ : gjør fra neste linje til : pow( DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON, ulKlokketikkSidenSisteInput, 
			for(unsigned int i=0; i < ulKlokketikkSidenSisteInput; i++ ){ 	tempFaktor *= DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON;   }
 			// TODO endre til tempFaktor *= DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON ...	optimalisering..
			// merk *= som betyr at det er lik seg sjølv ganger tempFaktor
			nVerdiForDepolarisering *= tempFaktor;  
		
			//cout <<"Tid siden sist:   " <<ulKlokketikkSidenSisteInput <<" (tid no:" <<ulTidsiterasjoner <<")\t\n";
		
		}


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
		 * * * 
		 * * *  	virtual int sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( int nInnsignalArg )
		 * * * 		- Arg: 	innsignal. foreløpig altid lik synaptisk vekt.
		 * * *  		- Return: 	Depolarisasjon for postsyn. neuron. 
		 * 					- Kan kanskje være negativ for veldig lite depol. (=>LTD). Ikkje implementert enda.
		 * 					(-1) ved fyring av postsyn. neuron. Dette for å gi signal til presyn. for LTP.
		 * * * 
		 * * *
		 * * *
		 * * *
		 * * * 
		 * * *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		virtual int sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( int nInnsignalArg )
		{
			if( ulTidsiterasjoner -  ulTimestampFyring > REFRACTION_TIME_NEURON ){ cout<<"Hindra gjentatt fyring.\n"; sleep(1); return -9; }
			//int foerTidSig = nVerdiForDepolarisering; 		// for utskrift.
			kalkulerDegraderingAvVerdiPaaGrunnlagAvTimestampSidenSist();
			//int etterTidSig = nVerdiForDepolarisering; 	 	// for utskrift.
			
			// Legger til verdi.
			nVerdiForDepolarisering += nInnsignalArg;
			
			/* utskrift:
			//if( ulTidsiterasjoner % 1000 == 0){
			cout<<" Verdi ( før-tid | etter-tid ... ) :\t:\t" ; 	cout.width(8);
			cout<<std::left <<foerTidSig; 	                     cout<<"    |    ";  cout.width(8); cout<<etterTidSig;
			cout<<" \t+\t"; cout.width(8); cout<<nInnsignalArg; cout<<" \t=>\t";  cout.width(8); 	cout<<nVerdiForDepolarisering <<"\n";
			//} */ 

			// tilordner tempklokke til timestamp for verdien for nVerdiForDepolarisering (neuron::timestamp_forrige_input)
			ulTimestampForrigeVerdiendring_neuronDepol = ulTidsiterasjoner;
			
			// ser om det går over terskel:
			int returverdi;
			if( (returverdi = sjekkKorDepolarisertNeuronEr() ) == -1 )
			{
				fyr();
				return returverdi;
			}else{
				return returverdi;
			}
		}
		
		
		// grunnen til overlagring er refraction period. XXX Den kan eg kanskje ta vekk, uansett
		virtual int fyr()
		{
			// Oppdaterer medlemsvariabler. F.eks. DA-nivå, som har innvirkning på synPlasticity.
			oppdaterNeuron();

			// refraction-period for neuron: (ikkje lov å gjøre to fyringer ila. eit sekund.)
			if( ulTimestampFyring == ulTidsiterasjoner ){ 	
				cout<<"Feil?\n\nTo fyringer på eit sekund.  l.211 i neuron.h\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; 
				exit(-9); /* return 0; */ }

				cout 	<<"\n*************************************************************************** * * * ****************************************************************\n* * * * * * * * * ** ** ** **** *******           neuron  " <<navn <<"          ******** ***** *** ** ** * * * * * * * * * * * * * * * * * * * * * *\n**************************************************** * * * *****************************************************************************************\n"; 
					       

			// Resetter nVerdiForDepolarisering, etter fyring:
			nVerdiForDepolarisering = 0;

			
			// denne fyrer: sjekk alle som den synapser til ( legg til i arbeidskø ):
			for( std::vector<synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
			{ 	// legger alle synapsene mine inn i arbeidskø
				synSkilleElement::pNesteSynapseUtregningsKoe.push_back( *iter );
			}	//Legg til alle synapser i neste fifo-kø foR utregning. //OK // går over til bare ei kø, med "tids-synapse"
	
			// TODO Legg også inn litt LTP i kva synapse. TODO
			// Er i gang med å lage engen LTP / homoD funksjon. kall her, dersom det skal skje..
			// vha. tilsvarande opplegg som over. for allt pInnSynapser - kjør ltd-og-ltp funksjon. XXX XXX
			

			// TODO Når eg har laga en autofyringsarray: Legg til i denne etter fyring, etter [autoFyringsPeriode]. 
			// 		( - finn peiker til dette neuronet i array,
			// 		  - endre fyringstidspunkt til no-tid + periode
			// 		)



			// Setter timestamp for fyring:
			ulTimestampFyring = ulTidsiterasjoner;

			// Sender respons om at eg fyrer: TA VEKK returverdi?
			return 1;
		}

		
		/****************************************************************************************
		** 	virtual int sjekkKorDepolarisertNeuronEr() 						
		**		Arg  : 	- 								
		**		Retur: 	int, som seier kor depolarisert neuron er. Brukes bl.a. for LTP/D
		**			Dersom retur er -1, betyr det at neuron fyrer. 			
		**	 									
		** 		Denne funksjonen kalles (bare/bl.a. ?) fra funk sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal() 
		** 		  før den skal sjekke om postsyn fyrer. Brukes når neuron får inn nytt signal, for å:
		**	  		- Sjekke om postsyn. fyrer. 				
		**			- Sjekke om det skjer LTP / homoLTD 		
		****************************************************************************************/
		virtual int sjekkKorDepolarisertNeuronEr( ){ 
		 	// returnerer depolarisertings-verdi. Dersom den er over terskel, returneres -1.

			if( nVerdiForDepolarisering > TERSKEL_DEFAULT  	/*nTerskel*/){

				return -1;
		 	}else
				return nVerdiForDepolarisering;
	
		}









	public:
		//neuron() 		: nVerdiForDepolarisering(0), ulTimestampForrigeVerdiendring_neuronDepol(0), navn("uten navn"),   	terskel(TERSKEL_DEFAULT) { } 
		neuron( std::string n ) : 	nVerdiForDepolarisering(0), 
							nVerdiForDepolariseringEndringsrate(100/*XXX alt for mykje. Tester.. XXX*/),
	     						ulTimestampForrigeVerdiendring_neuronDepol (ulTidsiterasjoner), 
		 					ulTimestampFyring 	(ulTidsiterasjoner),
							ulTimestampForrigeOppdatering(ulTidsiterasjoner),
							navn(n)
 		{ 
			oppdaterNeuron();
		} 

		//destructor:
		~neuron()
		{ 
			//{ Tar vekk synapser (på hi sida og).
			cout<<"~neuron : " <<navn <<"\n";

			synapse* pTempSyn;

			//destruerer alle inn-synapser:
			while( !pInnSynapser.empty() ){
				cout<<pInnSynapser.size() <<" innsynapser igjen til " <<navn <<"(fra " <<pInnSynapser.back()->pPreNode->navn <<").\n";
				
				// XXX OK har funnet meir ut problemet. Følgende er quick-fix. Skal vekk..
				//if( pInnSynapser.back() == pTempSyn ) break;
				// Problemet er at vektoren ikkje registrerer at synapsa er vekk. sjå ~synapse@neuronEnhet.cpp

				// Mellomlagrer peiker til siste synapse.
			 	pTempSyn = pInnSynapser.back(); 
				cout<<"~neuron: fjærner innsyn. fra " <<(*pInnSynapser.back()).pPreNode->navn <<"\ttil " <<navn <<"(meg): \n";
				// Tar vekk siste element i vector.
				pInnSynapser.pop_back(); //-Nei: taes hand om i ~synapse
				
				// ordner det fra hi sida og:
				pTempSyn->pPreNode->taBortOutputSynapse( pTempSyn ); //XXX

				// Sletter synapse som pTempSyn peiker på.
				// for no, driter eg i minnelekkasje, og dropper delete. Lekkasje i frie lager!
			 	//delete( pTempSyn ); // XXX Istadenfor, tar eg den bare vekk fra pInnSynapser-lista
			} //og ut-synapser:
			while( !pUtSynapser.empty() ){
				// Mellomlagrer peiker til siste synapse.
	 			pTempSyn = pUtSynapser.back();
				cout<<"~neuron: fjærner utsyn. fra " <<navn <<"(meg) til " <<(*pUtSynapser.back()).pPostNode->navn <<endl;
				// Tar vekk siste element i vector.
				pUtSynapser.pop_back(); //-Nei: taes hand om i ~synapse
			
				pTempSyn->pPostNode->taBortInputSynapse( pTempSyn );
				// Sletter synapse som pTempSyn peiker på.
				// for no, driter eg i minnelekkasje, og dropper delete. Lekkasje i frie lager!
				//delete( pTempSyn ); // XXX Istadenfor, tar eg den bare vekk fra pUtSynapser-lista.
			}

			//evt andre ting
	 		cout<<"~neuron ferdig for " <<navn <<endl;
			//}
		} 


		// U-testa: (grundig, men den funker)
		void taBortInputSynapse(synapse* syn_p_ArgTaVekk)
		{ 
	 		//fjærner arg fra inputliste: 	//{
			for( std::vector<synapse*>::iterator iter = pInnSynapser.begin(); iter != pInnSynapser.end() ; iter++ ){
				if( *iter == syn_p_ArgTaVekk ){
					cout<<"\ttaBortInputSynapse(syn*): Fjærner input-syn. fra [" <<syn_p_ArgTaVekk->pPreNode->navn <<"] -> " 
										     <<"meg(" <<navn <<")\n";
					(pInnSynapser).erase( iter );
					break;
				}
			} //}
		} 

		void taBortOutputSynapse(synapse* syn_p_ArgTaVekk)
		{ // {
	 		//fjærner arg fra inputliste: //{
			for( std::vector<synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end() ; iter++ ){
				if( *iter == syn_p_ArgTaVekk ){
					cout<<"\ttaBortOutputSynapse(syn*): Fjærner output-syn. fra meg(" <<navn <<") -> [" <<syn_p_ArgTaVekk->pPostNode->navn <<"]\n";
					(pUtSynapser).erase( iter );
					break;
				}
			} //}
		}

	
		// neuron::oppdater: oppdaterer forskjellige mdl.variabler. Basert på sist-oppdatering-timestamp.
		void oppdaterNeuron()
		{ //  {
			//{ Oppdaterer DA, depol., og andre tidsvariante fenomen i neuron.
//#define SVINN_FAKTOR_KVAR_RUNDE 0.98
			
			/* - Sjekk tid siden sist, kjør for-loop over alle iter. siden sist, oppdater for kvar iterasjon. 
			 	regn ut når neste fyring vil være, uten meir eksitatorisk/inhibitorisk input. Skriv dette i tabell sNesteFyringForNeuron.
			*/

			/*
			cout<<"Inne i neuron.oppdaterNeuron(). Sjekker om eg har oppdatert allerede, denne runden:"; cout.flush();
			// Sjekker om allerede har oppdatert denne iterasjonen:
			if(ulTidsiterasjoner == (ulTimestampForrigeOppdatering / *XXX+1* /) ){ cout<<"ja\n"; return; } //+1 pga. strict "mindre enn" i for-loop.
			else cout<<"nope\n";*/
			if(ulTidsiterasjoner > ulTimestampForrigeOppdatering ){

				// Går bak i tid, og itererer over tida, og på kvart hopp, gjør eg alt som skulle vore gjort.
				// 	(maskinvaren klarer bedre slik aktivitet som lettare kan pipelines)..
				while( ulTimestampForrigeOppdatering < ulTidsiterasjoner ){
					// ulTimestampForrigeOppdatering er allerede gjennomført.
				 	ulTimestampForrigeOppdatering++;

					cout<<"(oppdaterNeuron()@neuron.h . ulTimestampForrigeOppdatering: " <<ulTimestampForrigeOppdatering  
						<<" for neuron " <<navn <<" \t\t\tdepol.: " <<nVerdiForDepolarisering <<"\n";

					// Legger til positive effekt: innflyt av verdi.
					nVerdiForDepolarisering += nVerdiForDepolariseringEndringsrate;

					// Trekker fra negativ effekt: lekking av verdi i "leaky integrator". ( Tenk på eit lekk vannrør. )
					nVerdiForDepolarisering *= DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON;

					// oppdater ulTimestampForrigeVerdiendring_neuronDepol
					ulTimestampForrigeVerdiendring_neuronDepol = ulTidsiterasjoner;

					if( nVerdiForDepolarisering > TERSKEL_DEFAULT ){
						cout<<"\n\tEGENFYRER! * * * * * * * * \n";
						fyr();
					}


				// 	Ikkje implementert DA enda..
				//	dMengde_DA_i_neuron *= SVINN_FAKTOR_KVAR_RUNDE; 
				}
				// Her er bedre å (optimalisering:) pow(dMengde_DA_i_neuron, antall iterasjoner);
			}

			// konstant derivert av depol.
			unsigned uTidsDiffTilNesteFyring =  ( ((TERSKEL_DEFAULT - nVerdiForDepolarisering) / nVerdiForDepolariseringEndringsrate) +1.5);
			// +0.5 fordi int blir default runda ned. 	+0.5 runder opp! Legger til litt til, pga. vanligvis er det lenger itgjen. 
			// XxX Potensiell bug!
	
			// Safety, redundans. Antagelig unaudsynt redundans..
			if(uTidsDiffTilNesteFyring > DEF_MAKS_TID_MELLOM_OPPDATERINGER_FOR_KVART_NEURON) 	//opt240@neuron.h
				uTidsDiffTilNesteFyring = DEF_MAKS_TID_MELLOM_OPPDATERINGER_FOR_KVART_NEURON;

			cout<<"\t\t\toppdatering() for neuron " <<navn <<". Neste oppdatering: "
					<<(ulTidsiterasjoner + uTidsDiffTilNesteFyring)
					<<" Depol.: " <<nVerdiForDepolarisering <<" (depol. rate [" <<nVerdiForDepolariseringEndringsrate 
					<<"] \t @neuron.h asdf632\n";
			cDebug<<"( om " <<uTidsDiffTilNesteFyring <<" klikk ).\n";

			// Oppdater sNesteFyringForNeuron:
			sNesteFyringForNeuron[this] = (ulTidsiterasjoner + uTidsDiffTilNesteFyring);
			//}
		}


		std::string getNavn() const { return navn; }

		void settInnElektrisitetGjennomProbe( ){ 	fyr();  }



	
		// Holder styr på utsynapser vha. vector.
		std::vector <synapse*> pUtSynapser;

		// Holder styr på innsynapser, bl.a. for å holde orden på hetero LTD og LTP og totalVekt.
		std::vector <synapse*> pInnSynapser; 
	



		// TODO: SKAL VÆRE PRIVATE/PROTECTED. Ikkje puclic. Her fordi eg utprøver..
		static std::map<neuron*, unsigned> sNesteFyringForNeuron; 







		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *
		 *  neuron* s_get_npNesteNeuron_med_oppdatering_neste_tid()
		 *  	- arg: 	void
		 *  	- return: 	neuron* (peiker til neste på oppdateringslista).
		 *  			0 når den er ferdig, eller lista er tom.
		 *  	- static: 	static map<neuron*, unsigned>::iterator iter;
		 *  			static bool bI_gang_med_soek_etter_neuron_med_fyring_neste_tid;
		 *
		 *  	Funksjonen har en bool: bI_gang_med_soek_etter_neuron_med_fyring_neste_tid,
		 *  		som holder styr på om den er i gang med å gå gjennom lista fra før.
		 *  	Dersom den er det, vil det fortsette. 
		 *  	Dersom ikkje ellers vil den initiere iteratoren til sNesteFyringForNeuron.begin(),
		 *  		og bI_gang_med_soek.. = true.
		 * 		(slik at den ikkje initieres på nytt ved neste kall)
		 *
		 * 	Funksjonen må kalles en gang for kvar neuron* til neste neuron. Difor 
		 * 		er det best å kalle den fra while-løkke. (vil fortsette der den slapp).
		 *
		 *
		 *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static neuron* s_get_npNesteNeuron_med_oppdatering_neste_tid()
		{
			// Signalliserer tilbake at liste er tom, om Tom:
			if( sNesteFyringForNeuron.empty() ) return 0;

			static std::map<neuron*, unsigned>::iterator iter;
			static bool bI_gang_med_soek_etter_neuron_med_fyring_neste_tid = false;
		     
			// Dersom forrige søk er fullført, dvs ikkje er i gang med søk etter neuron med fyring neste tid, skal iterator resettes:
			if( ! bI_gang_med_soek_etter_neuron_med_fyring_neste_tid ){
				iter = sNesteFyringForNeuron.begin();
				cDebug<<"init iter til: [" <<(*iter).first <<", " <<(*iter).second <<"]\n";
				// Er no i gang med søk etter neste neuron. Setter bool deretter:
				bI_gang_med_soek_etter_neuron_med_fyring_neste_tid = true;
			}

			while( ++iter != sNesteFyringForNeuron.end() ){
				if( (*iter).second <= (ulTidsiterasjoner) ){	
					// returnerer neuron. Vil fortsette igjen her (siden iter ikkje vil bli resatt siden
					// 	bI_gang_med_soek_etter_neuron_med_fyring_neste_tid = true (kommer ikkje til kallet nederst)
					return (*iter).first;
				}
			}

			// Er ferdig med søk om han kommer hit. Setter bool derretter:
			bI_gang_med_soek_etter_neuron_med_fyring_neste_tid = false;
			// Signalliserer at er ferdig:
			return 0;
		}

  		static void sSkrivUt_sNesteFyringForNeuron_map()
		{
			if(sNesteFyringForNeuron.size() == 0){ cout<<"sNesteFyringForNeuron.size() == 0. (Tom liste).\n";}
			if(sNesteFyringForNeuron.empty()){ cout<<"sNesteFyringForNeuron.empty(). (Tom liste). Returnerer.\n"; return; }
			
			std::map<neuron*, unsigned>::iterator iter = sNesteFyringForNeuron.begin();
			
			cout<<"sNesteFyringForNeuron.size() : " <<sNesteFyringForNeuron.size() <<endl;
			//cout<<" \t\t*iter: " <<*((*iter).first) <<" - " <<(*iter).second <<endl;
			while( ++iter != sNesteFyringForNeuron.end() ){
	 			cout<<"|- " <<(*iter).first <<" - " <<(*iter).second <<" -|\n";
			}


			// TESTER for-løkke:
			/*cout<<"JEJEJE\tTESTER for-løkke:\n"; Veit ikkje kvifor, men for-løkke ga en del tulleadresser.
			cout<<"for\n";
			if( ! sNesteFyringForNeuron.empty()){
				cout<<"ikkje tom liste\n";
				for( iter_sNeste = sNesteFyringForNeuron.begin(); iter_sNeste != sNesteFyringForNeuron.end(); ++iter_sNeste ){
					cout<<"|- " <<(*iter).first <<" - " <<(*iter).second <<" -|\n";
				}	 	
			}*/
		
		}

		static void test(neuron * pN)
		{
			
			cout<<"\n\n\n\n\n\n\nneuron::test(neuron*)\n";
		
			// TODO Dette skal framleis implementeres, siden oppdater bare kjører i enkelte tilfeller. Dette for å holde styr på systemet:	
			neuron::sNesteFyringForNeuron[pN] = 5;
			cout<<"ja, det funka. Neuron " <<*pN <<" er knytta til tallet fem: " <<sNesteFyringForNeuron[pN] <<std::endl;
			
			
			
			cout<<"neuron::test(neuron*)\n\n\n\n\n\n\n";
		}







	// setter synapse til å være venneklasse;
	friend class synapse;
	friend std::ostream & operator<< (std::ostream & ut, neuron neuroArg );
};











/***** ikkje i bruk enda: **********/


/*******************************************************************************************************
 ********************    class arbeidsHistorieKoeElement                             *******************
 ********************        - Element som inneholder synapse*, for å registrere     *******************
 ********************          historie for utregning av neuralnett.                 *******************
 ********************        - Skal kanskje også inneholde timestamp, og en måte     *******************
 ********************          for å finne ut frekvensen eller noke. Tenk tenk.      *******************
 ******************************************************************************************************/
class arbeidsHistorieElement {
	public:
		arbeidsHistorieElement( synapse* pElement ) : arbeidsElement_synP(pElement), antallGangerBrukt(1) { }

		synapse* arbeidsElement_synP;
		int antallGangerBrukt;
	 
};














#endif

// vim:fdm=marker:fmr=//{,//}:fdl=3:
