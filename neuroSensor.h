#include "neuroEnhet.h"

/*
	PLAN:
	Lage egen sensor-tråd for generering av sensorinput. Denne skal iterere en int og sende ut aksjonspotensial for sensorane i en definert frekvens.
	Frekvens definert av konstant eller funksjon. Frekvens konkretisert ved å sende ut aksjonspotensial kvar n-te iterasjon (vha. int). Dette gjøres 
	på normal måte, som neuron gjør det, ved å synapse->overfoerSignaletTilPostsynNeuron() .

*/




/*************************************************************
****    neuroSensor - klasse for generering av signal for ****
****               asynkrone neurale nett. Den skal       ****
****               typisk ligge i egen tråd.              ****
****    Arver fra neuron - klasse.                        ****
****                                                      ****
****    Funksjon: Generering av input-mønster for asynk.  ****
****              neurale nett.                           ****
****    Skal sjekkes av egen tråd, og sende signal, om    ****
****    det er på tide. (gitt av statisk frekvens, input..****
*************************************************************/
class neuroSensor : public neuron{
	private:
		unsigned int nFrekvens;
		unsigned long ulForrigeTidspunkt; // for tidsSkilleElement, og frekvensutregning.
		int nAntallRepitisjonerAvSignal;

	protected: 
		virtual int sigma_funk( double verdi ){ 
			if(verdi > terskel)	return 1;
		 	else			return 0;
		}
		virtual int sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( double dInnsignalArg )
		{ 	// for å sleppe utskrift på neuroSensor..
		 	unsigned long ulKlokketikkSidenSisteInput =   ulTidsiterasjoner - ulTimestampForrigeInput;
			
			double tempFaktor = 1;
			for(unsigned int i=0; i < ulKlokketikkSidenSisteInput; i++ ){ 	tempFaktor *= DEGRADERINGSFAKTOR_FOR_TIDSINTEGRASJON_I_NEURON;   }

			dVerdiForUtregningAvSigmaverdiForrigeIter *= tempFaktor; 
		
			dVerdiForUtregningAvSigmaverdiForrigeIter += dInnsignalArg;

			ulTimestampForrigeInput = ulTidsiterasjoner;
			
			
			// ser om det går over terskel, og om fyrer:
			if( sigma_funk( dVerdiForUtregningAvSigmaverdiForrigeIter )  )
			{
				for( std::vector<synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
				{ 	// legger alle synapsene mine inn i arbeidskø
					pNesteSynapseUtregningsKoe.push_back( *iter );
				}	//Legg til alle synapser i neste fifo-kø foR utregning. //OK // går over til bare ei kø, med "tids-synapse"
				
				// Resetter dVerdiForUtregningAvSigmaverdiForrigeIter, etter fyring:
				dVerdiForUtregningAvSigmaverdiForrigeIter = 0;
				return 1; 	//respons om eg fyrer.
			}else{  return 0; }
		}
	public:	 
/*		neuroSensor( neuron* pN, double vekt ) : nFrekvens(0)
			{ 	leggTilSynapse( pN, vekt );    pNeuroSensorListe.push_back( this );	}
	//neuroSensor( neuron* pN, double vekt ) 		    			{ 	pUtSynapser.push_back( new synapse( pN, vekt )  ); 	}
		neuroSensor( neuron* pN, double vekt, std::string navn ) : neuron(navn), nFrekvens(0)	
			{ 	leggTilSynapse( pN, vekt );    pNeuroSensorListe.push_back( this );	}
*/
//XXX Finn på noke lurt her. XXX for frekvens.		
	/* den kan:
		- sjekkes av tidsSkilleElement : public synapse, og slikleis legges til på rett plass.. XXX Jess!
	*/ 
		neuroSensor( neuron* pN, double vekt, int nF, std::string navn, int antallSig) : 
								neuron(navn), nFrekvens(nF), nAntallRepitisjonerAvSignal(antallSig) { 
			leggTilSynapse( pN, vekt );	
			
			// "venter" tilfeldig tidsintervall:
			if( ulTidsiterasjoner<100 ) ulTidsiterasjoner += rand()%10;

			ulForrigeTidspunkt = ulTidsiterasjoner; 
			cout<<"const. i sensor\n"; 

			pNeuroSensorListe.push_back( this );
		}

		void sjekkOmSkalFyres_ogFyr()
		{ 
			if( nAntallRepitisjonerAvSignal >= 0 ){
				nAntallRepitisjonerAvSignal --;

				if(  ulTidsiterasjoner-ulForrigeTidspunkt >= nFrekvens ) /// XXX endre frekvens slik at den gir meir meining. t.d. > 1/frekv
				{
					fyr();
					cout<<"fyrer " <<navn <<"\n";
				}
			}
		}
		void fyr()
		{
	 		this->sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal(9999);	
			ulForrigeTidspunkt = ulTidsiterasjoner;
		}
		// leggTilSynapse() finnes fra neuron.

		friend class tidsSkilleElement;
};
