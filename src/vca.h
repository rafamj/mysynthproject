//vca.h
#include "device.h"

class Vca: public Device {
  public:
    Vca():Device(2,1){} //in, cv, out
    void cycle() {setOutput(0,getInput(0)*getInput(1));}
};
