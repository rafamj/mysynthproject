//sub1.h
#include "device.h"

class Sub1: public Device {
  sample value;
  public:
    Sub1(sample v):Device(1,1){value=v;} //in, out
    void cycle() {setOutput(0,getInput(0)-value);}
};
