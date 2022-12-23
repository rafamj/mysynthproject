//oscSaw.h
#include "device.h"

class OscSaw: public Device {
  int phase;
  public:
    OscSaw():Device(2,1){} //note pitchbend out
    void cycle();
};
