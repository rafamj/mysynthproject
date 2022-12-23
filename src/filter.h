//filter.h
#include "device.h"

class Filter: public Device {
  sample y1;
  sample y2;
  sample y3;
  sample y4;
  sample oldx;
  sample oldy1;
  sample oldy2;
  sample oldy3;

  public:
    Filter();
    void cycle();
};
