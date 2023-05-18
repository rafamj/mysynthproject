//oscSaw.h
#include "device.h"

class OscSaw: public Device {
  int phase;
  sample *si[2];
  sample so[1];
  public:
    OscSaw():Device(si,2,so,1,0,0,0,0){} //note pitchbend out
    void cycle();
};
