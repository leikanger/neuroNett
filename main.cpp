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


/* VIL FUNKE, men trøbler med ostream. Sjekk i c++Boka!
std::ostream & groenUtskrift (std::ostream & ut, const std::string tekst)
{
	ut 	<<tekst <<"\n";
/ *
	ut 	<<"\033[0;32;40m %02s"
		<<tekst
		<<"%02s  \n"; */
//	return ut;
//}

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
	
	// Lagringa av peikaren er ikkje naudsynt. Gjør det for å aksessere medlemsfunksjonane til syn. Trengs ikkje. Tas hand om i konstruktor..
	//synapse* pTestLangrar =  // er lik peikeren lagd i new-statement under...
	
	new synapse (&nA, &nD);




	// void*(void*) - funksjon. Her går kalkuleringa av listene..	
//	arbeidsKoeArbeider(0);

	

	cout<<"\n\n\nSkriver ut alle synapser til nokre neuron:\n";
	cout 	<<nA <<"\n"
		<<nD <<"\n";

	
	cout<<"\n\n\nGjennomgang av ulike verdier av kappa:\n\n";

	for(float f = 5000; f>600; f*=0.95 ) //Tar vekk 5% av f
	{
		nA.neuroParam.nyKappa(f);
		//nA.kappaParam.kalkulerPeriode();
		cout 	<<nA.neuroParam ;
	}

	cout << "\n\t\tJEJEJEJ\n\n\n";

	cout << "\n\t\tOg alpha-endring:\n\n";

	nA.neuroParam.nyKappa( 3000 );
	for(float a = 0.001; a<0.015; a*=1.12)
	{
	 	nA.neuroParam.nyAlpha(a);
		cout 	<<nA.neuroParam;
	}



	cout<<"\n\t\tEitt sekunds stillhet for INGEN SEGAULT!\n\n";
	sleep(1);

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

