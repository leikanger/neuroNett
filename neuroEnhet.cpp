#include "main.h"
#include "neuroEnhet.h"
#include "neuroSensor.h"
#include "synapse.h"
#include <sstream>

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

/* hjelpefunskjon for konstruksjon av neuron: 	IKKJE LENGER NAUDSYNT. Deprecated.  	Nok å skrive    new synapse( &preNode, &postNode); så blir alt fiksa
 * 											  i constructor i synapse. Synapsa blir også rydda opp i ~neuron.
 * 											  	(gå gjerne over dette, og sjekk)
//
void neuron::leggTilSynapse( neuron* nyttUtNeuron, bool inhib_e, float vekt)
{ 
	co ut<<"neuroEnhet.cpp l.37 IKKJE I BRUK LENGER\n";
	exit(-1);
	pUtSynapser.push_back( new synapse( this, nyttUtNeuron, inhib_e, vekt) ); 
}*/


// constructor:
synapse::synapse( neuron* pPreN_arg, neuron* pPostN_arg, bool argInhibitorisk_effekt /*=false*/, float v /*=1*/ ) :  // v er oppgitt i promille.
		bInhibitorisk_effekt(argInhibitorisk_effekt),
		nBestilltSynteseAvSVFraForrigeIter(0),
		nBestilltReproduksjonAvSvFraForrigeIter(0),
		ulTimestampForrigeOppdatering( ulTidsiterasjoner ),
		ulAntallSynapticVesiclesAtt  (DEF_startANTALL_SYN_V),
		ulSynapticVesicles_i_membran (0),
	 	fGlutamatReceptoreIPostsynMem( v ),
		//dOppladingsFartForSynVesicles(DEF_OPPLADINGSFART_FOR_SYN_VESICLES),
		ulAntallSynV_setpunkt 	     (DEF_startANTALL_SYN_V),
		pPreNode(pPreN_arg), pPostNode(pPostN_arg)
{
	pPreNode->pUtSynapser.push_back(  this );
	pPostNode->pInnSynapser.push_back(this );
	

	// lag ei .oct - fil, og gjør klar for å kjøres i octave:
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiler_for_utskrift/synapse_" <<pPreNode->navn <<"-"  <<pPostNode->navn ;
	if(bInhibitorisk_effekt){ tempFilAdr<<"_inhi"; }
	else{ 			  tempFilAdr<<"_eksi"; }
	tempFilAdr<<".oct";

	std::string tempStr( tempFilAdr.str() );

	// trenger c-style string for open():
	utskriftsFilLogg.open( tempStr.c_str() );
	
	utskriftsFilLogg<<"data=[\n";
	
	utskriftsFilLogg.flush();
}


/***************************
*** Utskriftsprosedyrer: ***
***************************/
std::ostream & operator<< (std::ostream & ut, neuron neuroArg )
{ //{
	ut<<"| " <<neuroArg.getNavn() <<"  | verdi: " <<neuroArg.nVerdiForDepolarisering <<" |     Med utsynapser:\n";
	
	for( std::vector<synapse*>::iterator iter = neuroArg.pUtSynapser.begin(); iter != neuroArg.pUtSynapser.end(); iter++ ){
	 	ut<<"\t" << (*iter)->ulAntallSynapticVesiclesAtt <<" antall syn.vesicles att.  TIL " <<(*iter)->pPostNode->navn <<endl;
	}

	return ut;
} //}

std::ostream & operator<< (std::ostream & ut, synapse synArg )
{ //{
	std::string type;
	if( synArg.bInhibitorisk_effekt ) type = "inhibitorisk"; else type="eksitatorisk";

	ut 	<<*(synArg.pPreNode) <<" synapser til " <<*(synArg.pPostNode) <<"(" <<type <<") med vekt bestemt i postsyn mem. ikkje ferdig."  
		<<"    Antall syn.vesicles att: " <<synArg.ulAntallSynapticVesiclesAtt <<std::endl;
	return ut;
} //}

std::ostream & operator<< (std::ostream & ut, synSkilleElement synSkileE_arg ){ //{
	ut<<"synSkilleElement.\n";
	return ut;
} //}


