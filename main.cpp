#include "main.h"
#include "neuroEnhet.h"
#include "neuroSensor.h"


//#include <pthread.h>

// funk sist i fila, for å sleppe å deklarere alt her før bruk:
void testMain();

//double sensorFunksjon(double*, double); //test for callback i sensor-klassa.

//globale var.
pthread_t sig_gen_traad;

/**************************
 *** 	Deklarasjoner:	***
 **************************/
void* arbeidsKoeArbeider(void*);

// Deklarasjon av statiske element i synapse. 
list<synapse*>  synapse::pNesteSynapseUtregningsKoe;
list<synapse*>  synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe;	




/**********************************************************
***   int initArbeidskoe()             			***
***    	  arg: 	    -           			***
***       retur:    -           			***
*** 	  funksjon: 					***
***             Init arbeidskø med tidselement sist.    ***
***	(denne blir opprettholdt av indre funk i  	***
***	pTempTestSynapse) 				***
**********************************************************/
int initArbeidskoer()
{
	// Initierer arbeidskø.  Denne trengs ikkje som funskjon i det heile tatt, men skader ikkje..
	
	// legger til synSkilleElement sist. 
	// 	Denne oprettholder sin plass i køa (sist, etter en "iterasjon")
	
	/* Kontrollerer om den er initialisert fra før: */
	static bool alleredeInitialisert = false;
	if( alleredeInitialisert ) return 0;
	alleredeInitialisert = true;

	synSkilleElement* pHovedSkille = new synSkilleElement();
	
	// kan bruke samme objekt i oppdateringskø og utregningskø siden desse kaller ulike funksjoner (oppdater() og aktiviserOgRegnUt() )	
	synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe .push_back( pHovedSkille );
	synapse::pNesteSynapseUtregningsKoe  		    .push_back( pHovedSkille );

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


/*
Tenking rundt short-term plasticity:
	- Kvar fyring, så sleppes en mengde synaptic vesicles. 
	- Det blir reprodusert seinare.
		- (sansynlig vis med forsinkelse, og underdemping)
	- Deprevation skjer pga at synapsen går tom for synaptic vesicles.
	- Dette vil cella antagelig unngå, for seinare tid, så den setter igang med å produsere meir.
	- Meir fører til økt trykk i ventilene, og ved kanalåpning vil meir sleppes kvar gang.
	- Vi sitter igjen med en (gradvis) økt produksjon av synaptic vesicles. Denne stopper nok ikkje umiddelbart.

	Dette er muligens bakgrunnen for potentiation og augmentation

*/ 








	cout<<"Kommen til neuroNett. \n\n";

	initArbeidskoer(); 
	
	
	//testMain();


	neuron D("D");
	neuron C("C"); // C.leggTilSynapse(&D);

	
	//neuroSensor( neuron* pN, double vekt, int nFrekvens, std::string navn, int antallSig) : 
	neuroSensor B( &C, 0.1, 1, "sB", 3); // B.leggTilSynapse(&C);
	//synapse sBD(&B, &C, true, 0.5 );
	//synapse sCD(&C, &D);
	


	// funk. som er void* (void*) - funksjon	
	arbeidsKoeArbeider(0);


 	cout<<"\nFerdig\nAktuelle synapser:\n";

	for( list<arbeidsHistorieElement*>::iterator i = pArbeidsHistorieListe.begin() ; i != pArbeidsHistorieListe.end(); i++ ){
	  	cout <<((*i)->arbeidsElement_synP)  <<" er brukt " << (*i)->antallGangerBrukt <<" ganger. \n";
	}
	cout<<"\n\nog pNesteSynapseSomIkkjeErFerdigOppdatert_Koe:\n";
	for( list<synapse*>::iterator i = synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.begin() ;
		       	i != synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.end(); i++ ){
	  	cout <<(*i)  <<"\n";
	}

	return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * *   har ikkje gjort noke meir med denne. Dette låg i main før. Har plan om å lage en separat tråd som arbeider med arbeidskø.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 void* arbeidsKoeArbeider(void* ){

	while( ulTidsiterasjoner<30 ){
		if( synapse::pNesteSynapseUtregningsKoe.empty() ){ cout<<"\n\n SKAL ALDRI SKJE!  FEIL main.cpp : l.69\n\n"; exit(-1); }
	
		synapse* pSynForste = synapse::pNesteSynapseUtregningsKoe.front();
		synapse::pNesteSynapseUtregningsKoe.pop_front(); // tar vekk første element.
		
		pSynForste->aktiviserOgRegnUt();
		

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

	cout<<"\nTot. element i pNesteSynapseSomIkkjeErFerdigOppdatert_Koe: " <<synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.size() <<endl;
	return 0;

}














// vim:fdm=marker:fmr=//{,//}
