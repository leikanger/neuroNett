//#include "main.h"

#define TERSKEL_DEFAULT 999.9 
#define REFRACTION_TIME_NEURON 2 //eller kanskje heller 0.02 (gange med 100)
				//Kanskje denne er variabel etter kor "sliten" neuronet er?



#ifndef KAPPA_TILLEGG
  #define KAPPA_TILLEGG 



/*******************************************************************************************************
 ********************    class neuroParametre   -- konkret klasse                    *******************
 ********************          - holder styr på nye parametre som \kappa, \alpha     *******************
 ********************            periode, t_0, osv                                   *******************
 ********************          + alt anna nytt som følge av innføring av \kappa      *******************
 ********************                                                                *******************
 ******************************************************************************************************/
class neuroParametre{ 		// neuroParametre: \kappa, \alpha, t_0, periode, osv. Konkret klasse
	private:
		// Tenker at den periode som blir kalkulert ut her skal være 1/10 av en timestep. Ser bra ut. for K = 5*T -> periode 24 => 2.4=3 klikk.
		int periode; //XXX denne skal vekk herifra, og inn i neuron-klasse. neuroParametre brukes også i dendritt..
		
		float kappa;
		
		float alpha;
		// Kanskje heller ha Extracellular spenning? Og vidare regne ut synapsens alpha fra dette? Jmf. tanke om at EC spenning endrer drivkrafta 
		// 	til ioneflyten over membran, og dermed farta til flyten...
			
	public:
		//Constructor:
		neuroParametre() : kappa(0), alpha(1){}

		//XXX sjå forrige xxx
 		const int getPeriode()
		{
			if(periode < 0) return 0; // dersom periode er negativ, dvs. at kappa < terskel, så returner 0.
			
			return periode;
		}
	

		/* 	Når param. får oppdatert kappa, skal den legges til ei liste som gåes gjennom på slutten av iterasjonen. 
		 * 		Maks ei oppføring per neuron.
		 * 		Når eit neuron er der, skal gjennomgangen føre til at denne får oppdatert neuroparametre. ( kalkulerPeriode(); )
		 *
		*/
		int nyKappa(float nyKappa){
			kappa = nyKappa;
			return kalkulerPeriode();
		}
		int nyAlpha(float nyAlpha ){
			// finn delta alpha til delta periode - transform. For gradvis endring av alpha. F.eks. pga. synaptisk plasticity.
			alpha = nyAlpha;
			return kalkulerPeriode();
		}
		// XXX Sjå forrige xxx
		int kalkulerPeriode()
		{
			if( kappa < TERSKEL_DEFAULT ){
				periode = -999; //Signallere at det ikkje går an.. Negativ periode = -99 betyr uendelig periode..
				return periode;
			}
		 	periode = -(1/alpha) * log(1 - TERSKEL_DEFAULT/kappa ) + REFRACTION_TIME_NEURON + 0.5; //runda opp ved +0.5..
			
			//std::cout<<"a: " <<alpha <<" K: " <<kappa <<" T: " <<TERSKEL_DEFAULT <<"ref.t. " <<REFRACTION_TIME_NEURON <<"\n";
		 	//std::cout<< -1/alpha * log(1 - (float)TERSKEL_DEFAULT/(float)kappa ) + REFRACTION_TIME_NEURON <<std::endl; //runda opp ved +0.5..
			
			return periode;
		}


		friend std::ostream & operator<< (std::ostream & ut, neuroParametre pNP)
		{
			//ut 	<<"neuronets parametre: "
			//	<<"\n\tkappa:  \t"  <<pNP.kappa
			//	<<"\n\talpha:  \t"  <<pNP.alpha <<"\n";
			
			// Dersom K>T: skriv ut periode.
			if( pNP.kappa < TERSKEL_DEFAULT )
			{
				ut 	<<"\tperiode:\t\033[00;33mNaN\033[0m (uendelig) siden Kappa<Terskel "
					<<"\tTerskel: " <<TERSKEL_DEFAULT <<", Kappa: " <<pNP.kappa <<".\n";
			}else{
				ut 	<<"\tperiode:\t\033[00;32m"  <<pNP.periode  <<"\033[0m (inkludert refractiontime " 
					<<"\t\tTerskel: " <<TERSKEL_DEFAULT <<", Kappa: " <<pNP.kappa <<", alpha: " <<pNP.alpha  <<".\n";
			}

			return ut;
		}

};


#endif
