/*
Kladd:
	Kanskje eg skal forenkle til å ha bolsk verdi ut. Og med dette kan eg ha step-funksjon ut, sei til synapsene at de skal aktiveres (uten å sende inn
	noke verdi, osv). Gjør dette, eg!

	
	neste medio mål er implementering av læring. Synaptisk endring. (Eg må dele opp eksitatoriske og inhibitoriske synapser). Lær om læring.


	Neste store utfordring er korleis gro eit neuralt nettverk, uten å fotfølge algoritmene?
	- En mulighet er å legge inn sted-koordinater i neuron, ELLER på annen måte dele de opp i gjupper (for kven som kan gro kor). Litt for detaljstyrende
	- lage eit grovt (ikkje detaljstyrende) overordna system, som bestemmer kva som skal gro med kven.
	- Kanskje prøve meg på eit system med atraktorer. Kan godt være veldig regnekrevande, siden det ikkje skal gro mens det kjører..
	 	(det kan godt stå og gro for seg sjølv, (offline) ei natt.. ) - derretter kommer normal læring inn...
	- online-endring gjøres kanskje med enkelte neuropeptid?
	
	- les korleis dette gjøres i kroppen.
		


*/


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

#define DEF_OPPLADINGSFART_FOR_SYN_VESICLES 	5
#define DEF_OVERFOERINGSFAKTOR_FOR_SYN   	0.1
#define DEF_startANTALL_SYN_V 		    	1000

#define REFRACTION_PERIOD_TID_ITER 2

#define LTP_HASTIGHET_endringAvAntall_POSTSYN_RECEPTOR 2 // Ved LTP...
/*
#define KONST_FOR_LTP_OG_LTD_TIDSGRENSE 2
#define KONST_FOR_LTP_LTD_ENDRING 	0.01
*/

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






// hovedklasse for AUron.
class neuron {
	private:
		
		// ta med timedelay (vha. timestamp) ? for å få meir likt biosys.  


	protected:
		double dVerdiForUtregningAvSigmaverdiForrigeIter;


		// endrer tid fra denne til å være diskret oppdatert tid vha en klokkekrets med neuron, som også ligger i fifo-arbeidskø
		// ( arbeidsKoe_for_utregningAvNeuralnett_nesteSynapseP ).
		unsigned long ulTimestampForrigeInput;
		unsigned long ulTimestampFyring;
		
		std::string navn;

		double terskel;


