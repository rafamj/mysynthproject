//filter.h
#include "device.h"

class Filter: public Device {
  sample v0,v1,v2,v3;
  public:
    Filter();
    void cycle();
};
