//div1.h
#include "device.h"

class Div1: public Device {
  sample value;
  public:
    Div1(sample v):Device(1,1){value=v;} //in, out
    void cycle() {setOutput(0,getInput(0)/value);}
};