		// skal ikkje vekk, men skal endre funksjonalitet til å sjå om neuronet fyrer når den får eit innsignal. 
		// (timestamp og condensatorlignande utlanding for temporal integrasjon)
		virtual int sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( double dInnsignalArg )
		{
			// TODO Legg inn funskjonalitet for å sjekke om postsynaptisk neuron fyrer, og gi int-returverdi (eller bool) på grunnlag av dette.
			// BYTTER MED ulTimestampForrigeInput ... : timeval tempKlokke;
		
		 	unsigned long ulKlokketikkSidenSisteInput =   ulTidsiterasjoner - ulTimestampForrigeInput;

		
	
			double tempFaktor = 1;
			for(unsigned int i=0; i < ulKlokketikkSidenSisteInput; i++ ){ 	tempFaktor *= DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON;   }
 			
			double foerTidSig = dVerdiForUtregningAvSigmaverdiForrigeIter;
			
			// merk *= som betyr at det er lik seg sjølv ganger tempFaktor
			dVerdiForUtregningAvSigmaverdiForrigeIter *= tempFaktor; 
		
			double etterTidSig = dVerdiForUtregningAvSigmaverdiForrigeIter;
		
			dVerdiForUtregningAvSigmaverdiForrigeIter += dInnsignalArg;
		
			if( ulTidsiterasjoner % 1000 == 0){
				cout <<"\tTid siden sist:   " <<ulKlokketikkSidenSisteInput <<" (tid no:" <<ulTidsiterasjoner <<")\t";
				
				cout<<" Verdi ( før | etter-tid | m.nytt_signal ) :\t" ; 	cout.width(8);
				cout<<std::left <<foerTidSig; 	cout<<" \t|\t"; cout.width(8); cout<<etterTidSig;
				cout<<" \t|\t"; cout.width(8); 	cout<<dVerdiForUtregningAvSigmaverdiForrigeIter <<"\n";

				//cDebug 	<<"klokke:\t\t\t" <<ulTidsiterasjoner <<", forrige: " <<ulTimestampForrigeInput 
				//	<<"DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON: " <<DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON <<std::endl;
			}

			// tilordner tempklokke til timestamp for verdien for dVerdiForUtregningAvSigmaverdiForrigeIter (neuron::timestamp_forrige_input)
			ulTimestampForrigeInput = ulTidsiterasjoner;
			
			

			// ser om det går over terskel:
			if( sigma_funk( dVerdiForUtregningAvSigmaverdiForrigeIter )  )
			{
				// Resetter dVerdiForUtregningAvSigmaverdiForrigeIter, etter fyring:
				dVerdiForUtregningAvSigmaverdiForrigeIter = 0;
				
				// refraction-period for neuron:
				if( (ulTidsiterasjoner - ulTimestampFyring) <= REFRACTION_PERIOD_TID_ITER ){	return 0;   	}
	

				
				// denne fyrer: sjekk alle som den synapser til ( legg til i arbeidskø ):
				for( std::vector<synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
				{ 	// legger alle synapsene mine inn i arbeidskø
					pNesteSynapseUtregningsKoe.push_back( *iter );
				}	//Legg til alle synapser i neste fifo-kø foR utregning. //OK // går over til bare ei kø, med "tids-synapse"
	
				
				// Setter timestamp for fyring:
				ulTimestampFyring = ulTidsiterasjoner;

				// Sender respons om at eg fyrer:
				return 1;
			}

			// Dersom det ikkje fyrer: ikkje fyr av aksjonspotensial.
			return 0;
			
			// finn ut en grunn til å ha returverdi her. (endre til void?)
			// 	Bool returverdi som beskriver om den fyrer eller ikkje...
		}



		// blir overlagra i neuroSensor.
		virtual int sigma_funk( double verdi ){ 
		//	return (tanh( (5*verdi -5.0)*PI ) +1)/2 ;
							// | Her settes terskel.. XXX sett lik |neuron::terskel| TODO
		
			//går over til enkel step-funksjon:
			if(verdi > terskel){
				cout 	<<"\n * * * * * * * ** ***** neuron " <<navn <<" **** *** ** * * * *  * * * *\n";
					//<<"\n\t* * * * * * * * * * * * *** * *** ** **** **** *** ** ** *** *** ** ** ** * \n"
					//<<"\t* * * * * * * * * **** * Oh-jeah! NEURON FYRER!\t" <<navn <<" **** * * * * * * * * * * *\n"
					//<<"\t* * * * * * * * * * * * ** **** * *** *** *** ** ** * * **** * * * ** *  * *\n";
 				

				return 1;
		 	}else
				return 0;
	
			// 	skal eg ha fyring, eller ikkje-fyring. Og evt synapsene stå for inhibitorisk/eksit. effekt? 
			// 		( dette går også inn under læring. (graden styrken til e/i effekt)  )

		}

	public:
		neuron() 		: ulTimestampForrigeInput(0), navn("uten navn"),   terskel(TERSKEL_DEFAULT) 	{ }  		// Kanskje ta vekk???
		neuron( std::string n ) : ulTimestampForrigeInput(0), navn(n), 		 terskel(TERSKEL_DEFAULT)	{ }  		// constructor.

		//neuron( std::string n, double t ) : navn(n){ }  	// constructor.
		
		// Lag andre construcorer, som er i samsvar med metode for vaksing av n.n. (synapser, postsyn. neuron (t.d. vha. grupperinger av neuron-type 			tatt inn som int, og vokser synapser til nære neuron ut fra kva gruppe det er i (t.d. gruppe 1 vokser til gruppe 1,2  , og gr.2 til 1,2,3)		    eller ulike grupper oppfører seg ulikt, f.eks. en type neuron vokser til alle i nærheten, men ei anna gruppe ikkje vokser uten eksplisitte 			kall.
		

	
		//double forrigeUtregnaVerdi; /// XXX Ta bort ?

		// Holder styr på utsynapser vha. vector.
		std::vector <synapse*> pUtSynapser;
		//int antallUtSynapser;
	
		//void leggTilSynapse( synapse * pniNyUt ){ pUtSynapser.push_back(pniNyUt); } //ikkje i bruk enda. Når den blir avkommentert: 
														// husk nAntallNeuronEgSynapserTil
		void leggTilSynapse( neuron* nyttUtNeuron, double vekt); // definert i .cpp - fila, pga avhengigheter mellom synapse- og neuron- klassa.


		


		std::string getNavn() const { return navn; }
		




		void test() //bare for å teste grunn-funksjonen
		{
		
	
		}
	// setter synapse til å være venneklasse;
	friend class synapse;
	friend std::ostream & operator<< (std::ostream & ut, neuron neuroArg );
};













/**************************************************************************************************
***** TODO gjør slik at ladingsfarta for synaptic vesicles er bra. No går han rett opp over *******
***** 		til 113 (%). Uten provosering. Skal gå til 100. I tilfelle augmentation,    *******
***** 		skal den gå over. XXX 							    *******
**************************************************************************************************/



//  SKAL EG HA PRE- og POST- syn. læringseffekt? Eller er dette interresant bare vitenskapelig (årsak bak LTP).

/*******************************************************************************************************
 ********************   PLAN, LÆRING:                                                *******************
 ********************                                                                *******************
 ********************       - LTP :                                                  *******************
 ********************           - øker synapsevekt.                                  *******************
 ********************           - når skjer dette? Undersøk / tenk !                 *******************
 ********************                                                                *******************
 ********************       - HomoLTD :                                              *******************
 ********************           ( Synapsen fyrer masse, men postsyn. er stille )     *******************
 ********************           - Dette kan overføres til tidsplanet med å sei at    *******************
 ********************               ved syn.sig. og ingen postsyn. sig. så           *******************
 ********************               litt litt homoLTD (minker synapsevekt).          *******************      
 ********************               ( og litt øking ved motsatt tilfelle )           *******************
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
		synapse( neuron* pPreN_arg, neuron* pPostN_arg, double v ) : 
				ulAntallSynapticVesiclesAtt  (DEF_startANTALL_SYN_V),
				dGlutamatReceptoreIPostsynMem(DEF_OVERFOERINGSFAKTOR_FOR_SYN),
				dOppladingsFartForSynVesicles(DEF_OPPLADINGSFART_FOR_SYN_VESICLES),
				ulAntallSynV_setpunkt 	     (DEF_startANTALL_SYN_V),
				pPreNode(pPreN_arg), pPostNode(pPostN_arg), synaptiskVekt(v)
		{ }

		neuron* pPreNode;
		neuron* pPostNode;

		// FINN på en måte å lage skille mellom eksiterende og inhibiterende synapser. Kanskje bool for eksitatorisk?
		// Dette er viktig når eg begynner med læring / vektendring...
		
		double synaptiskVekt; 	// Denne er avhengig av / har betydning  :  vekt.


		// ikkje i bruk enda:
		//double LTP_og_LTD; 	// for å legge til til signal (synaptiskVekt).
				      	//   - er long-term men ikkje permanent. Når den blir permanent, føres det inn i variabelen over (synaptiskVekt).
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
		virtual void regnUt() // heite før: void overfoerSignaletTilPostsynNeuron()
		{
			// Implementerer short-term depression ved å legge inn (int dProsentSynapticVesiclesAtt) i neuron-klassa og 
			//    slepping av maksimalt synVesicles er proposjonalt med denne ( antall sluppetSynV < konst * dAntallSyn... )
			//    og lineær opplading av dProsentSynapticVesiclesAtt. Denne lineære oppladinga kan kanskje være en del av 
				// den synaptiske plastisiteten.
			
			/*
			 * XXX HAR Gått ut fra at det er konstant antall neurotransmittore inni syn.vesicles.
			 * XXX 	Dette kan være feilaktig antagelse..
			*/



