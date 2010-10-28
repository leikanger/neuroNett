//KANN/SANN

//{ Kladd:
/*
	Neste store utfordring er korleis gro eit neuralt nettverk, uten å fotfølge algoritmene?
	- En mulighet er å legge inn sted-koordinater i neuron, ELLER på annen måte dele de opp i gjupper (for kven som kan gro kor). Litt for detaljstyrende
	- lage eit grovt (ikkje detaljstyrende) overordna system, som bestemmer kva som skal gro med kven.
	- Kanskje prøve meg på eit system med atraktorer. Kan godt være veldig regnekrevande, siden det ikkje skal gro mens det kjører..
	 	(det kan godt stå og gro for seg sjølv, (offline) ei natt.. ) - derretter kommer normal læring inn...
	- online-endring gjøres kanskje med enkelte neuropeptid?
	
	- les korleis dette gjøres i kroppen.



*/
//}


/*
#include <iostream>
#include <iomanip>
#include <stdlib.h> 	// for exit()
#include <list> 	// for list
#include <vector>
#include <map>    // map
*/
#include "main.h"

using std::list;
using std::cout;
using std::endl;




#define cDebug std::cerr




// klasse-deklarasjoner:

//Ustarta:
class axon; 			// klasse for axon. Planlegger å dele opp seinare, i axon hillock og axon proper? 
class astrocytt; 		// klasse for astrocytt. Skal gi alpha for alle neuron i dens "domain".
//starta:
class dendritt; 		// klasse for dendritt. En av klassene som kan ha synapser.


class synapse; 		 	// grunnklasse for synapse. Kan brukes som den er.

class neuron; 			// Hovedklasse for funksjonaliteten til neuron. Temporal leaky integrator, fyring, inneholder synapse-vektor.
class neuroSensor; 		// NeuroSensor for å oversette tilstand til neuro-språk.
//class arbeidsHistorieElement; // Test for å ha en form for logg. Ikkje kommet så langt. Ta bort?

class neuroParametre; 		// neuroParametre: \kappa, \alpha, t_0, periode, osv. Konkret klasse



// globale variabelDeklarasjoner:
extern unsigned long ulTidsiterasjoner;
//extern list<synapse*> 		    pNesteSynapseUtregningsKoe; Flytta inn i synapse-klassa som static


#ifndef NEURON_h
  #define NEURON_h
 
  #include "synapse.h"




/*******************************************************************************************************
 ********************    class neuron                                                *******************
 ********************        - Hovedklasse for Auron. Innholder alt, med unntak      *******************
 ********************          av synapse-funksjonaliteten.                          *******************
 ******************************************************************************************************/

class neuron {
	private:
		// neste fyring ved noværande kappa, alpha, t_0
		static std::map<neuron*, unsigned> sNesteFyringForNeuron; 
		// Hm beste måten å regne ut neste er kanskje ved å regne ut depolverdi ved ny kappa, og regne ut perioden for den
		// 	delta kappa->tau (kor mykje kappa ligger over tau), og regne ut periode fra dette. Så ved neste omgang kjøre det for depol = 0.

	protected:
		int nVerdiForDepolarisering; 
 			// Trengs ikkje meir, for dette regnes ut heile tida. Lurt å ta med for optimalisering? lagre tidligare verdier..
		int nVerdiForDepolariseringEndringsrate;


		// Desse er en god ide! Endre til ulTimestampForrigeOppdateringAvKappa() eller tilsvarande..
		//unsigned long ulTimestampForrigeVerdiendring_neuronDepol;
		unsigned long ulTimestampNesteFyring;
		unsigned long ulTimestampForrigeEndingAvParam; // Oppdatering av: DA(for LTP-storleik), depol., osv.

		std::string navn;







