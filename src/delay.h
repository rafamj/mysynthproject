//delay.h
#include "device.h"

class Delay: public Device {
  sample *si[1];
  sample so[1];
  sample alm[44100 * 5];
  int i;
  public:
    Delay():Device(si,1,so,1,0,0,0,0){i=0;} //in, out
    void cycle();
};