			/******* Synaptisk depression: ******/ //For å begrense signalet til presyn->maksSignal. Denne er for short-time synaptic depression.
			oppdater();

			// Signaloverføring er avhengig av antall receptorer for aktuelle neurotransmittor. Ganger med en variabel
			// 	definert for synapse, og som kan variere med LTP/LTD.
			

			//
			// Denne var double -> unsigned long. Har ikkje kompilert etter..

			unsigned long ulTempSynSignal_antallSV = 0.1 * ulAntallSynapticVesiclesAtt * synaptiskVekt; // eller synaptiskVekt
					     			// | f.eks. *0.1
			
			cout 	<<"Sender " <<ulTempSynSignal_antallSV <<" syn.V. til " <<pPostNode->navn 
				<<"\tAv gjenverande " <<ulAntallSynapticVesiclesAtt 
				<<"\t og referansepkt. for antall s.V. " <<ulAntallSynV_setpunkt <<"\n";
		
			// og trekker fra de brukte syn.vesicles fra dProsentSynapticVesiclesAtt.
			ulAntallSynapticVesiclesAtt -= ulTempSynSignal_antallSV;

			/******* sender inn *******/ // if-testen er til for å sjå returverdien til sendInnPosts..()  (om postsyn. fyrer)
			if( pPostNode->sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( ulTempSynSignal_antallSV * dGlutamatReceptoreIPostsynMem ) )
			{
				// postsyn. neuron fyrer. Legger til alle synapsene dets i FIFO arbeidskø:
				//pPostNode->...
				// NEI. Dette blir gjort inne i sendInnPo...()
				//gjør noke anna her, da kanskje?
			}

			ulTimestampForrigeSignal = ulTidsiterasjoner;
		}		
		friend std::ostream & operator<< (std::ostream & ut, synapse );
};







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











#endif

// vim:fdm=marker:fmr=//{,//} : fdl=3
