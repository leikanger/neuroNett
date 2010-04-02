#include "main.h"
#include "neuron.h"
#include "neuroSensor.h"


#define ANTALL_TIDSITERASJONER 200


//#include <pthread.h>

// funk sist i fila, for å sleppe å deklarere alt her før bruk:
void testMain();

//double sensorFunksjon(double*, double); //test for callback i sensor-klassa.

//globale var.
//pthread_t sig_gen_traad;

/**************************
 *** 	Deklarasjoner:	***
 **************************/
void* arbeidsKoeArbeider(void*);

// Deklarasjon av statiske element
// 	- i synapse: 
extern list<synapse*>  		synapse::pNesteSynapseUtregningsKoe;
extern list<synapse*>  		synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe;	
// 	- i neuron:
extern std::map<neuron*, unsigned> 	neuron::sNesteFyringForNeuron;

//slett:
//extern vector<synapse*> slett_vpAlleSynapeneSomErContruert;




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
{ /* Initierer arbeidskø //{1  } */
	//  Denne trengs ikkje som funskjon i det heile tatt, men skader ikkje.. 
	

	/* Kontrollerer om den er initialisert fra før: */
	static bool alleredeInitialisert = false;
	if( alleredeInitialisert ) return 0;
	alleredeInitialisert = true;

	synSkilleElement* pHovedSkille = new synSkilleElement();
	
	// kan bruke samme objekt i oppdateringskø og utregningskø siden desse kaller ulike funksjoner (oppdaterSyn() og aktiviserOgRegnUt() )	
	synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe .push_back( pHovedSkille );
	synapse::pNesteSynapseUtregningsKoe  		    .push_back( pHovedSkille );

	return 1;
} //}


/*******************************
***   int main()             ***
***       arg:      -        ***
***       retur:    -        ***
*******************************/
int main(int argc, char *argv[])
{ 

	// initierer rand() - seed.
	srand( time(0) );

/* //{ Tom for kode: utkommentert! --Tenking.
Tenking rundt short-term plasticity: 
	- Kvar fyring, så sleppes en mengde synaptic vesicles. 
	- Det blir reprodusert seinare.
		- (sansynlig vis med forsinkelse, og underdemping)
	- Deprevation skjer pga at synapsen "går tom" for synaptic vesicles.
	- Dette vil cella antagelig unngå, for seinare tid, så den setter igang med å produsere meir.
	- Meir fører til økt trykk i ventilene, og ved kanalåpning vil meir sleppes kvar gang.
	- Vi sitter igjen med en (gradvis) økt produksjon av synaptic vesicles. Denne stopper nok ikkje umiddelbart.

	Dette er muligens bakgrunnen for potentiation og augmentation
*/ //}1 


	cout<<"Kommen til neuroNett. \n\n";

	initArbeidskoer(); 
	

	/* *********************************************************************
	 * *****   Trenger faktisk ikkje å holde styr på neurona.          *****
	 * *****    bare at                                                *****
	 * *****                                                           *****
	 * *****                                                           *****
	 * *********************************************************************/ 
	neuron nD("nD");
	neuron nA("nA");
	
	synapse (&nA, &nD);









	//først: få orden på likevekt uten signal.
	//synapse_likevekt Test(&C,&D);
	
	//så: få orden, med signal (facilitation, osv.)
	//neuroSensor sB( &nD, 0.1, 1, "sB", 3); // B.leggTilSynapse(&C);
	//neuroSensor sC( &nD, 0.1, 1, "sC", 3); // B.leggTilSynapse(&C);
	
	// For testing av syn._likevekt: 
	//synapse_likevekt synapsa( &sB, &D );

	
	//nD.oppdaterNeuron();

	// void*(void*) - funksjon. Her går kalkuleringa av listene..	
	arbeidsKoeArbeider(0);


	

	//neuron feil("feilNeuron");
	//neuron::sNesteFyringForNeuron[&feil] = 5;


	/*/ Skriver ut alle synapsene:
	cout<<"\n\nSkriver ut absolutt alle synapsene:\n";
	vector<synapse*>::iterator iter = slett_vpAlleSynapeneSomErContruert.begin();
	
	while( ++iter != slett_vpAlleSynapeneSomErContruert.end() ){
		cout<<(*iter);
	}*/

	cout<<"Skriver ut alle synapser til nokre neuron:\n";
	cout 	<<nA <<"\n"
		<<nD <<"\n";
		//<<sC <<"\n";


	cout<<"\nSkriver ut sNesteFyringForNeuron : \n";
	neuron::sSkrivUt_sNesteFyringForNeuron_map();


	cout<<"\n\t\tEitt sekunds stillhet for INGEN SEGAULT!\n\n";
	sleep(1);
/*
	for( list<arbeidsHistorieElement*>::iterator i = pArbeidsHistorieListe.begin() ; i != pArbeidsHistorieListe.end(); i++ ){
	  	cout <<((*i)->arbeidsElement_synP)  <<" er brukt " << (*i)->antallGangerBrukt <<" ganger. \n";
	}
*/

	//Funker ikkje heilt, men eg er inne på løysing. Prøv å skrive ut verdien til (*i).
/*	cout<<"\n\nog pNesteSynapseSomIkkjeErFerdigOppdatert_Koe:\n";
	for( list<synapse*>::iterator i = synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.begin() ;
		       	i != synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.end(); i++ ){
	  	cout <<(*i)  <<"\n------------------------\n";
	}
*/

	return 0;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * *   har ikkje gjort noke meir med denne. Dette låg i main før. Har plan om å lage en separat tråd som arbeider med arbeidskø.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 void* arbeidsKoeArbeider(void* ){
//{ planlagt å lage tråd for denne funk. }
	while( ulTidsiterasjoner<ANTALL_TIDSITERASJONER){
		if( synapse::pNesteSynapseUtregningsKoe.empty() ){ cout<<"\n\n SKAL ALDRI SKJE!  FEIL main.cpp : <asdf166\n\n"; exit(-1); }
	
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


		// bare skitten-testing:
		//if( ulTidsiterasjoner%150 == 0 ) (*pNeuroSensorListe.begin())->settInnElektrisitetGjennomProbe(); 

 	} 

	cout<<"\nTot. element i pNesteSynapseSomIkkjeErFerdigOppdatert_Koe: " <<synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.size() <<endl;
	return 0;

} //}


// vim:fdm=marker:fmr=//{,//} : fdl=3