/* ******************************************************************************************************
// Spesielle overlagra funksjonen i synSkilleElement: (alt som skal gjøres en gang per tidsiterasjon..)
// 	- skal øke tid med 1.
// 	- sjekke om neurosensore skal fyre.
// 	- oppdatere synapser i lista for syn. i transient forløp.
 * ********                                                                                    **********
 * ******************************************************************************************************/ 
void synSkilleElement::aktiviserOgRegnUt()
{ //{
	// legger meg til bakerst i arbeidskø. (for å holde på "iterasjoner" videre i arbeidskøa)
	synapse::pNesteSynapseUtregningsKoe.push_back( this );
	// iterer tid.
	ulTidsiterasjoner++;

	// utskrift:
	cout<<"\n\t\t\t\t\t\t\t\tØker tid til:\t" <<ulTidsiterasjoner <<" -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  \n";


	// Sjekker om frekvens-output-sensore skal fyre.
	for( list<neuroSensor*>::iterator i = pNeuroSensorListe.begin(); i != pNeuroSensorListe.end(); i++ ){
		(*i)->sjekkOmSkalFyres_ogFyr();
	}


	// Dersom nokre synapser ikkje har blitt fult opplada, skal de lades opp litt meir no (og evt. legges til i køa igjen, om meir gjenstår).
	/// TODO kjør oppdateringsliste. (de som ikkje er ferdig oppdatert, skal oppdateres en gang til..


	// ha en iterasjon i pNesteSynapseSomIkkjeErFerdigOppdatert_Koe... Kanskje ved å fjærne første ledd når er ferdig med det.
 	while( pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front()->oppdater() == 1 ) ;

	
	/* ***
	 * Denne lista er også schedula med eit skilleelement. Dette skilleelementet
	 * har også overlagra oppdater(), som returnerer 1 ved normal drift, og 0 her.
	 * Dette gjør at while-setninga over er gyldig.
	 * ***/ 

} //}

void synapse::aktiviserOgRegnUt()
{ //{
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
	uAntallSynapticV_sluppet = 0.2 * ulAntallSynapticVesiclesAtt; /// TODO TODO TODO sjå under.
				//  | f.eks. *0.07 TODO Dette skal være en variabel, som kan også endres ved LTP (membranareal kan øke).
	
	/*
	co ut 	<<" Sendte " <<uAntallSynapticV_sluppet <<" syn.V. " 
		<<"\t(av " <<ulAntallSynapticVesiclesAtt <<")"
	        <<"\t->  " <<pPostNode->navn 
		//<<"\t og referansepkt. for antall s.V. " <<ulAntallSynV_setpunkt; //<<"\n"
		;
	if( bInhibitorisk_effekt ) co ut<<" (inhibitorisk)\n"; else co ut<<" (eksitatorisk)\n";
	*/

	/* Ikkje naudsynt. sluppet = 0.07 * antallAtt (sjå nokre linjer opp..
	// forsikrer meg om at ulAntallSynapticVesiclesAtt ikkje blir negativ:
	if( ulAntallSynapticVesiclesAtt - uAntallSynapticV_sluppet < 0 ){
		uAntallSynapticV_sluppet = ulAntallSynapticVesiclesAtt; 
	}*/ 

	mergeSynapticVesicles(); // tar int-argument. Alternativt leser den ut fra klassevariabelen: uAntallSynapticV_sluppet (satt litt lenger opp.)
	
	// Legger til synapse i pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.
	synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.push_back( this );
	

	/******* oppdaterer timestamp for tidspkt for signal ********/
	//ulTimestampForrigeSignal = ulTidsiterasjoner;
} //}	

#define MAX_MEPP_MERGING 2

/***********************************************************
 * Denne er føkka. Ordne denne slik at den funker bra i nye framgansmåten min.
 *
 ***********************************************************/
