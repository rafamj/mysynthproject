//oscSQ.h
#include "device.h"

class OscSQ: public Device {
  int phase;
  int delta;
  int zero;
  int rem;
  //int cycles,n;
  sample *si[2];
  sample so[1];
  public:
    OscSQ():Device(si,2,so,1,0,0,0,0){delta=0;rem=0;/*cycles=n=0;*/} //note pitchbend out
    void cycle();
};
