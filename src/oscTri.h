//oscTri.h
#include "device.h"

class OscTri: public Device {
  int phase;
  public:
    OscTri():Device(2,1){} //note pitchbend out
    void cycle();
};
