//div.h
#include "device.h"

class Div: public Device {
  public:
    Div():Device(2,1){} //in1, in2, out
    void cycle() {setOutput(0,getInput(0)/getInput(1));}
};
