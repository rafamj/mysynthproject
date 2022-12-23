//sub.h
#include "device.h"

class Sub: public Device {
  public:
    Sub():Device(2,1){} //in[2], out
    void cycle() {setOutput(0,getInput(0)-getInput(1));}
};
