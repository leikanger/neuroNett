#include "main.h"
#include "neuroEnhet.h"
#include "neuroSensor.h"


//#include <pthread.h>

// funk sist i fila, for å sleppe å deklarere alt her før bruk:
void testMain();

//double sensorFunksjon(double*, double); //test for callback i sensor-klassa.

//globale var.
pthread_t sig_gen_traad;

/**********************************************************
***   int initArbeidskoe()             			***
***    	  arg: 	    -           			***
***       retur:    -           			***
*** 	  funksjon: 					***
***             Init arbeidskø med tidselement sist.    ***
***	(denne blir opprettholdt av indre funk i  	***
***	pTempTestSynapse) 				***
**********************************************************/
int initArbeidskoe()
{
	// Initierer arbeidskø.  Denne trengs ikkje som funskjon i det heile tatt, men skader ikkje..
	
	// legger til tidsSkilleElement sist. 
	// 	Denne oprettholder sin plass i køa (sist, etter en "iterasjon")
	// Trenger ikkje legge til fleir. Lag heller sensore (desse legges automatisk inn i pNeuroSensorListe, som sjekkes kvar tidsiter i tidsSkilleElement
	pNesteSynapseUtregningsKoe.push_back( new tidsSkilleElement() );	 
	return 1;
}


/*******************************
***   int main()             ***
***       arg:      -        ***
***       retur:    -        ***
*******************************/
int main()
{

	srand( time(0) );


/* XXX XXX XXX
Tenking rundt short-term plasticity:
	- Kvar fyring, så sleppes en mengde synaptic vesicles. 
	- Det blir reprodusert seinare.
		- (sansynlig vis med forsinkelse, og underdemping)
	- Deprevation skjer pga at synapsen går tom for synaptic vesicles.
	- Dette vil cella antagelig unngå, for seinare tid, så den setter igang med å produsere meir.
	- Meir fører til økt trykk i ventilene, og ved kanalåpning vil meir sleppes kvar gang.
	- Vi sitter igjen med en (gradvis) økt produksjon av synaptic vesicles. Denne stopper nok ikkje umiddelbart.

	Dette er muligens bakgrunnen for potentiation og augmentation

*/ /// XXX XXX








	cout<<"Kommen til neuroNett. \n\n";

	initArbeidskoe(); //pNesteSynapseUtregningsKoe.push_back( new tidsSkilleElement() );	 
	
	
	//testMain();


	neuron B("B"); 

	neuroSensor sA( &B, 1, 2, "sA", 3);
	//sA.leggTilSynapse( &C, 0.20 );





	while( ulTidsiterasjoner<30/*0000*/ ){
		if( pNesteSynapseUtregningsKoe.empty() ){ cout<<"\n\n SKAL ALDRI SKJE!  FEIL main.cpp : l.69\n\n"; exit(-1); }
	
		synapse* pSynForste = pNesteSynapseUtregningsKoe.front();
		pNesteSynapseUtregningsKoe.pop_front(); // tar vekk første element.
		
		pSynForste->regnUt();
		

		arbeidsHistorieElement* pTempHistorieElement;
		static bool bLagtTilIarbeidsHistorie;
		// sjekker om den ligger i historie:
		for( list<arbeidsHistorieElement*>::iterator i = pArbeidsHistorieListe.begin(); i != pArbeidsHistorieListe.end(); i++ ){
		
			bLagtTilIarbeidsHistorie = false;


	 		if( (*i)->arbeidsElement_synP == pSynForste ){
				
				pTempHistorieElement = (*i);
				pArbeidsHistorieListe.erase( i );
				pArbeidsHistorieListe.push_front( pTempHistorieElement );
				pArbeidsHistorieListe.front()->antallGangerBrukt++;
				bLagtTilIarbeidsHistorie = true;

				break;
			}
		}

		if( !bLagtTilIarbeidsHistorie ){ pArbeidsHistorieListe.push_front( new arbeidsHistorieElement(pSynForste) ); 	}

		
		
 	}

 	cout<<"\nFerdig\nAktuelle synapser:\n";

	for( list<arbeidsHistorieElement*>::iterator i = pArbeidsHistorieListe.begin() ; i != pArbeidsHistorieListe.end(); i++ ){
	  	cout << (*i)->arbeidsElement_synP <<" er brukt " << (*i)->antallGangerBrukt <<" ganger. \n";
	}

	return 0;
}


















void testMain()
{

}


// vim:fdm=marker:fmr=//{,//}
