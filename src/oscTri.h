//oscTri.h
#include "device.h"

class OscTri: public Device {
  sample *si[2];
  sample so[1];
  int phase;
  public:
    OscTri():Device(si,2,so,1,0,0,0,0){} //note pitchbend out
    void cycle();
};
