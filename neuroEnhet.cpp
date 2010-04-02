#include "main.h"
#include "neuron.h"
#include "neuroSensor.h"
// To alternative framgangsmåter for synapsene. Synapse.h er bra for rask utregning, synapse_likevekt-klassa er for utprøving av teori, og lovende for plasti
#include "synapse.h"
#include "synapse_likevekt.h" 

#include <sstream>

using std::list;


// "Arbeidsliste" - bl.a. for å holde styr på tid.
//list<synapse*> pNesteSynapseUtregningsKoe;

// Historieliste  - holder styr på historia til arbeidet (lista over)
list<arbeidsHistorieElement*> pArbeidsHistorieListe;

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
synapse::synapse( neuron* pPreN_arg, neuron* pPostN_arg, bool argInhibitorisk_effekt /*=false*/, float v /*=1*/ ) :  // v er oppgitt i promille.
		bInhibitorisk_effekt(argInhibitorisk_effekt),
		//uAntallSynapticV_sluppet(0),
		nBestilltSynteseAvSVFraForrigeIter(0),
		nBestilltReproduksjonAvSvFraForrigeIter(0),
		ulTimestampForrigeOppdatering( ulTidsiterasjoner ),
		ulTimestampForrigeSignal 	(ulTidsiterasjoner ),
		ulAntallSynapticVesiclesAtt  (DEF_startANTALL_SYN_V),
		ulSynapticVesicles_i_membran (0),
	 	fGlutamatReceptoreIPostsynMem( v ),
		//dOppladingsFartForSynVesicles(DEF_OPPLADINGSFART_FOR_SYN_VESICLES),
		ulAntallSynV_setpunkt 	     (DEF_startANTALL_SYN_V),
		pPreNode(pPreN_arg), pPostNode(pPostN_arg)
{
	//{
	pPreNode->pUtSynapser.push_back(  this );
	pPostNode->pInnSynapser.push_back(this );
	

	//Tester. Slett:
	//slett_vpAlleSynapeneSomErContruert.push_back( this );


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

	// Logg for størrelsen av PSP-endring:
	std::ostringstream tempFilAdr2;
	tempFilAdr2<<"./datafiler_for_utskrift/effekt_av_PSP_for_syn_" <<pPreNode->navn <<"-"  <<pPostNode->navn ;
//	if(bInhibitorisk_effekt){ tempFilAdr2<<"_inhi"; }
// 	else{ 			  tempFilAdr2<<"_eksi"; }
	tempFilAdr2<<".oct";

	tempStr = tempFilAdr2.str() ;

	// trenger c-style string for open():
	PSP_loggFil.open( tempStr.c_str() );
	
	PSP_loggFil<<"data=[\n";
	PSP_loggFil.flush();
	
	//}
}

synapse_likevekt::synapse_likevekt( neuron* pPreN_arg, neuron* pPostN_arg, bool argInhibitorisk_effekt /*=false*/, float v /*=1*/ )
	: synapse(pPreN_arg, pPostN_arg, argInhibitorisk_effekt , v ), 
	nRaavarer_til_SV_syntese(50),
	uSynapticVesicles_setpunkt_for_membran(  ulAntallSynapticVesiclesAtt * DEF_FORHOLD_FRI_VS_MEM_SV )
	{
		//{
		ulSynapticVesicles_i_membran = uSynapticVesicles_setpunkt_for_membran ;

		nSynapticSkall_tommeSV = ulAntallSynapticVesiclesAtt * 0.05;

		cout<<"\nLager likevekts-synapse fra " <<pPreNode->getNavn() <<" til " <<pPostNode->getNavn()
			<<", med:\t\tS.V. i mem.:\t" <<ulSynapticVesicles_i_membran <<"\t, frie S.V.: "
			<<ulAntallSynapticVesiclesAtt <<" osv.\n";

		// mens eg driver å utvikler, legger eg den til, automagisk.
		synapse::pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.push_front(this);

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
			//<< (*iter)->ulAntallSynapticVesiclesAtt <<" antall syn.vesicles att.  TIL "
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

	ut 	<<*(synArg.pPreNode) <<" synapser til " <<*(synArg.pPostNode) <<"(" <<type <<") med vekt bestemt i postsyn mem. ikkje ferdig."  
		<<"    Antall syn.vesicles att: " <<synArg.ulAntallSynapticVesiclesAtt <<std::endl;
	return ut;
} //}

std::ostream & operator<< (std::ostream & ut, synSkilleElement synSkileE_arg ){ //{
	ut<<"synSkilleElement.\n";
	return ut;
} //}
/*
std::ostream & operator<< (std::ostream & ut, synapse* pSynArg){
	 ut<<(*pSynArg);
	 return ut;
}*/


