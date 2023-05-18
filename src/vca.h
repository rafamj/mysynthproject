//vca.h
#include "device.h"

class Vca: public Device {
   sample *si[2];
   sample so[1];
  public:
    Vca():Device(si,2,so,1,0,0,0,0){} //in, cv, out
    void cycle() {setOutput(0,NORMALIZE(getInput(0))*NORMALIZE(getInput(1)));/*printf("0 %d 1 %d %d\n",getInput(0),getInput(1),getInput(0)>>16)*(getInput(1)>>16);*/}
};
