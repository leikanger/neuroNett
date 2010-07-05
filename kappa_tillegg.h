#include "main.h"



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
		float kappa;
		float alpha;
		float periode;
		float T; //terskel;
		
#define REFRACTIONTIME 0.02; //Kanskje denne er variabel etter kor "sliten" neuronet er?
		int kalkulerPeriode()
		{
		 	periode = -1/alpha * log(1 - T/kappa) + REFRACTIONTIME;
			return 0;
		}
};




#endif
