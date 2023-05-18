//sum2.h
#include "device.h"

class Sum: public Device {
   sample *si[2];
   sample so[1];
  public:
    Sum():Device(si,2,so,1,0,0,0,0){} //in[2], out
    void cycle() {setOutput(0,getInput(0)+getInput(1));}
};
