
/*
	PLAN:
	Bytt ut denne simple måten (fyr kvar n-te tidssteg) med den nye mekanismen for egen-eksitasjon ("lekker inn verdi"). 
		Denne lekkasjen kan variere med målt variabel (som i kroppen..) Ved høg målt variabel, vil det være høgare egenfrekvens..
*/


#ifndef NEUROSENSOR_H
  #define NEUROSENSOR_H

  #include "neuron.h"


/****************************************************************************************************************************************************************************************
 ***************************
 ***************************      TODO TODO TODO TDOD TODO OTODO TODOT TODOTO ODTO  OTOD TODO TODOT ODOD TODO TODO TODO TODO TODO
 ***************************          Implementer denne med tanke på nye effekta for neuron: egen-eksitasjon!
 ***************************
 *************************** 		(Egeneksitasjonen skal variere med målt variabel. Dette eg også meir biologisk!)
 ***************************
 ***************************
 ***************************
 *************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/












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
			}
		}

		// leggTilSynapse() finnes fra neuron.

		friend class synSkilleElement;
};

#endif
