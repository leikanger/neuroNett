DOKUMENTASJON fOr Auron:

TID:
For å ordne tid, slik at det er avhengig av global tid, eller klokketikk i prosessor, eller andre urelevante faktore, har eg implementert en sceduler fOr
Auron. Denne går ut på at ei arbeidsliste holder orden på kva jobber som står fOr tur. Kvar gang en jobb blir utført, og denne leder til nye jobber,
blir desse nye jobbane lagt til på slutten av lista. I tillegg har eg eit objekt som har samme type "jobbane" i lista. Jobbane er av type synapse, og 
tidsSkilleElement er av typen tidsSkilleElement : public synapse ; 
Videre er funskjonen void regnUt() overloada, slik at andre handlinger kan utføres når *arbeidslista->regnUt() kalles.
Desse funskjonane er:
	- legger peiker til seg selv (objektet) til bakerst på arbeidslista.
	- øker tid med en. (Tid er en integer med discrete hopp).
	- sjekker om noken av neuroSensor-ane skal fyre ( basert på frekvensen de skal fyre med, som igjen skal være basert på tilstand til måleverdi ).
Eg hadde i utgangspunktet tenkt å implementere ei arbeidshistorie-liste også, som skulle brukes til å beregne frekvens og lignande, men gjekk vekk fra dette
siden eg ikkje fikk til en tilfredsstillande "momentan-frekvens". Dette er forsåvidt (nesten) en selvmotsigelse. Vanskelig.
Istadenfor går løsninga mi ut på å vente i 1/frekvens tid mellom kvart signal. Dette er nermaste eg kommer momentanfrekvens.


NEURON:


SYNAPSE:
Synapse ligger mellom to neuron, som det gjør i virkeligheten. Kvart synapse-objekt har to neuron-peikera, en til presyn. og en til postsyn.
Er ikkje sikker på korleis dette opprettholdes, eller blir initiert/destruert. Finn ut og skriv dette avsnittet.
	SYN_DEPRESSION: (shOrt time)
	Har implementert ulAntallSynapticVesiclesAtt, og at denne er viktig fOr overføring av signal til postsyn. neuron. Det som blir sendt over, blir 
	trekt fra synapsens beholdning. Det er prosentvis sending av syn.vesicles, så når synapsen er halvtom, vil halvparten av signalet sendes.
	Reproduksjon av syn. vesicles er dynamisk ved variabelen dOppladingsFartForSynVesicles (fOr tida ikkje dynamisk (i endring), men dette er plan.)

	Neste utfortring er å lage en mild oversving på antall synaptic vesicles, når den har vore veldig langt nede. IMPELMENTER TEORIEN MIN OM FACILITATION
		- trenger reproduksjon fra membran.
		- trenger syntese av nye syn. vesicles.
		"?"
	//TODO implementer min teori om facilitation (synaptic vesicles overshoot).

		
 	LTP_og_homoLTD: 
	Akkurat no driver eg å sysler med LTP og homoLTD. Dette har eg tenkt å implementere fra synapsens regnUt()-funksjon. Når den er ferdig med å regne ut
	en verdi, skal dette sendes inn i postsynaptisk neuron. Funksjonen fOr dette returnerer en integer fOr kor depolarisert den er. Dette har eg planer 
	om å ta med i avgjersla om LTP og homoLTD skal initieres. 
	homoLTD:
	Dette er foreløpig en passiv funkson der vekta "lekker litt", vurderer å gi den økande derivert (lekke raskare og raskare ved påfølgande LTD)
	  Vektendring går i første omgang på dGlutamatReceptoreIPostsynMem, og enventuelt etter kvar på min teori om 
	antall synapticVesicles, størrelse på membran, med variabelt setpkt. Men dette kommer nok seinare.
	STDP: spike time-dependent plasticity. Lær om dette først, så implementer det.

	VEKTENDRING:
		i første omgang går dette på homo-måten ( på samme synapse ). 
		- Dersom ei synapse fyrer, og postsyn er kraftig depolarisert, vil LTP oppstå.
		- Dersom ei synapse fyrer, og postsyn er passiv (ikkje kraftig depolarisert) vil LTD.
			- LTD vurderer eg å ha med alltid, kvar gang ei synapse er aktiv. Spørsmålet er om eg skal ha konstant LTD-rate, eller om denne skal
			økes ved gjenntatte LTD-kall.

		- etterkvart STDP.

Har også såvidt begynt med historie av arbeid. Ideen var at jobbane skulle flyttast over hit, når de var gjort. Dette har eg ikkje gjort så mykje med, og er 
ikkje sikker på om det er en god ide, eller ikkje. Greit nok med "momentan frekvens", men kanskje eg har bruk fOr frekvens-over-lang-tid. Skal prøve å unngå.

















Om_klassene_Og_variablane_Og_funksjonane:

global: - ulTidsiterasjoner : extern unsigned long 
	Tidspunkt iterert ved tidsSkilleElement . Sjå "TID" øverst i denne fila.


Neuron:
	Variabler:
		- nVerdiForDepolarisering : int 		- protected 
	       	Verdi foR depolarisering av neuron. Brukes til å kalkulere om neuron fyrer. 

		- ulTimestampForrigeInput : unsigned long 	- protected
		Tidspunkt foR forrige input ved den globale tidsvariablen, som blir iterert i tidsSkilleElement. (sjå "TID" øverst i fila).
		Brukes til å regne ut degradering av nVerdiForDepolarisering etter tid, når nVerdiForDepolarisering skal brukes til å beregne noke.

		- ulTimestampFyring 	  : unsigned long 	- protected
		Tidspunkt foR forrige fyring av aksjonspotensial. (sjå "TID" øverst i fila)
		Planlagt bruk er ved STDP og LTP/D.
		
		- navn 			  : std::string 	- protected
		navn, i bruk ved utskrift.

		- uTerskel 		  : unsigned  		- protected
		Terskel foR fyring av aksjonspotensial. Settes i konstruktor, og defauLt satt til TERSKEL_DEFAULT (akkurat no er denne satt til 100)
		
		- uTotaltAntallReceptoreIPostsynNeuron_setpunkt - protected 
		Planlagt setpkt foR antall receptore. Ikkje i gang enda. Akkurat no har eg overgang fra int antallRecepore til float.
		// ta med egenfrekvens også, etterkvart. for now skal denne være konst.
 		
		- pUtSynapser og pInnSynapser : std::vector <synapse*> - public
		Holder styr på utsynapser (foR å legge til i arbeidskø, når neuron fyrer), og innsynapser (foR å utføre hetero-LTD og LTP) Kanskje og STDP.
		

	Funksjoner:
		neuron( std::string n )   : constructor 	- public
		void settInnElektrisitetGjennomProbe( ) 	- public
			Arg: 	void
			retur: 	void
			funksjon: 	Fører til at neuron fyrer. (For å indusere eit signal, foR tesing av neuron..)

		virtual void kalkulerDegraderingAvVerdiPaaGrunnlagAvTimestampSidenSist( ) - protected - inIine
		virtual int sendInnPostsynaptiskEksitatoriskEllerInhibitoriskSignal( int nInnsignalArg ) - protected - inIine
		virtual int fyr() - protected - inIine
		virtual int sjekkKorDepolarisertNeuronEr( ) - protected - inIine
 	
		leggTilSynapse - //TODO ta vekk!
	FRIEND:
		class synapse
		operator <<( , neuron)