void synapse::mergeSynapticVesicles( int antallArg /* =0 */ ){
	int nTempAntall;
	if( antallArg ){  if(antallArg>MAX_MEPP_MERGING)nTempAntall = MAX_MEPP_MERGING;
				else					nTempAntall = antallArg; 	}
	else 		  	nTempAntall = uAntallSynapticV_sluppet;

	// og trekker fra de brukte syn.vesicles fra ulSynapticVesiclesAtt.
	ulAntallSynapticVesiclesAtt -= nTempAntall;

	// Skriver det merger dei inn i membran:
	ulSynapticVesicles_i_membran += nTempAntall;	


	unsigned uTempPostsynEffekt = nTempAntall * fGlutamatReceptoreIPostsynMem;
	//****** sender inn ******* // if-testen er til for å sjå returverdien til sendInnPosts..()  (om postsyn. fyrer)
	//int nPostsynDepolarisering; <-XXX for seinare fikling med LTP
	// Sjekker om denne synapsen har inhibitorisk effekt, i såfall send inn negativt signal.
	if( bInhibitorisk_effekt )
	    //nPostsynDepolarisering = 							// arg rett under her er int.
	        pPostNode->sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( (-1) * uTempPostsynEffekt	);
	else // ikkje inhibitorisk, fortsetter med eksitatorisk (pos. signal inn i postsyn. neuron).
	    //nPostsynDepolarisering = 
		pPostNode->sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal(        uTempPostsynEffekt );
	
}

synapse::~synapse()
{ //{
	bool bPreOk  = false;
	bool bPostOk = false;


	//fjærner seg sjølv fra prenode:
	for( std::vector<synapse*>::iterator iter = pPreNode->pUtSynapser.begin(); iter != pPreNode->pUtSynapser.end() ; iter++ ){
	 	if( *iter == this ){
	       	pPreNode->pUtSynapser.erase( iter );
			bPreOk = true;
			break;
		}
	}
	//fjærner seg sjølv fra postnode:
	for( std::vector<synapse*>::iterator iter = pPostNode->pInnSynapser.begin(); iter != pPostNode->pInnSynapser.end() ; iter++ ){
	 	if( *iter == this ){
			pPostNode->pInnSynapser.erase( iter );
			bPostOk = true;
			break;
		}
	}

	if( (!bPreOk) || (!bPostOk) ){
		/// FEIL:
		std::cerr<<"\n\n\n\n\nFRIL FEIL FEIL!\nSjekk neuroEnhet.cpp ca. l.250. Feil i synapse destruksjon. (~synapse )\n\n\n\n";
		exit(-9);	
	}

	if( ulTimestampForrigeOppdatering < 10 ){
	 	utskriftsFilLogg <<"0 0 0 0"; // for å unngå feilmelding om tom vektor, fra octave når det skal plottes..
	}

	// no er data slik: [tid, antall, 		tomme, membran ] i synapse_likevekt..
	utskriftsFilLogg<<"];\n"
				<<"plot( data([1:end-1],1), data([1:end-1],2), \";Frie   S.V. i presyn. teminal;\",\n\t"
	     				<<"data([1:end-1],1), data([1:end-1],3), \";S.V. 2;\",\n\t" 
					<<"data([1:end-1],1), data([1:end-1],4), \";S.V. 3;\"\t);\n"
			<<"title \"Synaptic vesicles i presyn. terminal i synapse: " <<pPreNode->navn <<" -> " <<pPostNode->navn <<"\"\n"
			<<"xlabel Tid\n" <<"ylabel Antall\n"
			<<"akser=[0 data(end,1) 0 1300]; axis(akser);\n"
			<<"print(\'eps_" <<pPreNode->navn <<"->" <<pPostNode->navn <<".eps\', \'-deps\');"
			//<<"print(\'pdf_" <<pPreNode->navn <<"->" <<pPostNode->navn <<".pdf\', \'-dpdf\');"
			<<"sleep(9);";
	utskriftsFilLogg.close();

	/* opning av plot for synapsens syn.v. i octave:
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiler_for_utskrift/synapse_" <<pPreNode->navn <<"-"  <<pPostNode->navn;
	
	std::string tempStr("octave ");
	tempStr +=  tempFilAdr.str() ;

	system("tempStr.c_str()");
	co ut<<tempStr <<endl;
	sleep(4);
	*/
} //}



// vim:fdm=marker:fmr=//{,//} : fdl=3
