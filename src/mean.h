#include "device.h"

class Mean: public Device {
   sample **si;
   sample so[1];
   int n;
  public:
    Mean(sample **ssi,int nn); //in[n], out
    void cycle();
};

