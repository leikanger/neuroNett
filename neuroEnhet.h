//{3
/*
Kladd:

	Neste store utfordring er korleis gro eit neuralt nettverk, uten å fotfølge algoritmene?
	- En mulighet er å legge inn sted-koordinater i neuron, ELLER på annen måte dele de opp i gjupper (for kven som kan gro kor). Litt for detaljstyrende
	- lage eit grovt (ikkje detaljstyrende) overordna system, som bestemmer kva som skal gro med kven.
	- Kanskje prøve meg på eit system med atraktorer. Kan godt være veldig regnekrevande, siden det ikkje skal gro mens det kjører..
	 	(det kan godt stå og gro for seg sjølv, (offline) ei natt.. ) - derretter kommer normal læring inn...
	- online-endring gjøres kanskje med enkelte neuropeptid?
	
	- les korleis dette gjøres i kroppen.



*/
//}3


//  LÆRING:
//  Skal eg ha pre- og post- syn. læringseffekt? Eller er dette bare interresant vitenskapelig (årsak bak LTP).

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





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * *                        KODE STARTER :                           * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SYNAPSE
    #define NEURON
    #include "synapse.h"
#endif

#include <iostream>
#include <iomanip>
#include <stdlib.h> 	// for exit()
#include <list> 	// for list
#include <vector>


using std::list;
using std::cout;
using std::endl;




#define cDebug std::cerr

// TODO gjør denne mykje mindre. En tidsiterasjon skal være veldig lite, (mykje mindre) og skal dermed føre til mindre degradering av verdi.

#define DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON 0.96 // for eksempel..         (ganger med denne faktoren per klokketikk) 

//#define REFRACTION_PERIOD_TID_ITER 2


#define TERSKEL_DEFAULT 100

// klasse-deklarasjoner:
class synapse;
class neuron;
class neuroSensor;
class arbeidsHistorieElement;

// globale variabelDeklarasjoner:
extern unsigned long ulTidsiterasjoner;
//extern list<synapse*> 		    pNesteSynapseUtregningsKoe;
extern list<arbeidsHistorieElement*> pArbeidsHistorieListe;

extern list<neuroSensor*> pNeuroSensorListe; // extern for at den kan deklareres her.


#ifndef neuroEnhet_h
#define neuroEnhet_h





/*******************************************************************************************************
 ********************    class neuron                                                *******************
 ********************        - Hovedklasse for Auron. Innholder alt, med unntak      *******************
 ********************          av synapse-funksjonaliteten.                          *******************
 ******************************************************************************************************/
class neuron {
	private:

	protected:
		int nVerdiForDepolarisering;

		unsigned long ulTimestampForrigeInput;
		unsigned long ulTimestampFyring;
		
		std::string navn;

		int nTerskel;

		virtual void kalkulerDegraderingAvVerdiPaaGrunnlagAvTimestampSidenSist( )
		{
		 	unsigned long ulKlokketikkSidenSisteInput =   ulTidsiterasjoner - ulTimestampForrigeInput;
		
			double tempFaktor = 1;
			// optimalisering_ : gjør fra neste linje til : pow( DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON, ulKlokketikkSidenSisteInput, 
			for(unsigned int i=0; i < ulKlokketikkSidenSisteInput; i++ ){ 	tempFaktor *= DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON;   }
 			// TODO endre til tempFaktor *= DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON ...	optimalisering..
			// merk *= som betyr at det er lik seg sjølv ganger tempFaktor
			nVerdiForDepolarisering *= tempFaktor;  
		
			//cout <<"Tid siden sist:   " <<ulKlokketikkSidenSisteInput <<" (tid no:" <<ulTidsiterasjoner <<")\t\n";
		
		}


