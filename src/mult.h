//mult.h
#include "device.h"

class Mult: public Device {
  public:
    Mult():Device(2,1){} //in[0], in[1], out
    void cycle() {setOutput(0,getInput(0)*getInput(1));}
};
