//glide.cc
#include "glide.h"

Glide::Glide():Device(2,1) {//in time  out
 ant=0.0;
}

void Glide::cycle() {
  sample in=getInput(0);
  sample time=getInput(1);
  sample f=rate*time*0.5;
  sample out=(f*ant+in)/(f+1);
  ant=out;
  setOutput(0,out);
}
  
