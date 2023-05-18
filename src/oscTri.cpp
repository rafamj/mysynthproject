//oscTri.cc
#include <math.h>
#include "oscTri.h"
//#include <cstdio>


void OscTri::cycle() {
  sample res=0;
  sample freq=getInput(0) + getInput(1);

        if(freq==0) {
	  phase=0;
	  setOutput(0,0);
	  return;
	}
        phase++;
	freq=getInput(0) + getInput(1);
        if(phase>=2*rate/freq) {
	  phase=0;
	  res=0;
	} else if(phase<rate/(2*freq)) { //first segment
	  res= MAX_VALUE * (phase/(rate/(2*freq)));
	} else if(phase<3*rate/(2*freq)) { //central segment
	  res=MAX_VALUE * (1-(phase-rate/(2*freq))/(rate/(2*freq)));
	} else { //last segment
	  res=MAX_VALUE * (-1+(phase-3*rate/(2*freq))/(rate/(2*freq)));
	}
	setOutput(0,res);
}

