//adsr.cpp
#include "adsr.h"

Adsr::Adsr():Device(si,5,so,1,0,0,0,0) {//gate,a,d,s,r  out
  state=CLOSED;
  out=0;
}

#define DELTA 0.00001
#define MAX_TIME 5 //5 secs

static inline sample STEP(sample input, int rate) {return input==0?MAX_VALUE:(MAX_VALUE/input*MAX_VALUE/rate)/MAX_TIME;}

void Adsr::cycle() {
  sample gate=getInput(0);

  if(gate==0 && state!=CLOSED && state!=R) { //gate==0
    state=R;
  }
  switch(state){
    case CLOSED: if(gate>MAX_VALUE/2) state=A;return; break;
    case A: out=out+STEP(getInput(1),rate);if(out>=MAX_VALUE || out<0){out=MAX_VALUE;state=D;};break;
    case D: out=out-STEP(getInput(2),rate);if(out<=getInput(3)){out=getInput(3);state=S;};break;
    case S: out=getInput(3);break;
    case R: if(gate>MAX_VALUE/2) { //retrigger
              state=A;out=0;
	    } else {
	      out=out-STEP(getInput(4),rate);
	      if(out<=0){
	        state=CLOSED;out=0;
	       }
	    }
	    break;
  }
//printf("state %d MAX_VALUE %f input[1] %f rate %d out %f\n",state,MAX_VALUE,getInput(1),rate,out);
//printf("state %d MAX_VALUE %d input[1] %d STEP(1) %d rate %d out %d\n",state,MAX_VALUE,getInput(1),STEP(getInput(1),rate),rate,out);
//printf("state %d input %d\n",state,getInput(state));
  setOutput(0,out);
}