		// ENDRE neste til å være "endring av presynaptisk periode" eller "endringAvKappa_ij" eller noke. Endring!!

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
		 * * * 
		 * * *  	virtual int sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( int nInnsignalArg )
		 * * * 		- Arg: 	innsignal. foreløpig altid lik synaptisk vekt.
		 * * *  		- Return: 	Depolarisasjon for postsyn. neuron. 
		 * 					- Kan kanskje være negativ for veldig lite depol. (=>LTD). Ikkje implementert enda.
		 * 					(-1) ved fyring av postsyn. neuron. Dette for å gi signal til presyn. for LTP.
		 * * *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/*	virtual int sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( int nInnsignalArg )
		{
			if( ulTidsiterasjoner -  ulTimestampFyring > REFRACTION_TIME_NEURON ){ cout<<"Hindra gjentatt fyring.\n"; sleep(1); return -9; }
			//int foerTidSig = nVerdiForDepolarisering; 		// for utskrift.
			
			// Legger til verdi.
			nVerdiForDepolarisering += nInnsignalArg;
			

			// tilordner tempklokke til timestamp for verdien for nVerdiForDepolarisering (neuron::timestamp_forrige_input)
			ulTimestampForrigeVerdiendring_neuronDepol = ulTidsiterasjoner;
			
			// ser om det går over terskel:
			int returverdi;
			if( (returverdi = sjekkKorDepolarisertNeuronEr() ) == -1 )
			{
				fyr();
				return returverdi;
			}else{
				return returverdi;
			}
		}
		
	*/	


		// DENNE kan eg endre, slik at den sjekker postsyn. depol. for STDP, eller tilsvarande..
		virtual int fyr()
		{
				
			/* FOR KAPPA:   - sette t_0 (eller tidspkt for forrige fyring) = no + refractionperiod.     
					- sjekke postsyn. depol. for alle synapser (for STDP).
					- 

			*/		

			// Ny funksjon for arbeidskø: Når kappa-neuron fyrer, skal arbeidskø taes i bruk;
				// - nye verdier for postsyn. neuron skal regnes ut.
				// - depol-verdi for postsyn. for STDP?
				// - anna?

			//for( std::vector<synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
			//{ 	// legger alle synapsene mine inn i arbeidskø
			//	synSkilleElement::pNesteSynapseUtregningsKoe.push_back( *iter );
			//}
	
	
	//også bra for Kappa?:
			// Setter timestamp for fyring:
			//ulTimestampFyring = ulTidsiterasjoner;

			// Sender respons om at eg fyrer: TA VEKK returverdi?
			return 1;
		}

		
		/****************************************************************************************
		** 	virtual int sjekkKorDepolarisertNeuronEr() 						
		**		Arg  : 	- 								
		**		Retur: 	int, som seier kor depolarisert neuron er. Brukes bl.a. for LTP/D
		**			Dersom retur er -1, betyr det at neuron fyrer. 			
		**	 									
		****************************************************************************************/
		virtual int sjekkKorDepolarisertNeuronEr( ){ 
		 	// returnerer depolarisertings-verdi. Dersom den er over terskel, returneres -1.

	//OGSÅ bra i kappa?
			if( nVerdiForDepolarisering > TERSKEL_DEFAULT  	/*nTerskel*/){

				return -1;
		 	}else
				return nVerdiForDepolarisering;
	
		}









	public:
		//neuron() 		: nVerdiForDepolarisering(0), ulTimestampForrigeVerdiendring_neuronDepol(0), navn("uten navn"),   	terskel(TERSKEL_DEFAULT) { } 
	
		// XXX LAG constructor.
		neuron( std::string n ) : navn( n )
		{

		}

