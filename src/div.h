//div.h
#include "device.h"

class Div: public Device {
  sample *si[2];
  sample so[1];
  public:
    Div():Device(si,2,so,1,0,0,0,0){} //in1, in2, out
    void cycle() {setOutput(0,getInput(0)/getInput(1));}
};
