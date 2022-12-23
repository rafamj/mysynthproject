//delay.h
#include "device.h"

class Delay: public Device {
  sample alm[44100 * 5];
  int i;
  public:
    Delay():Device(1,1){i=0;} //in, out
    void cycle();
};
