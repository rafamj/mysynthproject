//atan.h
#include "device.h"

class Atan: public Device {
  public:
    Atan():Device(1,1){} //in, out
    void cycle() {setOutput(0,atan(getInput(0)));}
};
