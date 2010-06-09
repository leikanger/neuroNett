
//KANN
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

#define TERSKEL_DEFAULT 100
#define HALV_TERSKEL_DEFAULT TERSKEL_DEFAULT/2



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


#ifndef DA_NEURON
  #define DA_NEURON
 
  #include "neuron.h"



/*******************************************************************************************************
 ********************    class DA_neuron       ( Dopamin )                               ***************
 ********************        - Hovedklasse for Ventral tegmental area - neuron - Dopamin.***************
 ********************        - Leverer ut dopamin til andre celler ( meir ved reward ).  ***************
 ********************            Dette gir auka grad av LTP. Tonisk fyring               ***************
 ********************            vedlikeholder synaptisk vekt ved å vedlikeholde vektauke.**************
 ********************            (LTD er stabil, eller avhengig av størrelsen på syn.vekt)**************
 ********************        - Pause gir minka vekt, phasic fyring gir auke i vekt.       **************
 ********************        ( tenker kanskje tonic-fyring varierer litt som MA-filter )  ************** (frekvensen til fyringa ..)
 ******************************************************************************************************/
class DA_neuron : public neuron{

	Ok, da har eg gjort neuron klart foR å bli nedarva hit: Har laga eit godt system foR egenfyring (egeneksitasjon), med optimalisering s.a. eg ikkje trenger å gjøre det alle tidsiterasjonane, men bare ved oppdatering.

		Vidare trenger eg en måte å levere ut DA på, som er relativt spesific. (treffer de aktuelle neurona som gjorde en god jobb).
		- her har eg begynnt å tenke på naturlig-utvalg--inspirerte metoder (dette kan gjelde generellt). Finn ut korleis neuron "dør".
			- kanksje mangel på DA (etter mykje skuff?) gjør at neuron dør. Darwinistisk plausibelt!
		- har også begynnt å tenke på pre-DA-circuits. 
			[ Her er det viktig å sjå på det på rett måte: Eit neuron har vansker med å eksitere eit anna over terskelen, men mange
			inputneuron (der kvart har mange output-neuron) gir oss ei sky med neuron som summert skal klare å eksitere kun målet over
			fyringsterskelen (uten at andre neuron fyrer. Dette på tross av at kvart neuron har i snitt 1000 målneuron i hjerna. ]

			- 2 pre-DA-neuron. En f0r E og en f0r I virkning på DA-neurona. Desse får innput fra samme neuron som DA-neurona output til.
			- har output til DA-neurona. (og andre effekter og?)

			Ved handling, vil handlingsneurona eksitere preDA-neurona nesten opp til fyring. Avhengig av om resultatet er bra/dårlig, 
			 	- ved bra: EpDA bli eksitert over terskelen, og sende (burst?)-mld til DA-n. Dette fører videre til phasic firing av DA-n.
				- ved dårlig: IpDA bli eksitert over terskelen, og sende (burst?) mld til DA-n. Dette fører til stopp i tonisk fyring i DA-n.
			- kva med at perioden auker ved grad av brahet?
				- kanskje dette gjøres ved å rekruttere fleire preDA-neuron f0r jobben?
		
			Dette (preDA-neuron) gir oss:
				- forventning: Forventning er en god følelse. Kanskje fordi det sporadisk blir eksitert DA-n. over terskelen?
				- spesifikk DA-leveranse. DA-leveransen vil da gå til rett sted. Seier ikkje noke om oppløsninga. Dette er gitt med antall pDA-kretsr

Virkemåte f0r DA. Kanskje DA også auker ekstiabiliteten til neuronet. Dette gjør at etter å fatt meir forsyning med DA, vil neuronet lettere bli eksitert i tillegg til høgare syn.plasticitet (i pluss-retninga)


}
