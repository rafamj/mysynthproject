//glide.cc
#include "glider.h"

Glider::Glider():Device(si,2,so,1,0,0,0,0) {//in time  out
 ant=0.0;
}

void Glider::cycle() {
  sample in=getInput(0);
  sample time=getInput(1);
  sample f=rate*time*0.5;
  sample out=(f*ant+in)/(f+1);
  ant=out;
  setOutput(0,out);
}
  
