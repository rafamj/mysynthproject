//oscPWM.cc
#include "oscPWM.h"

void OscPWM::cycle() {
  int zero;
  sample z;
  sample res=0;
  sample freq;
        phase++;
        freq=getInput(0) + getInput(1);
        zero=rate /freq;
	z=rate /freq;
        if(phase<zero*(0.1+0.8*getInput(2))) {
          res =1;
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
              res=-1;
        }
  setOutput(0,res);
}

