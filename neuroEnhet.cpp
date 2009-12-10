#include "main.h"
#include "neuroEnhet.h"
#include "neuroSensor.h"

using std::list;


// "Arbeidsliste" - bl.a. for å holde styr på tid.
list<synapse*> pNesteSynapseUtregningsKoe;

// Historieliste  - holder styr på historia til arbeidet (lista over)
list<arbeidsHistorieElement*> pArbeidsHistorieListe;

// alle frekvensstyrte neurosensore. Skal kontrolleres (i likhet med anna frekvensstyring) i tidsSkilleElement
list<neuroSensor*> pNeuroSensorListe;





// TID :
unsigned long ulTidsiterasjoner = 0;




/********************************
***                           ***
***   funksjonsdefinisjoner   ***
***                           ***
********************************/
/* Dersom man driver med switching av to synapse-kø-peikere.
void switchAktivArbeidsKoe_og_itererTid()
{
	// bytter aktiv kø, og sidekø:
	
	if( pAktivArbeidsKoe_synapseP == &arbeidsKoe_for_utregningAvNeuralnett_nesteSynapseP__A )
	{
		pAktivArbeidsKoe_synapseP = &arbeidsKoe_for_utregningAvNeuralnett_nesteSynapseP__B;
	//	std::cout<<"pAktivArbeidsKoe_synapseP er no  B  \n";
		pSideKoe_Inaktiv_synapseP = &arbeidsKoe_for_utregningAvNeuralnett_nesteSynapseP__A; 
	}else{
		pAktivArbeidsKoe_synapseP = &arbeidsKoe_for_utregningAvNeuralnett_nesteSynapseP__A;
	//	std::cout<<"pAktivArbeidsKoe_synapseP er no  A  \n";
		pSideKoe_Inaktiv_synapseP = &arbeidsKoe_for_utregningAvNeuralnett_nesteSynapseP__B; 
	}


	// Itererer tid ( ulTidsiterasjoner ):
	
	ulTidsiterasjoner ++;	
}*/

// hjelpefunskjon for konstruksjon av neuron:
void neuron::leggTilSynapse( neuron* nyttUtNeuron, double vekt)
{ 
	pUtSynapser.push_back( new synapse( this, nyttUtNeuron, vekt) ); 
}


/***************************
*** Utskriftsprosedyrer: ***
***************************/
std::ostream & operator<< (std::ostream & ut, neuron neuroArg )
{
	ut<<"| " <<neuroArg.getNavn() <<"  | ";
	return ut;
}

std::ostream & operator<< (std::ostream & ut, synapse synArg )
{
	ut 	<<*(synArg.pPreNode) <<" synapser til " <<*(synArg.pPostNode) <<" med vekt bestemt i postsyn mem. ikkje ferdig."  
		<<"    Antall syn.vesicles att: " <<synArg.ulAntallSynapticVesiclesAtt <<std::endl;
	return ut;
}

// Spesielle overlagra funksjonen i tidsSkilleElement: (alt som skal gjøres en gang per tidsiterasjon..)
void tidsSkilleElement::regnUt()
{
	// legger meg til bakerst i arbeidskø. (for å holde på "iterasjoner" videre i arbeidskøa)
	pNesteSynapseUtregningsKoe.push_back( this );
	// iterer tid.
	ulTidsiterasjoner++;

	// Evt andre ting...
	cout<<"\t\t\t\t\t\t\tØker tid til:\t" <<ulTidsiterasjoner <<"\n";

	// Sjekker om frekvens-output-sensore skal fyre.
	for( list<neuroSensor*>::iterator i = pNeuroSensorListe.begin(); i != pNeuroSensorListe.end(); i++ ){
		(*i)->sjekkOmSkalFyres_ogFyr();
	}

	/*
		Kontroller frekvens på ulike synapser, for å holde styr på LTP/LTD, neuropeptid, osv..
			- overfør utførte "jobber" fra pNesteSynapseUtregningsKoe til pJobbHistorie
				- skal ha historie, og historie-historie (iterativ historie) for å begrense arbeid (trenger ikkje regne ut
				alle i historia..)
				- desse bør kanskje være av spesiell klasse. Skal ha timestamp, for når de var utregna med aktuelle frekvens,
				for å kunne regne ut iterativt ny frekvens etter ei stund. Køa kan dermed være bare n element stor, der
				n er antall neuron. 
				- kva betyr frekvens i dette tilfelle? "momentan frekvens" gir ikkje meining. Må finne på ei meining/funksjon
				
		Anna..
	*/
}

void synapse::regnUt()
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

	unsigned long ulTempSynSignal_antallSV = 0.1 * ulAntallSynapticVesiclesAtt;
						// | f.eks. *0.1
	
	cout 	<<"Sendte " <<ulTempSynSignal_antallSV <<" syn.V. til " <<pPostNode->navn 
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

// vim:fdm=marker:fmr=//{,//} : fdl=3
