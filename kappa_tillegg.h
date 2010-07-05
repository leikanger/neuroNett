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
	public:
		//Constructor:
		neuroParametre() : kappa(0), alpha(1){}


		int kappa;
		
		float alpha;
		// Kanskje heller ha Extracellular spenning? Og vidare regne ut synapsens alpha fra dette? Jmf. tanke om at EC spenning endrer drivkrafta 
		// 	til ioneflyten over membran, og dermed farta til flyten...
		
		int periode;
//#define TERSKEL_DEFAULT 999; //terskel fra neuron.h
		
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
			 ut 	<<"neuronets parametre: "
				<<"\n\tkappa:  \t"  <<pNP.kappa
				<<"\n\talpha:  \t"  <<pNP.alpha
				<<"\n\tperiode:\t"  <<pNP.periode  <<" (inkludert refractiontime " 
					<<REFRACTION_TIME_NEURON <<", ved terskel " <<TERSKEL_DEFAULT <<")"	;
			return ut;
		}

};




#endif
