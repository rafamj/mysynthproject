//oscSaw.cc
#include <math.h>
#include "oscSaw.h"

void OscSaw::cycle() {
  sample res=0;
  sample freq;
  //static int cycles=0,ticks=0;

        phase++;
	freq=getInput(0) + getInput(1);
        if(phase>=2*rate/freq) {
	  phase=0;
	  res=-1;
	  //cycles++;
	} else {
	  res=-1+2*(phase/(2*rate/freq));
	}
	/*
	{
          ticks++;
	  if(ticks>=5 * rate) {
	    printf("%f %f\n",(double)2*cycles*rate/(double)ticks,freq);
	    ticks=cycles=0;
	  }
	}
	*/
  setOutput(0,res); 	
}

