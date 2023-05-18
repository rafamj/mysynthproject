//oscDiv.h
#include "device.h"
#define OCTAVES 8
class OscDiv: public Device {
  sample *si[1];
  sample out[OCTAVES];
  int phase;
  int delta;
  int zero;
  int rem;
  //int ticks,change;
  public:
    OscDiv(); //note pitchbend out[8]
    void cycle();
};
