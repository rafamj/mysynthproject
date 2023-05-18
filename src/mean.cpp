#include "mean.h"

Mean::Mean(sample **ssi,int nn):Device(ssi,nn,so,1,0,0,0,0) {
  si=ssi;
  n=nn;
}

void Mean::cycle() {
  sample r=0;
  for(int i=0;i<n;i++) {
    r += getInput(i)/n;
  }
  setOutput(0,r);
}