		virtual int sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( int nInnsignalArg )
		{

			int foerTidSig = nVerdiForDepolarisering; 		// for utskrift.
			kalkulerDegraderingAvVerdiPaaGrunnlagAvTimestampSidenSist();
			int etterTidSig = nVerdiForDepolarisering; 	 	// for utskrift.
			
			// Legger til verdi.
			nVerdiForDepolarisering += nInnsignalArg;
			
			// utskrift:
			//if( ulTidsiterasjoner % 1000 == 0){
			cout<<" Verdi ( før-tid | etter-tid ... ) :\t:\t" ; 	cout.width(8);
			cout<<std::left <<foerTidSig; 	                     cout<<"    |    ";  cout.width(8); cout<<etterTidSig;
			cout<<" \t+\t"; cout.width(8); cout<<nInnsignalArg; cout<<" \t=>\t";  cout.width(8); 	cout<<nVerdiForDepolarisering <<"\n";
			//}

			// tilordner tempklokke til timestamp for verdien for nVerdiForDepolarisering (neuron::timestamp_forrige_input)
			ulTimestampForrigeInput = ulTidsiterasjoner;
			
			// ser om det går over terskel:
			int returverdi;
			if( (returverdi = sjekkKorDepolarisertNeuronEr() ) == -1 )
			{
				fyr();
				return returverdi;
				//return -1;
			}else{
				return returverdi;
			}
		}
		
		
		// grunnen til overlagring er refraction period. XXX Den kan eg kanskje ta vekk, uansett
		virtual int fyr()
		{
	 
			// refraction-period for neuron: (ikkje lov å gjøre to fyringer ila. eit sekund.)
			if( ulTimestampFyring == ulTidsiterasjoner ){ 	cout<<"AVSLUTTER\n\nl.211 i neuronEnhet.hi\n";  exit(-1);/*skal være return 0; */   }

			// Resetter nVerdiForDepolarisering, etter fyring:
			nVerdiForDepolarisering = 0;

			
			// denne fyrer: sjekk alle som den synapser til ( legg til i arbeidskø ):
			for( std::vector<synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
			{ 	// legger alle synapsene mine inn i arbeidskø
				synapse::pNesteSynapseUtregningsKoe.push_back( *iter );
			}	//Legg til alle synapser i neste fifo-kø foR utregning. //OK // går over til bare ei kø, med "tids-synapse"
	
			// TODO Legg også inn litt LTP i kva synapse. TODO
			// Er i gang med å lage engen LTP / homoD funksjon. kall her, dersom det skal skje..
			
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

			if( nVerdiForDepolarisering > nTerskel){
				cout 	<<"\n\n* * * * * * * * * ** ** ** **** *******           neuron  " <<navn <<"          ******** ***** *** ** ** * * * * * * * * * * * * * * * * * * * * * *\n\n";

				return -1;
		 	}else
				return nVerdiForDepolarisering;
	
		}





		// Ikkje i full bruk enda, men god ide. For å sjekke kva som ligger bak facilitation.. (regtek ide, og oversving)
		unsigned uTotaltAntallReceptoreIPostsynNeuron_setpunkt; // ta med egenfrekvens også, etterkvart. for now skal denne være konst.




	public:
		//neuron() 		: nVerdiForDepolarisering(0), ulTimestampForrigeInput(0), navn("uten navn"),   	terskel(TERSKEL_DEFAULT) { } 
		neuron( std::string n ) : nVerdiForDepolarisering(0), ulTimestampForrigeInput(0), navn(n), 		nTerskel(TERSKEL_DEFAULT), 
			uTotaltAntallReceptoreIPostsynNeuron_setpunkt(1000) 
 		{ } 

		// Lag andre construcorer, som er i samsvar med metode for vaksing av n.n. (synapser, postsyn. neuron (t.d. vha. grupperinger av neuron-type 			tatt inn som int, og vokser synapser til nære neuron ut fra kva gruppe det er i (t.d. gruppe 1 vokser til gruppe 1,2  , og gr.2 til 1,2,3)		    eller ulike grupper oppfører seg ulikt, f.eks. en type neuron vokser til alle i nærheten, men ei anna gruppe ikkje vokser uten eksplisitte 			kall.
		

	
		// Holder styr på utsynapser vha. vector.
		std::vector <synapse*> pUtSynapser;

		// Holder styr på innsynapser, bl.a. for å holde orden på hetero LTD og LTP og totalVekt.
		std::vector <synapse*> pInnSynapser; 
	
		//void leggTilSynapse( synapse * pniNyUt ){ pUtSynapser.push_back(pniNyUt); } //ikkje i bruk enda. Når den blir avkommentert: 
														// husk nAntallNeuronEgSynapserTil
		/* * * * * * TODO Ta vekk: TODO * * * * * * * */
		void leggTilSynapse( neuron* nyttUtNeuron, bool inhib, float vekt =1); 
			// definert i .cpp - fila, pga avhengigheter mellom synapse- og neuron- klassa.

		
		std::string getNavn() const { return navn; }
		


		void settInnElektrisitetGjennomProbe( ){
	 		fyr();
		}

		void test() //bare for å teste grunn-funksjonen
		{
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
