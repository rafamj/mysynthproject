//oscTri.cc
#include <math.h>
#include "oscTri.h"
//#include <cstdio>


void OscTri::cycle() {
  sample res=0;
  sample freq;

        phase++;
	freq=getInput(0) + getInput(1);
        if(phase>=2*rate/freq) {
	  phase=0;
	  res=0;
	} else if(phase<rate/(2*freq)) { //first segment
	  res=phase/(rate/(2*freq));
	} else if(phase<3*rate/(2*freq)) { //central segment
	  res=1-(phase-rate/(2*freq))/(rate/(2*freq));
	} else { //last segment
	  res=-1+(phase-3*rate/(2*freq))/(rate/(2*freq));
	}
	setOutput(0,res);
}

