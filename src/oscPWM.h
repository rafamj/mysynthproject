//oscPWM.h
#include "device.h"

class OscPWM: public Device {
  int phase;
  //long int cycles,n;
  sample delta;
  public:
    OscPWM():Device(3,1){delta=0;/*cycles=n=0;*/} //note pitchbend pw out
    void cycle();
};
