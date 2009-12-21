#include "main.h"
#include "neuroEnhet.h"
#include "neuroSensor.h"

using std::list;


// "Arbeidsliste" - bl.a. for å holde styr på tid.
//list<synapse*> pNesteSynapseUtregningsKoe;

// Historieliste  - holder styr på historia til arbeidet (lista over)
list<arbeidsHistorieElement*> pArbeidsHistorieListe;

// alle frekvensstyrte neurosensore. Skal kontrolleres (i likhet med anna frekvensstyring) i synSkilleElement
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
		//ulTimestampForrigeSignal( ulTidsiterasjoner ),
		ulAntallSynapticVesiclesAtt  (DEF_startANTALL_SYN_V),
		ulSynapticVesicles_i_membran (0),
	 	fGlutamatReceptoreIPostsynMem( v ),
		//dOppladingsFartForSynVesicles(DEF_OPPLADINGSFART_FOR_SYN_VESICLES),
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
	ut<<"| " <<neuroArg.getNavn() <<"  | verdi: " <<neuroArg.nVerdiForDepolarisering <<" |     Med utsynapser:\n";
	


	for( std::vector<synapse*>::iterator iter = neuroArg.pUtSynapser.begin(); iter != neuroArg.pUtSynapser.end(); iter++ ){

	 	ut<<"\t" << (*iter)->ulAntallSynapticVesiclesAtt <<" antall syn.vesicles att.  TIL " <<(*iter)->pPostNode->navn <<endl;

	}

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



/* ******************************************************************************************************
// Spesielle overlagra funksjonen i synSkilleElement: (alt som skal gjøres en gang per tidsiterasjon..)
// 	- skal øke tid med 1.
// 	- sjekke om neurosensore skal fyre.
// 	- oppdatere synapser i lista for syn. i transient forløp.
 * ********                                                                                    **********
 * ******************************************************************************************************/ 
void synSkilleElement::aktiviserOgRegnUt()
{
	// legger meg til bakerst i arbeidskø. (for å holde på "iterasjoner" videre i arbeidskøa)
	synapse::pNesteSynapseUtregningsKoe.push_back( this );
	// iterer tid.
	ulTidsiterasjoner++;

	// utskrift:
	cout<<"\n\t\t\t\t\t\t\t\tØker tid til:\t" <<ulTidsiterasjoner <<" -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  \n\n";


	// Sjekker om frekvens-output-sensore skal fyre.
	for( list<neuroSensor*>::iterator i = pNeuroSensorListe.begin(); i != pNeuroSensorListe.end(); i++ ){
		(*i)->sjekkOmSkalFyres_ogFyr();
	}


	// Dersom nokre synapser ikkje har blitt fult opplada, skal de lades opp litt meir no (og evt. legges til i køa igjen, om meir gjenstår).
	/// TODO kjør oppdateringsliste. (de som ikkje er ferdig oppdatert, skal oppdateres en gang til..


	// ha en iterasjon i pNesteSynapseSomIkkjeErFerdigOppdatert_Koe... Kanskje ved å fjærne første ledd når er ferdig med det.
	cout<<"Før løkke. Returverdi fra funk: " <<pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front()->oppdater() <<endl;
	cout<<"Før løkke. 2 " <<endl;
 	while( pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front()->oppdater() == 1 ) {
	      cout<<"Inni løkke. je\n";
	} // Neste linje: tar også bort synSkilleElement ... 
	cout<<"Størrelse på kø: " <<pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.size();

	
	cout<<" => " <<pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.size();
	cout<<"Etter løkke.\n";
	/* ***
	 * Denne lista er også schedula med eit skilleelement. Dette skilleelementet
	 * har også overlagra oppdater(), som returnerer 1 ved normal drift, og 0 her.
	 * Dette gjør at while-setninga over er gyldig.
	 * ***/ 

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

	// sluppet trengs ikkje. Kan for så vidt ha lokal variabel her. Initielt var tanken å ha en liten treighet her. type MA..
	uAntallSynapticV_sluppet = 0.2 * ulAntallSynapticVesiclesAtt;
				//  | f.eks. *0.07 TODO Dette skal være en variabel, som kan også endres ved LTP (membranareal kan øke).
	
	cout 	<<" Sendte " <<uAntallSynapticV_sluppet <<" syn.V. " 
		<<"\t(av " <<ulAntallSynapticVesiclesAtt <<")"
	        <<"\t->  " <<pPostNode->navn 
		//<<"\t og referansepkt. for antall s.V. " <<ulAntallSynV_setpunkt; //<<"\n"
		;
	if( bInhibitorisk_effekt ) cout<<" (inhibitorisk)\n"; else cout<<" (eksitatorisk)\n";

	/* Ikkje naudsynt. sluppet = 0.07 * antallAtt (sjå nokre linjer opp..
	// forsikrer meg om at ulAntallSynapticVesiclesAtt ikkje blir negativ:
	if( ulAntallSynapticVesiclesAtt - uAntallSynapticV_sluppet < 0 ){
		uAntallSynapticV_sluppet = ulAntallSynapticVesiclesAtt; 
	}*/ 

	// og trekker fra de brukte syn.vesicles fra ulSynapticVesiclesAtt.
	ulAntallSynapticVesiclesAtt -= uAntallSynapticV_sluppet;

	// Skriver det merger dei inn i membran:
	ulSynapticVesicles_i_membran += uAntallSynapticV_sluppet;	


	unsigned uTempPostsynEffekt = uAntallSynapticV_sluppet * fGlutamatReceptoreIPostsynMem;
	/******* sender inn *******/ // if-testen er til for å sjå returverdien til sendInnPosts..()  (om postsyn. fyrer)
	//int nPostsynDepolarisering; <-XXX for seinare fikling med LTP
	// Sjekker om denne synapsen har inhibitorisk effekt, i såfall send inn negativt signal.
	if( bInhibitorisk_effekt )
	    //nPostsynDepolarisering = 							// arg rett under her er int.
	        pPostNode->sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( (-1) * uTempPostsynEffekt	);
	else // ikkje inhibitorisk, fortsetter med eksitatorisk (pos. signal inn i postsyn. neuron).
	    //nPostsynDepolarisering = 
		pPostNode->sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal(        uTempPostsynEffekt );
	
	
	// Legger til synapse i pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.
	synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.push_back( this );
	
	/******* oppdaterer timestamp for tidspkt for signal ********/
	//ulTimestampForrigeSignal = ulTidsiterasjoner;
}		

// vim:fdm=marker:fmr=//{,//} : fdl=3
