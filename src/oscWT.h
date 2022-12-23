//oscWT.h
#include "device.h"

class OscWT: public Device {
  sample *wav;
  int phase;
  int spc; //samples per cycle
  int nw; //number of waves
  sample delta;
  public:
    OscWT(unsigned char *data, size_t n, int type);
    void cycle();
    ~OscWT();
};
