//KANN/SANN
#include "main.h"
//#include "neuron.h"
#include "neuroSensor.h"


#define ANTALL_TIDSITERASJONER 200


//Etterkvart:
//#include <pthread.h>


//double sensorFunksjon(double*, double); //test for callback i sensor-klassa.

/**************************
 *** 	Deklarasjoner:	***
 **************************/
void* arbeidsKoeArbeider(void*);

// Deklarasjon av statiske element
// 	- i synapse: 
extern list<synapse*>  		synSkilleElement::pNesteSynapseUtregningsKoe;
//extern list<synapse*>  		synapse::p NesteSynapseSomIkkjeErFerdigOppdatert_Koe;	
// 	- i neuron:
extern std::map<neuron*, unsigned> 	neuron::sNesteFyringForNeuron;





/**********************************************************
***   int initArbeidskoe()             			***
***    	  arg: 	    -           			***
***       retur:    -           			***
*** 	  funksjon: 					***
***             Init arbeidskø med tidselement sist.    ***
***		(denne blir opprettholdt av indre funk  ***
***		i pTempTestSynapse) 			***
**********************************************************/
int initArbeidskoer()
{ /* Initierer arbeidskø //{4  } */
	//  Denne trengs ikkje som funskjon i det heile tatt, men skader ikkje.. 
	

	/* Kontrollerer om den er initialisert fra før: */
	static bool alleredeInitialisert = false;
	if( alleredeInitialisert ) return 0;
	alleredeInitialisert = true;

	synSkilleElement* pHovedSkille = new synSkilleElement();
	
	// kan bruke samme objekt i oppdateringskø og utregningskø siden desse kaller ulike funksjoner (oppdaterSyn() og aktiviserOgRegnUt() )	
	//synapse::p NesteSynapseSomIkkjeErFerdigOppdatert_Koe .push_back( pHovedSkille );
	synSkilleElement::pNesteSynapseUtregningsKoe  		    .push_back( pHovedSkille );

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

	cout<<"Kommen til neuroNett. \n\n";

	initArbeidskoer(); 
	


	neuron nD("nD");
	neuron nA("nA");
	
	new synapse (&nA, &nD);






	// void*(void*) - funksjon. Her går kalkuleringa av listene..	
//	arbeidsKoeArbeider(0);


	//nA.test_neuroParamJeje.kappa = 4;
	//cout<<"Funker: nA.test_neuroParamJeje.kappa == " <<nA.test_neuroParamJeje.kappa <<endl;

	

	cout<<"Skriver ut alle synapser til nokre neuron:\n";
	cout 	<<nA <<"\n"
		<<nD <<"\n";
		//<<sC <<"\n";

	cout<<"\n\n\n\n\nHERHERHER:\n\n";
	nA.test_neuroParamJeje.kappa = 1000;
	nA.test_neuroParamJeje.alpha = 0.001;
	cout<<"nA.test_neuroParamJeje.kappa = X; utskrift av den: " <<nA.test_neuroParamJeje.kappa <<"\n\n\n\n";



	cout<<"\n\t\tEitt sekunds stillhet for INGEN SEGAULT!\n\n";
	sleep(1);

	nA.test_neuroParamJeje.kalkulerPeriode();
	cout 	<<"Meir: nA's neuroParametre: ";
	cout 	<<nA.test_neuroParamJeje <<endl;
	
	cout << "\n\t\tJEJEJEJ\n\n\n";


	return 0;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * *   har ikkje gjort noke meir med denne. Dette låg i main før. Har plan om å lage en separat tråd som arbeider med arbeidskø.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 void* arbeidsKoeArbeider(void* ){
//{ planlagt å lage tråd for denne funk. }

	while( ulTidsiterasjoner<ANTALL_TIDSITERASJONER){

		if( synSkilleElement::pNesteSynapseUtregningsKoe.empty() ){ cout<<"\n\n SKAL ALDRI SKJE!  FEIL main.cpp : <asdf166\n\n"; exit(-1); }
	
		synapse* pSynForste = synSkilleElement::pNesteSynapseUtregningsKoe.front();
		synSkilleElement::pNesteSynapseUtregningsKoe.pop_front(); // tar vekk første element.
		
		pSynForste->aktiviserOgRegnUt();
 	} 

	return 0;

} //}


// vim:fdm=marker:fmr=//{,//} : fdl=3