/* ******************************************************************************************************
// Spesielle overlagra funksjonen i synSkilleElement: (alt som skal gjøres en gang per tidsiterasjon..)
// 	- skal øke tid med 1.
// 	- sjekke om neurosensore skal fyre.
// 	- oppdatere synapser i lista for syn. i transient forløp.
 * ********                                                                                    **********
 * ******************************************************************************************************/ 
inline void synSkilleElement::aktiviserOgRegnUt()
{ //{  ... }
	// legger meg til bakerst i arbeidskø. (for å holde på "iterasjoner" videre i arbeidskøa)
	synapse::pNesteSynapseUtregningsKoe.push_back( this );
	
	// - - - - - - - - synSkilleElement flytta til slutten, igjen, alle nye jobber kommer på neste tidsteg, igjen.. - - - - - - - - - - - - - - - -


	// Det over skal endres. Holder på med nytt system for frekvens-egenfyring. Dette involverer estimat for fyring (for å optimalisere).
	//{inni her

	// Tester det: XXX
	//neuron::sNesteFyringForNeuron[&slettA] = 999;
	neuron::sSkrivUt_sNesteFyringForNeuron_map();
	//}

	
	/* //{ UTKOMMENTERT
	// Static fordi denne brukes så ofte. Optimalisering.	
	static std::map<neuron*, unsigned>::iterator iter_sNeste;
	iter_sNeste = neuron::sNesteFyringForNeuron.begin();
	// ****************************************

	
	if( ! neuron::sNesteFyringForNeuron.empty() ) {
	
		while( ++iter_sNeste != neuron::sNesteFyringForNeuron.end() )
			cout<<"jepp " <<(*iter_sNeste).first <<endl;  	
	}

	if( ! neuron::sNesteFyringForNeuron.empty() ){ // <- Inger innvirkning..
	//if( neuron::sNesteFyringForNeuron.size() == 0) cout<<"Size == 0\n\n";
		// Veit ikkje kvifor, men for funka ikkje. Dette funker perfekt:
		while( ++iter_sNeste != neuron::sNesteFyringForNeuron.end() ){
			cout<<"jepp " <<(*iter_sNeste).first <<endl;
		}
	}
	*/ //}


	/* * * * Oppdatering, etter estimert-liste * * * */

	/*static?*/ neuron* pNeuronTemp;

	// returnerer 0 når verdig:
	while( (pNeuronTemp = neuron::s_get_npNesteNeuron_med_oppdatering_neste_tid()) != 0){
	 	// kaller oppdaterNeuron() for det neuronet:
		cDebug<<"\n\n\t\t\t\tFann neuron som skal oppdateres!\t\tXXX\n\n";
		pNeuronTemp->oppdaterNeuron();
		//sleep(1);
	}

	

	/*********************************************************************
	 * KORLEIS SKAL EG gjøre det når eg skal bestille ei ny fyring, på slutten av neste tids-steg?
	 * 	- legge til ei ny synapse som peiker på neuronet, sist i arbeidskø? --og slette den etterpå. Nei, for tidkrevande..
	 * 	- ha ei fast inn-synapse, som eg kalla hendel, eller noke. Denne hendelen, som er fast "synapse", (ekskludert fra inn-synapse-lista)
	 * 		legger eg til på slutten av arbeidsliste. Kanskje eg tilogmed skal lage ei spesial-klasse for denne, for å hindre LTP-kalkulering..
	 * 	- * eller bare "fyre" den, etter å oppjustert tida, og flytta tidsSkilleElement til slutten av lista. Da vil alle synapsene dens legges etter den.
	 *
	 */





	// Dersom nokre synapser ikkje har blitt fult opplada, skal de lades opp litt meir no (og evt. legges til i køa igjen, om meir gjenstår).
	///  kjør oppdateringsliste. (de som ikkje er ferdig oppdatert, skal oppdateres en gang til..






	// ha en iterasjon i pNesteSynapseSomIkkjeErFerdigOppdatert_Koe... Kanskje ved å fjærne første ledd når er ferdig med det.
 	while( pNesteSynapseSomIkkjeErFerdigOppdatert_Koe.front()->oppdaterSyn() == 1 ) ; // Merk; ;;;
	// Trur dette kjører X.front()->oppdaterSyn() intil den returnerer anna enn 1 (når synskilleelement dukker opp..)
	// Mao. gjør ferdig "iterasjonen".
	/* ***
	 * Denne lista er også schedula med eit skilleelement. Dette skilleelementet
	 * har også overlagra oppdaterSyn(), som returnerer 1 ved normal drift, og 0 i synskilleelement.
	 * Dette gjør at while-setninga over er gyldig (og jævlig smart!).
	 * ***/ 




	// iterer tid.
	ulTidsiterasjoner++;

	// - - - - - - - - - ny tid - - - - - - - - - - - - -
	
	// utskrift:
	cout<<"\t\t\t\t\t\t\t\tØker tid til:\t" <<ulTidsiterasjoner <<" -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  \n";


	// Sjekker om frekvens-output-sensore skal fyre.
	for( list<neuroSensor*>::iterator i = pNeuroSensorListe.begin(); i != pNeuroSensorListe.end(); i++ ){
		(*i)->sjekkOmSkalFyres_ogFyr();
	}


} //}




