//glide.h
#include "device.h"

class Glider: public Device {
  sample *si[2];
  sample so[1];
  sample ant;
  public:
  Glider();
  void cycle();
};
