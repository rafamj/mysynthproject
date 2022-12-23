//pan.h
#include "device.h"

class Pan: public Device {
  public:
    Pan():Device(2,2){} //in,cv, out[2]
    void cycle() {setOutput(0,getInput(0)*getInput(1));setOutput(1,getInput(0)*(1.0-getInput(1)));}
};
