//sum2.h
#include "device.h"

class Sum: public Device {
  public:
    Sum():Device(2,1){} //in[2], out
    void cycle() {setOutput(0,getInput(0)+getInput(1));}
};
