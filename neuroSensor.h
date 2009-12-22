
/*
	PLAN:
	Lage egen sensor-tråd for generering av sensorinput. Denne skal iterere en int og sende ut aksjonspotensial for sensorane i en definert frekvens.
	Frekvens definert av konstant eller funksjon. Frekvens konkretisert ved å sende ut aksjonspotensial kvar n-te iterasjon (vha. int). Dette gjøres 
	på normal måte, som neuron gjør det, ved å synapse->overfoerSignaletTilPostsynNeuron() .

*/


#ifndef NEUROSENSOR_H
  #define NEUROSENSOR_H

  #include "neuroEnhet.h"

/*************************************************************
****    neuroSensor - klasse for generering av signal for ****
****               asynkrone neurale nett. Den skal       ****
****               typisk ligge i egen tråd.              ****
****    Arver fra neuron - klasse.                        ****
****                                                      ****
****    Funksjon: Generering av input-mønster for asynk.  ****
****              neurale nett.                           ****
****    Skal sjekkes av egen tråd, og sende signal, om    ****
****    det er på tide. (gitt av statisk frekvens, input..**** 		neuroSensor( neuron* pN, double vekt, int "frekvens", string navn, int antallSig) : 
*************************************************************/
class neuroSensor : public neuron{
	private:
		unsigned int nFrekvens;
		//unsigned long ulForrigeTidspunkt; // for synSkilleElement, og frekvensutregning.
		int nAntallRepitisjonerAvSignal;

	public:	 
/*		neuroSensor( neuron* pN, double vekt ) : nFrekvens(0)
			{ 	leggTilSynapse( pN, vekt );    pNeuroSensorListe.push_back( this );	}
	//neuroSensor( neuron* pN, double vekt ) 		    			{ 	pUtSynapser.push_back( new synapse( pN, vekt )  ); 	}
		neuroSensor( neuron* pN, double vekt, std::string navn ) : neuron(navn), nFrekvens(0)	
			{ 	leggTilSynapse( pN, vekt );    pNeuroSensorListe.push_back( this );	}
*/

		// XXX  "vekt" er litt utdatert.. mange variable no.. FIX
		neuroSensor( neuron* pN, double vekt, int nF, std::string navn, int antallSig =-1) : 
								neuron(navn), nFrekvens(nF), nAntallRepitisjonerAvSignal(antallSig) { 
			//før: leggTilSynapse( pN, vekt );   no:
			new synapse( this, pN, false, vekt ); 
			
			// "venter" tilfeldig tidsintervall:
			if( ulTidsiterasjoner<100 ) ulTidsiterasjoner += rand()%10;

			cout 	<<"const. i sensor\n"
				<<"\tdouble vekt:\t\t" <<vekt 
				<<"\n\tTid mellom kvar puls:\t" <<nFrekvens 
				<<"\n\tantall repitisjoner:\t" <<nAntallRepitisjonerAvSignal <<endl;


			pNeuroSensorListe.push_back( this );
		}

		void sjekkOmSkalFyres_ogFyr()
		{ 
			if( (ulTidsiterasjoner-ulTimestampFyring) >= nFrekvens  	&&  	(nAntallRepitisjonerAvSignal != 0 )  )
			{
				fyr();
				nAntallRepitisjonerAvSignal --;
				cout<<"fyrer " <<navn <<"\n";
			}
		}

		// leggTilSynapse() finnes fra neuron.

		friend class synSkilleElement;
};

#endif
