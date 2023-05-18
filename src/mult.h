//mult.h
#include "device.h"

class Mult: public Device {
  sample *si[2];
  sample so[1];
  public:
    Mult():Device(si,2,so,1,0,0,0,0){} //in[0], in[1], out
    void cycle() {setOutput(0,NORMALIZE(getInput(0))*NORMALIZE(getInput(1)));}
};
