//KANN/SANN 
#include "main.h"
#include "neuron.h"
#include "neuroSensor.h"
#include "synapse.h"

#include <sstream>

using std::list;


// "Arbeidsliste" - bl.a. for å holde styr på tid.
//list<synapse*> pNesteSynapseUtregningsKoe;

// Historieliste  - holder styr på historia til arbeidet (lista over)
//list<arbeidsHistorieElement*> pArbeidsHistorieListe;

// alle frekvensstyrte neurosensore. Skal kontrolleres (i likhet med anna frekvensstyring) i synSkilleElement
list<neuroSensor*> pNeuroSensorListe;


//tester. Slett:
//vector<synapse*> slett_vpAlleSynapeneSomErContruert;






// TID :
unsigned long ulTidsiterasjoner = 0;




/********************************
***                           ***
***   funksjonsdefinisjoner   ***
***                           ***
********************************/

// constructor:
synapse::synapse( neuron* pPreN_arg, neuron* pPostN_arg, bool argInhibitorisk_effekt /*=false*/, float vekt /*=1*/ ) : //vekt bør være i promille.
														// 	og kanskje type int eller long.
		bInhibitorisk_effekt(argInhibitorisk_effekt),
	 	fSynaptiskVekt( vekt ),
		//ulTimestampForrigeEndingAvParam( ulTidsiterasjoner ),
		pPreNode(pPreN_arg), pPostNode(pPostN_arg)
{
	//{
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

	
	//}
}

/***************************
*** Utskriftsprosedyrer: ***
***************************/
std::ostream & operator<< (std::ostream & ut, neuron neuroArg )
{ //{
	ut<<"| " <<neuroArg.getNavn() <<"  | verdi: " <<neuroArg.nVerdiForDepolarisering <<" \t|\tMed utsynapser:\n";
	
	// Innsynapser:
	for( std::vector<synapse*>::iterator iter = neuroArg.pInnSynapser.begin(); iter != neuroArg.pInnSynapser.end(); iter++ ){
	 	ut 	<<"\t" <<(*iter)->pPreNode->navn <<" -> " <<neuroArg.navn <<"\t|" <<endl;
	}

	// Utsynapser:
	for( std::vector<synapse*>::iterator iter = neuroArg.pUtSynapser.begin(); iter != neuroArg.pUtSynapser.end(); iter++ ){
	 	ut 	<<"\t\t\t|\t" <<neuroArg.navn <<" -> " <<(*iter)->pPostNode->navn <<endl;
	}


	return ut;
} //}

std::ostream & operator<< (std::ostream & ut, neuron* pNeuroArg )
{ //{
	ut<<"jeje " <<(*pNeuroArg);
	return ut;
} //}

std::ostream & operator<< (std::ostream & ut, synapse synArg )
{ //{
	std::string type;
	if( synArg.bInhibitorisk_effekt ) type = "inhibitorisk"; else type="eksitatorisk";

	ut 	<<*(synArg.pPreNode) <<" synapser til " <<*(synArg.pPostNode) <<"(" <<type <<") med vekt bestemt i postsyn mem. ikkje ferdig."  ;
	return ut;
} //}

std::ostream & operator<< (std::ostream & ut, synSkilleElement synSkileE_arg ){ //{
	ut<<"synSkilleElement.\n";
	return ut;
} //}

/* ******************************************************************************************************
// aktiviserOgRegnUt() :
// 	Spesielle overlagra funksjon i synSkilleElement: (alt som skal gjøres en gang per tidsiterasjon..)
// 	  - skal øke tid med 1.
// 	  - sjekke om neurosensore skal fyre.
// 	  - oppdatere synapser i lista for syn. i transient forløp.
 * ******************************************************************************************************/ 
inline void synSkilleElement::aktiviserOgRegnUt()
{ //{  ... }
	// legger meg til bakerst i arbeidskø. (for å holde på "iterasjoner" videre i arbeidskøa)
	/*synSkilleElement::*/pNesteSynapseUtregningsKoe.push_back( this );
	
	// - - - - - - - - synSkilleElement flytta til slutten, igjen, alle nye jobber kommer på neste tidsteg, igjen.. - - - - - - - - - - - - - - - -


	/* * * * Oppdatering, etter estimert-liste * * * */

	// neuron* pNeuronTemp;  	for oppdaterNeuron() under..

	// returnerer 0 når verdig:
	//while( (pNeuronTemp = neuron::s_get_npNesteNeuron_med_oppdatering_neste_tid()) != 0){
	 	// kaller oppdaterNeuron() for det neuronet:
//		cDebug<<"\n\n\t\t\t\tFann neuron som skal oppdateres!\t\tXXX\n\n";
//		pNeuronTemp->oppdaterNeuron();
		//sleep(1);
//	}

	


	// ha en iterasjon i pNesteSynapseSomIkkjeErFerdigOppdatert_Koe... Kjører gjennom alle som ikkje er oppdatert, og oppdaterer
 	//while( pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front()->oppdaterSyn() != 0 ) ; // Merk; ;;;




	// iterer tid.
	ulTidsiterasjoner++;

	// - - - - - - - - - ny tid - - - - - - - - - - - - -
	
	// utskrift:
	cout<<"\t\t\t\t\t\t\t\tØker tid til:\t" <<ulTidsiterasjoner <<" -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  \n";


	// Sjekker om frekvens-output-sensore skal fyre.
	// Kanksje eg skal ordne fyring på denne måten?
	/*for( list<neuroSensor*>::iterator i = pNeuroSensorListe.begin(); i != pNeuroSensorListe.end(); i++ ){
		(*i)->sjekkOmSkalFyres_ogFyr();
	}*/


} //}



