//oscSaw.cc
#include <math.h>
#include "oscSaw.h"

void OscSaw::cycle() {
  sample res=0;
  sample freq=getInput(0) + getInput(1);
  //static int cycles=0,ticks=0;
        if(freq==0) {
	  phase=0;
	  setOutput(0,0);
	  return;
	}
        phase++;
        if(phase>=2*rate/freq) {
	  phase=0;
	  res=-MAX_VALUE;
	  //cycles++;
	} else {
	  res=MAX_VALUE*(-1+(phase*freq)/rate);
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

