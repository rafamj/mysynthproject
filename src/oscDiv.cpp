//oscDiv.cc
//#include <math.h>
#include "oscDiv.h"

OscDiv::OscDiv():Device(si,1,out,OCTAVES,0,0,0,0){ //note note out[]
  for(int i=0;i<OCTAVES;i++) out[i]=MAX_VALUE;
  phase=0;
  delta=0;
  rem=0;
  zero=0;
  //ticks=change=0;
}

void OscDiv::cycle() {
	//int freq=(int)getInput(0);
        phase++;
	if(phase>=zero+delta) {
	  int freq=(int)getInput(0);
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
	  out[0]*=-1;
	  setOutput(0,out[0]);
	  for(int i=1; i<OCTAVES; i++) {
	    if(out[i-1]==-MAX_VALUE) {
	      out[i]*=-1;
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
	  printf("frec %f %8d %f\n",((double)change*rate)/((double)ticks),freq,((double)change*rate/((double)ticks)-freq));
	  ticks=change=0;
	}
*/	
}

