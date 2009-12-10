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

#define DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON 0.95 // for eksempel..         (ganger med denne faktoren per klokketikk) 

//#define REFRACTION_PERIOD_TID_ITER 2


#define TERSKEL_DEFAULT 95

// klasse-deklarasjoner:
class synapse;
class neuron;
class neuroSensor;
class arbeidsHistorieElement;

// globale variabelDeklarasjoner:
extern unsigned long ulTidsiterasjoner;
extern list<synapse*> 		    pNesteSynapseUtregningsKoe;
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
		
		// ta med timedelay (vha. timestamp) ? for å få meir likt biosys.  


	protected:
		int nVerdiForDepolarisering;


		// endrer tid fra denne til å være diskret oppdatert tid vha en klokkekrets med neuron, som også ligger i fifo-arbeidskø
		// ( arbeidsKoe_for_utregningAvNeuralnett_nesteSynapseP ).
		unsigned long ulTimestampForrigeInput;
		unsigned long ulTimestampFyring;
		
		std::string navn;

		double terskel;

		virtual void kalkulerDegraderingAvVerdiPaaGrunnlagAvTimestampSidenSist( )
		{
			// BYTTER MED ulTimestampForrigeInput ... : timeval tempKlokke;
		
		 	unsigned long ulKlokketikkSidenSisteInput =   ulTidsiterasjoner - ulTimestampForrigeInput;

		
	
			double tempFaktor = 1;
			for(unsigned int i=0; i < ulKlokketikkSidenSisteInput; i++ ){ 	tempFaktor *= DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON;   }
 			
			// merk *= som betyr at det er lik seg sjølv ganger tempFaktor
			nVerdiForDepolarisering *= tempFaktor;  
		
			cout <<"Tid siden sist:   " <<ulKlokketikkSidenSisteInput <<" (tid no:" <<ulTidsiterasjoner <<")\t\n";
		
		}



		// skal ikkje vekk, men skal endre funksjonalitet til å sjå om neuronet fyrer når den får eit innsignal. 
		// (timestamp og condensatorlignande utlanding for temporal integrasjon)
		virtual int sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( double dInnsignalArg )
		{
			

			int foerTidSig = nVerdiForDepolarisering; 		// for utskrift.
			kalkulerDegraderingAvVerdiPaaGrunnlagAvTimestampSidenSist();
			int etterTidSig = nVerdiForDepolarisering; 	 	// for utskrift.
			
			// Legger til verdi.
			nVerdiForDepolarisering += dInnsignalArg;
			

			// utskrift:
			//if( ulTidsiterasjoner % 1000 == 0){
				
				cout<<" Verdi ( før | etter-tid | m.nytt_signal ) :\t" ; 	cout.width(8);
				cout<<std::left <<foerTidSig; 	cout<<" \t|\t"; cout.width(8); cout<<etterTidSig;
				cout<<" \t|\t"; cout.width(8); 	cout<<nVerdiForDepolarisering <<"\n";

			//}

			// tilordner tempklokke til timestamp for verdien for nVerdiForDepolarisering (neuron::timestamp_forrige_input)
			ulTimestampForrigeInput = ulTidsiterasjoner;
			
			

			// ser om det går over terskel:
			if( sjekkKorDepolarisertNeuronEr() == -1 )
			{
				fyr();
			}

			// Dersom det ikkje fyrer: ikkje fyr av aksjonspotensial.
			return 0;
			
			// finn ut en grunn til å ha returverdi her. (endre til void?)
			// 	Bool returverdi som beskriver om den fyrer eller ikkje...
		}
		
		
		// grunnen til overlagring er refraction period. XXX Den kan eg kanskje ta vekk, uansett
		virtual int fyr()
		{
	 
			// refraction-period for neuron:
			//if( (ulTidsiterasjoner - ulTimestampFyring) <= REFRACTION_PERIOD_TID_ITER ){	return 0;   	}

			// Resetter nVerdiForDepolarisering, etter fyring:
			nVerdiForDepolarisering = 0;

			
			// denne fyrer: sjekk alle som den synapser til ( legg til i arbeidskø ):
			for( std::vector<synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
			{ 	// legger alle synapsene mine inn i arbeidskø
				pNesteSynapseUtregningsKoe.push_back( *iter );
			}	//Legg til alle synapser i neste fifo-kø foR utregning. //OK // går over til bare ei kø, med "tids-synapse"
	
			// TODO Legg også inn litt LTP i kva synapse. TODO
			
			// Setter timestamp for fyring:
			ulTimestampFyring = ulTidsiterasjoner;

			// Sender respons om at eg fyrer:
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

			if( nVerdiForDepolarisering > terskel){
				cout 	<<"\n * * * * * * * ** ***** neuron " <<navn <<" **** *** ** * * * *  * * * *\n";

				return -1;
		 	}else
				return nVerdiForDepolarisering;
	
		}

	public:
		neuron() 		: nVerdiForDepolarisering(0), ulTimestampForrigeInput(0), navn("uten navn"),   	terskel(TERSKEL_DEFAULT) { } 
		neuron( std::string n ) : nVerdiForDepolarisering(0), ulTimestampForrigeInput(0), navn(n), 		terskel(TERSKEL_DEFAULT) { } 

		// Lag andre construcorer, som er i samsvar med metode for vaksing av n.n. (synapser, postsyn. neuron (t.d. vha. grupperinger av neuron-type 			tatt inn som int, og vokser synapser til nære neuron ut fra kva gruppe det er i (t.d. gruppe 1 vokser til gruppe 1,2  , og gr.2 til 1,2,3)		    eller ulike grupper oppfører seg ulikt, f.eks. en type neuron vokser til alle i nærheten, men ei anna gruppe ikkje vokser uten eksplisitte 			kall.
		

	
		// Holder styr på utsynapser vha. vector.
		std::vector <synapse*> pUtSynapser;
	
		//void leggTilSynapse( synapse * pniNyUt ){ pUtSynapser.push_back(pniNyUt); } //ikkje i bruk enda. Når den blir avkommentert: 
														// husk nAntallNeuronEgSynapserTil
		void leggTilSynapse( neuron* nyttUtNeuron, double vekt); // definert i .cpp - fila, pga avhengigheter mellom synapse- og neuron- klassa.
		// TODO lag tabortSynapse() for bruk ved veldig stor LTD.

		
		std::string getNavn() const { return navn; }
		




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
