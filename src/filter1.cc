//filter.cc
#include <math.h>
#include "filter.h"

Filter::Filter():Device(3,1) { //in cutoff reso out
  v0=v1=v2=v3=0.0;
}

void Filter::cycle() {
  sample cutoff=2*M_PI*getInput(1)/rate;  
  sample reso=pow(0.5, (getInput(2)*127+24) / 16.0);

  v0=(1-reso*cutoff)*v0  -  (cutoff)*v1  + (cutoff)*getInput(0);
  v1=(1-reso*cutoff)*v1  +  (cutoff)*v0;
  v2=(1-reso*cutoff)*v2  +  (cutoff)*v1;
  v3=(1-reso*cutoff)*v3  +  (cutoff)*v2;
  
  setOutput(0,v1);
}

