//adsr.cc
#include "adsr.h"

Adsr::Adsr():Device(5,1) {//gate,a,d,s,r  out
  state=CLOSED;
  out=0.0;
}

void Adsr::cycle() {
  int slowness=5;

  if(state!=CLOSED && state!=R && getInput(0)==0.0) { //gate==0
    state=R;
  }
  switch(state){
    case CLOSED: if(getInput(0)>0.5) state=A;break;
    case A: out=out+1/(slowness*(getInput(1)+0.00001)*rate);if(out>=1.0){state=D;};break;
    case D: out=out-1/(slowness*(getInput(2)+0.00001)*rate);if(out<=getInput(3)){out=getInput(3);state=S;};break;
    case S: out=getInput(3);break;
    case R: if(getInput(0)>0.5) { //retrigger
              state=A;out=0.0;
	    } else {
	      out=out-1/(slowness*(getInput(4)+0.00001)*rate);
	      if(out<=0.0){
	        state=CLOSED;out=0.0;
	       }
	    }
	    break;
  }
  setOutput(0,out);
}
