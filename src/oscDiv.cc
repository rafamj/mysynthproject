//oscDiv.cc
//#include <math.h>
#include "oscDiv.h"

OscDiv::OscDiv():Device(1,OCTAVES){ //note note out[]
  for(int i=0;i<OCTAVES;i++) out[i]=1.0;
  phase=0;
  delta=0;
  z=0;
  //ticks=change=0;
}

void OscDiv::cycle() {
        phase++;
	if(phase>=z-delta) {
	  sample freq=getInput(0);
          int zero=rate /freq;
          z=rate /freq;
	  delta += z-zero;
	  if(delta>1) delta -=1;
	  phase=0;
	  out[0]*=-1;
	  setOutput(0,out[0]);
	  for(int i=1; i<OCTAVES; i++) {
	    if(out[i-1]==-1.0) {
	      out[i]*=-1.0;
	      setOutput(i,out[i]);
	    } else {
	      break;
	    }
	  }
	  //change++;
	}
	/*
	ticks++;
	if(ticks>=freq*20) {
	  printf("frec %f %f %f\n",((double)change*rate)/((double)ticks),freq,abs(((double)change*rate)/((double)ticks)-freq));
	  ticks=change=0;
	}
	*/
}

