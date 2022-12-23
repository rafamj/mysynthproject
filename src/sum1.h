//sum1.h
#include "device.h"

class Sum1: public Device {
  sample value;
  public:
    Sum1(sample v):Device(1,1){value=v;} //in, out
    void cycle() {setOutput(0,getInput(0)+value);}
};
