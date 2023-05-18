//mult1.h
#include "device.h"

class Mult1: public Device {
  sample *si[1];
  sample so[1];
  sample value;
  public:
    Mult1(sample x):Device(si,1,so,1,0,0,0,0),value(x){} //in, out
    void cycle() {setOutput(0,getInput(0)*value);}
};