synapse::~synapse()
{ //{ ... }
	bool bPreOk  = false;
	bool bPostOk = false;


	// Sjekker om der er utsynapser i prenode:
	if( ! (pPreNode->pUtSynapser).empty() ){
		cout<<"\tså skriver eg ut alle utsynapsene fra prenode:\n";
		for( std::vector<synapse*>::iterator iter = pPreNode->pUtSynapser.begin(); iter != pPreNode->pUtSynapser.end() ; iter++ ){
			cout<<"\t\tUtsynapse fra " <<(*iter)->pPreNode->navn <<" til " <<(*iter)->pPostNode->navn <<"\t->\n";
		}
	}else{ 
		cout<<"\tprenode  er Tom for utsynapser.\n"; 
		//bPreOk = true;
	}
	// Sjekker om det er innsynapser i postnode:
	if( ! pPostNode->pInnSynapser.empty() ){
		cout<<"\tså skriver eg ut alle innsynapsene til postnode:\n";
		 
		for( std::vector<synapse*>::iterator iter = pPostNode->pInnSynapser.begin(); iter != pPostNode->pInnSynapser.end() ; iter++ ){
			cout<<"\t\tInnynapse fra " <<(*iter)->pPreNode->navn <<" til " <<(*iter)->pPostNode->navn <<"\t<-\n";
		}
	}else{ 
		cout<<"\tpostnode er Tom for innsynapser.\n";
		//bPostOk = true;
	}


	if( !bPreOk ){
		//fjærner seg sjølv fra prenode:
		for( std::vector<synapse*>::iterator iter = pPreNode->pUtSynapser.begin(); iter != pPreNode->pUtSynapser.end() ; iter++ ){
			if( *iter == this ){
				cout<<"\t~synapse: Fjærner ut-syn. fra prenode: [" <<pPreNode->navn <<"] "; //Merk: ingen endl;
				(pPreNode->pUtSynapser).erase( iter );
				bPreOk = true;
				break;
			}
		}
	}
	if( !bPostOk ){
		//fjærner seg sjølv fra postnode:
		for( std::vector<synapse*>::iterator iter = pPostNode->pInnSynapser.begin(); iter != pPostNode->pInnSynapser.end() ; iter++ ){
			if( *iter == this ){
				cout<<"\t og videre inn-syn. til postnode [" <<pPostNode->navn <<"]\n";
				(pPostNode->pInnSynapser).erase( iter );
				bPostOk = true;
				break;
			}
		}
	}

	if( (!bPreOk) || (!bPostOk) ){
		/// FEIL:
		std::cerr<<"\n\n\n\n\nFEIL FEIL FEIL!\nSjekk neuroEnhet.cpp #asdf250. Feil i synapse destruksjon. (~synapse )\n";
		std::cerr<<"I test om eg vart sletta  fra presyn. neuron og postsyn. neuron: failed\t\t" 
			<<"bPreOk (" <<pPreNode->navn <<"):" <<bPreOk <<"  ->  bPostOk (" <<pPostNode->navn <<"): " <<bPostOk 
			<<"\n(Eg er ikkje sletta fra det aktuelle neuronet (dei med verdi 0)\n";
		std::cerr<<"Eg ligger mellom (neuron: presyn - postsyn): " <<pPreNode->navn <<" - " <<pPostNode->navn <<endl;
		exit(-9);	
	}
	cout<<"\tO.K.  -  syn. " <<pPreNode->navn <<" til " <<pPostNode->navn <<" er ikkje lenger\n";




	//{ utskriftslogger
	/*if( ulTimestampForrigeEndingAvParam < 10 ){
	 	utskriftsFilLogg <<"0 0 0 0"; // for å unngå feilmelding om tom vektor, fra octave når det skal plottes..
	}*/

	/* / no er data slik: [tid, antall, 		tomme, membran ] i synapse_likevekt..
	utskriftsFilLogg<<"];\n"
				<<"plot( data([1:end-1],1), data([1:end-1],2), \";Frie  S.V.;\",\n\t"
	     				<<"data([1:end-1],1), data([1:end-1],3), \";tomme S.V.;\",\n\t" 
					<<"data([1:end-1],1), data([1:end-1],4), \";mem.  S.V.;\""
					<<");\n"
			<<"title \"Synaptic vesicles i presyn. terminal i synapse: " <<pPreNode->navn <<" -> " <<pPostNode->navn <<"\"\n"
			<<"xlabel Tid\n" <<"ylabel Antall\n"
			<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
			<<"print(\'eps_" <<pPreNode->navn <<"->" <<pPostNode->navn <<".eps\', \'-deps\');"
			//<<"print(\'pdf_" <<pPreNode->navn <<"->" <<pPostNode->navn <<".pdf\', \'-dpdf\');"
			<<" sleep(9); ";
	utskriftsFilLogg.close();
	*/
	//}
} //}


/*inline*/ void synapse::kalkulerKappa_ij()
{
	///*kappa_ij*/ parametre.kappa = fSynaptiskVekt / pPreNode->neuroParam.getPeriode();
}


// vim:fdm=marker:fmr=//{,//} : fdl=3
