//div1.h
#include "device.h"

class Div1: public Device {
  sample *si[1];
  sample so[1];
  sample value;
  public:
    Div1(sample v):Device(si,1,so,1,0,0,0,0),value(v){} //in, out
    void cycle() {setOutput(0,getInput(0)/value);}
    //void cycle() {setOutput(0,getInput(0)/value);printf("%d %d\n",getInput(0),getInput(0)/value);}
};