#define MAX_MEPP_MERGING 2

/***********************************************************
 * Denne er føkka. Ordne denne slik at den funker bra i nye framgansmåten min.
 *
 ***********************************************************/
void synapse::mergeSynapticVesicles( int antallArg /* =0 */ )
{ //{ ... }
	int nTempAntall;
	if( antallArg ){  if(antallArg>MAX_MEPP_MERGING)nTempAntall = MAX_MEPP_MERGING;
				else					nTempAntall = antallArg; 	}
	else 		  	nTempAntall = uAntallSynapticV_sluppet;

	//cDebug<<"nTempAntall i synapse::mergeSynapticVesicles(): " <<nTempAntall <<endl;
	//cDebug<<"og uAntallSynapticV_sluppet: \t\t\t" <<uAntallSynapticV_sluppet <<endl;

	// og trekker fra de brukte syn.vesicles fra ulSynapticVesiclesAtt.
	ulAntallSynapticVesiclesAtt -= nTempAntall;

	// Skriver det merger dei inn i membran:
	ulSynapticVesicles_i_membran += nTempAntall;	


	unsigned uTempPostsynEffekt = nTempAntall * fGlutamatReceptoreIPostsynMem;
	//****** sender inn ******* // if-testen er til for å sjå returverdien til sendInnPosts..()  (om postsyn. fyrer)
	//(ikkje implementert enda..)
	//int nPostsynDepolarisering; <-XXX for seinare fikling med LTP
	// Sjekker om denne synapsen har inhibitorisk effekt, i såfall send inn negativt signal.
	if( bInhibitorisk_effekt )  // XXX
						// nPostsynDepolarisering !
	    //nPostsynDepolarisering = 							// arg rett under her er int.
		pPostNode->sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( (-1) * uTempPostsynEffekt	);
	else // ikkje inhibitorisk, fortsetter med eksitatorisk (pos. signal inn i postsyn. neuron).
	    //nPostsynDepolarisering = 
		pPostNode->sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal(        uTempPostsynEffekt );



	// Kaller LTD-effekt (med argumentet - korMangeSVsomErMerga)
	// Eventuell LTP skjer dersom postsyn. fyrer. Da vil LTP kalles fra postsyn. neuron.
	homoLTD( nTempAntall );
}//}

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
	
	if( ulTimestampForrigeOppdatering < 10 ){
	 	utskriftsFilLogg <<"0 0 0 0"; // for å unngå feilmelding om tom vektor, fra octave når det skal plottes..
	}

	// no er data slik: [tid, antall, 		tomme, membran ] i synapse_likevekt..
	utskriftsFilLogg<<"];\n"
				<<"plot( data([1:end-1],1), data([1:end-1],2), \";Frie  S.V.;\",\n\t"
	     				<<"data([1:end-1],1), data([1:end-1],3), \";tomme S.V.;\",\n\t" 
					<<"data([1:end-1],1), data([1:end-1],4), \";mem.  S.V.;\""
			/**///	<<",\n\tdata([1:end-1],1), data([1:end-1],5), \";raavarer;\"\n\t" //XXX For likevekts_synapse utskrift.
					<<");\n"
			<<"title \"Synaptic vesicles i presyn. terminal i synapse: " <<pPreNode->navn <<" -> " <<pPostNode->navn <<"\"\n"
			<<"xlabel Tid\n" <<"ylabel Antall\n"
			<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
			<<"print(\'eps_" <<pPreNode->navn <<"->" <<pPostNode->navn <<".eps\', \'-deps\');"
			//<<"print(\'pdf_" <<pPreNode->navn <<"->" <<pPostNode->navn <<".pdf\', \'-dpdf\');"
			<<" sleep(9); ";
	utskriftsFilLogg.close();
	
	// Og PSP_loggFil:
	PSP_loggFil <<"\t]; \nplot(data);\n"
			<<"title \"Effekt ved overfoering i forskjellige tidspunkt i synapse: " <<pPreNode->navn <<" -> " <<pPostNode->navn <<"\"\n"
			<<"xlabel Tid\n" <<"ylabel Potensiell_effekt\n"
			<<"sleep(10);";
	PSP_loggFil.close();
	//}
} //}



// vim:fdm=marker:fmr=//{,//} : fdl=3
