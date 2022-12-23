//mult1.h
#include "device.h"

class Mult1: public Device {
  sample value;
  public:
    Mult1(sample x):Device(1,1){value=x;} //in, out
    void cycle() {setOutput(0,getInput(0)*value);}
};
