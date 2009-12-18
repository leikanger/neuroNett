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

// hjelpefunskjon for konstruksjon av neuron:
//
void neuron::leggTilSynapse( neuron* nyttUtNeuron, bool inhib_e, float vekt)
{ 
	cout<<"neuroEnhet.cpp l.37 IKKJE I BRUK LENGER\n";
	exit(-1);
	pUtSynapser.push_back( new synapse( this, nyttUtNeuron, inhib_e, vekt) ); 
}


// constructor:
synapse::synapse( neuron* pPreN_arg, neuron* pPostN_arg, bool argInhibitorisk_effekt /*=false*/, float v /*=1*/ ) :  // v er oppgitt i promille.
		bInhibitorisk_effekt(argInhibitorisk_effekt),
		ulTimestampForrigeSignal( ulTidsiterasjoner ),
		ulAntallSynapticVesiclesAtt  (DEF_startANTALL_SYN_V),
	 	fGlutamatReceptoreIPostsynMem( v ),
		dOppladingsFartForSynVesicles(DEF_OPPLADINGSFART_FOR_SYN_VESICLES),
		ulAntallSynV_setpunkt 	     (DEF_startANTALL_SYN_V),
		pPreNode(pPreN_arg), pPostNode(pPostN_arg)
{
	pPreNode->pUtSynapser.push_back(  this );
	pPostNode->pInnSynapser.push_back(this );
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
	std::string type;
	if( synArg.bInhibitorisk_effekt ) type = "inhibitorisk"; else type="eksitatorisk";

	ut 	<<*(synArg.pPreNode) <<" synapser til " <<*(synArg.pPostNode) <<"(" <<type <<") med vekt bestemt i postsyn mem. ikkje ferdig."  
		<<"    Antall syn.vesicles att: " <<synArg.ulAntallSynapticVesiclesAtt <<std::endl;
	return ut;
}

// Spesielle overlagra funksjonen i tidsSkilleElement: (alt som skal gjøres en gang per tidsiterasjon..)
void tidsSkilleElement::aktiviserOgRegnUt()
{
	// legger meg til bakerst i arbeidskø. (for å holde på "iterasjoner" videre i arbeidskøa)
	pNesteSynapseUtregningsKoe.push_back( this );
	// iterer tid.
	ulTidsiterasjoner++;

	// Evt andre ting...
	cout<<"\n\t\t\t\t\t\t\tØker tid til:\t" <<ulTidsiterasjoner <<"\n";



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

void synapse::aktiviserOgRegnUt()
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

	unsigned long ulTempSynSignal_antallSV = 0.07 * ulAntallSynapticVesiclesAtt;
						//  | f.eks. *0.07 TODO Dette skal være en variabel, som kan også endres ved LTP (membranareal kan øke).
	
	cout 	<<" Sendte " <<ulTempSynSignal_antallSV <<" syn.V. " 
		<<"\t(av gjenverande " <<ulAntallSynapticVesiclesAtt 
	        <<")\t->  " <<pPostNode->navn 
		<<"\t og referansepkt. for antall s.V. " <<ulAntallSynV_setpunkt; //<<"\n";
	if( bInhibitorisk_effekt ) cout<<" (inhibitorisk)\n"; else cout<<" (eksitatorisk)\n";

	// og trekker fra de brukte syn.vesicles fra ulSynapticVesiclesAtt.
	ulAntallSynapticVesiclesAtt -= ulTempSynSignal_antallSV;

	/******* oppdaterer timestamp for tidspkt for signal ********/
	ulTimestampForrigeSignal = ulTidsiterasjoner;

	/******* sender inn *******/ // if-testen er til for å sjå returverdien til sendInnPosts..()  (om postsyn. fyrer)
	//int nPostsynDepolarisering;
	// Sjekker om denne synapsen har inhibitorisk effekt, i såfall send inn negativt signal.
	if( bInhibitorisk_effekt )
	    //nPostsynDepolarisering = 							// arg rett under her er int.
	        pPostNode->sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( (-1) * ulTempSynSignal_antallSV*fGlutamatReceptoreIPostsynMem );
	else // ikkje inhibitorisk, fortsetter med eksitatorisk (pos. signal inn i postsyn. neuron).
	    //nPostsynDepolarisering = 
		pPostNode->sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( ulTempSynSignal_antallSV*fGlutamatReceptoreIPostsynMem );

	// TODO gjør ferdig if-testen for LTP og homoLTD. ( if( nPostsynDepolarisering>?? ) LTP..
	
}		

// vim:fdm=marker:fmr=//{,//} : fdl=3
