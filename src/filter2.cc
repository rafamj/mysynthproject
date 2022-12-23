//filter.cc
#include <math.h>
#include "filter.h"

Filter::Filter():Device(3,1) { //in cutoff reso out
  v0=v1=0.0;
}

void Filter::cycle() {
  //double cutoff=2*M_PI*getInput(1)/rate;  
  //double reso=pow(0.5, (getInput(2)*127+24) / 16.0);

  //v0=(1-reso*cutoff)*v0  -  (cutoff)*v1  + (cutoff)*getInput(0);
  //v1=(1-reso*cutoff)*v1  +  (cutoff)*v0;
  //
  sample cutoff= getInput(1);
  sample reso=getInput(2);
  sample feedback=reso+reso/(1.000001 -cutoff);

  v0+= cutoff * (getInput(0) - v0);// + feedback * (v0-v1));
  v1+= cutoff * (v0-v1);
  v2+= cutoff * (v1-v2);
  v3+= cutoff * (v2-v3);
  setOutput(0,v3);
}

