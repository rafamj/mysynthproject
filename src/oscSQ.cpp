//oscSQ.cc
//#include <math.h>
#include "oscSQ.h"

void OscSQ::cycle() {
  phase++;
        if(phase>=zero+delta) {
          int freq=(int)getInput(0)+(int)getInput(1);
	  if(!freq) {
	    outputs[0]=0;
	    return;
	  }
          zero=rate/freq;
          rem +=rate % (int)freq;
          //printf("phase %d zero %d delta %d rate %d rem %d freq %f\n",phase,zero,delta,rate,rem,freq);
          if(rem>freq) {
            delta=1;
            rem -=freq;
          } else {
            delta=0;
          }
          phase=0;
          if(outputs[0]>0) {
            outputs[0]=-MAX_VALUE;
          } else {
            outputs[0]=MAX_VALUE;
          }
        }
  //printf("freq %d  zero %d phase %d phase<zero %d z %f res %d MAX_VALUE %d\n",freq,zero,phase,phase<zero,z,res,MAX_VALUE);
}

