//oscSQ.cc
//#include <math.h>
#include "oscSQ.h"

void OscSQ::cycle() {
  int zero;
  sample z;
  sample res=0;
  sample freq;
        phase++;
        freq=getInput(0) + getInput(1);
        zero=rate /freq;
	z=rate /freq;
        if(phase<zero) {
          res =MAX_VALUE;
        }else {
          if(phase>2*z-delta) {
	      delta += z-zero;
	      if(delta>1) delta -=1;
	  /*    
	      cycles+=phase;
	      n++;
	      if(cycles>rate * 5) {
	        printf("freq %f %f\n",((double)n*rate*2)/((double)cycles),freq);
		n=cycles=0;
	      }
	   */  
	      phase=0;
	  }
              res=-MAX_VALUE;
        }
  setOutput(0,res);
}

