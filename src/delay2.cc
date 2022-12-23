//delay2.cc
#include "delay2.h"

void Delay2::cycle() {
  sample out=0;
  sample del=getInput(1);
  alm[i]=getInput(0);
  for(int n=0;n<MAX;n++) {
    int j=((i-(int)(n*200*del)))%(rate*5);
    if(j<0) j= -j;
    out+=alm[j];
  }
  setOutput(0,out/MAX);
  i=(i+1) % (rate*5);
}