		//destructor:
		~neuron()
		{ 
			//{ Tar vekk synapser (på hi sida og).
			cout<<"~neuron : " <<navn <<"\n";

			synapse* pTempSyn;

			//destruerer alle inn-synapser:
			while( !pInnSynapser.empty() ){
				cout<<pInnSynapser.size() <<" innsynapser igjen til " <<navn <<"(fra " <<pInnSynapser.back()->pPreNode->navn <<").\n";
				
				// XXX OK har funnet meir ut problemet. Følgende er quick-fix. Skal vekk..
				//if( pInnSynapser.back() == pTempSyn ) break;
				// Problemet er at vektoren ikkje registrerer at synapsa er vekk. sjå ~synapse@neuronEnhet.cpp

				// Mellomlagrer peiker til siste synapse.
			 	pTempSyn = pInnSynapser.back(); 
				cout<<"~neuron: fjærner innsyn. fra " <<(*pInnSynapser.back()).pPreNode->navn <<"\ttil " <<navn <<"(meg): \n";
				// Tar vekk siste element i vector.
				pInnSynapser.pop_back(); //-Nei: taes hand om i ~synapse
				
				// ordner det fra hi sida og:
				pTempSyn->pPreNode->taBortOutputSynapse( pTempSyn ); //XXX

				// Sletter synapse som pTempSyn peiker på.
				// for no, driter eg i minnelekkasje, og dropper delete. Lekkasje i frie lager!
			 	//delete( pTempSyn ); // XXX Istadenfor, tar eg den bare vekk fra pInnSynapser-lista
			} //og ut-synapser:
			while( !pUtSynapser.empty() ){
				// Mellomlagrer peiker til siste synapse.
	 			pTempSyn = pUtSynapser.back();
				cout<<"~neuron: fjærner utsyn. fra " <<navn <<"(meg) til " <<(*pUtSynapser.back()).pPostNode->navn <<endl;
				// Tar vekk siste element i vector.
				pUtSynapser.pop_back(); //-Nei: taes hand om i ~synapse
			
				pTempSyn->pPostNode->taBortInputSynapse( pTempSyn );
				// Sletter synapse som pTempSyn peiker på.
				// for no, driter eg i minnelekkasje, og dropper delete. Lekkasje i frie lager!
				//delete( pTempSyn ); // XXX Istadenfor, tar eg den bare vekk fra pUtSynapser-lista.
			}

			//evt andre ting
	 		cout<<"~neuron ferdig for " <<navn <<endl;
			//}
		} 


		// U-testa: (grundig, men den funker)
		void taBortInputSynapse(synapse* syn_p_ArgTaVekk)
		{ 
	 		//fjærner arg fra inputliste: 	//{
			for( std::vector<synapse*>::iterator iter = pInnSynapser.begin(); iter != pInnSynapser.end() ; iter++ ){
				if( *iter == syn_p_ArgTaVekk ){
					cout<<"\ttaBortInputSynapse(syn*): Fjærner input-syn. fra [" <<syn_p_ArgTaVekk->pPreNode->navn <<"] -> " 
										     <<"meg(" <<navn <<")\n";
					(pInnSynapser).erase( iter );
					break;
				}
			} //}
		} 

		void taBortOutputSynapse(synapse* syn_p_ArgTaVekk)
		{ // {
	 		//fjærner arg fra inputliste: //{
			for( std::vector<synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end() ; iter++ ){
				if( *iter == syn_p_ArgTaVekk ){
					cout 	<<"\ttaBortOutputSynapse(syn*): Fjærner output-syn. fra meg(" <<navn <<") -> [" 
						<<syn_p_ArgTaVekk->pPostNode->navn <<"]\n";
					(pUtSynapser).erase( iter );
					break;
				}
			} //}
		}

	


		std::string getNavn() const { return navn; }

	


		neuroParametre neuroParam;




		// Holder styr på utsynapser vha. vector.
		std::vector <synapse*> pUtSynapser;

		// Holder styr på innsynapser, bl.a. for å holde orden på hetero LTD og LTP og totalVekt.
		std::vector <synapse*> pInnSynapser; 
	










	// setter synapse til å være venneklasse;
	friend class synapse;
	friend std::ostream & operator<< (std::ostream & ut, neuron neuroArg );
};

/* Dendritt:
* 	Neuron har en eller fleire av denne klassa.
* 	Inneheld synapser. Kanskje peiker til extracellular fluid? (bestemmer alpha for synapsa)
* 	
* 	Skal ha innsynapser. Skal regne ut egen K. Sender vidare. Soma regner ut sin kappa bl.a. basert på avstand til ulike dendrittane, og deira kappa.
	  Dette er lineære kalkulasjoner, så rimelige lette operasjoner.
* 		- skal dermed ha egen K
*/
class dendritt{
	std::list<synapse> innkoblinger;

	neuroParametre dendrittParam;
	
	//XXX	friend class astrocytt;
};





#endif

// vim:fdm=marker:fmr=//{,//}:fdl=3:
