//delay.cc
#include "delay.h"

void Delay::cycle() {
  setOutput(0,alm[i]);
  alm[i]=getInput(0);
  //i=(i+1) % (44100*5);
  i=(i+1) % 20000;
}
