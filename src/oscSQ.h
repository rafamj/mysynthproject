//oscSQ.h
#include "device.h"

class OscSQ: public Device {
  int phase;
  //long int cycles,n;
  sample delta;
  public:
    OscSQ():Device(2,1){delta=0;/*cycles=n=0;*/} //note pitchbend out
    void cycle();
};
