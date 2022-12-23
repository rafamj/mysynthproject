//oscDiv.h
#include "device.h"
#define OCTAVES 8
class OscDiv: public Device {
  sample out[OCTAVES];
  int phase;
  sample delta;
  sample z;

  //int ticks,change;
  public:
    OscDiv(); //note pitchbend out[8]
    void cycle();
};
