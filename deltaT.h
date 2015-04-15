#include <ctime>
using namespace std;

#ifndef _CALC_DELTA_T_
#define _CALC_DELTA_T_

clock_t lastUpdate=0;

// la funzione deve calcolare il deltaT,
//il quale verrà successivamente usato per
//rendere il movimento indipendete dal tempo di rendering
double calculateDeltaT()
{
//  TIMING - start
  clock_t t = clock(); //salvo tempo attuale
  // elapsed time in seconds from the last draw
  double deltaT = double (t - lastUpdate) /  (double) CLOCKS_PER_SEC; 
  

  lastUpdate = t;

  return deltaT;
}

#endif