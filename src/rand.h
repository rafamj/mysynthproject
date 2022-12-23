//rand.h
#include <random>
#include "device.h"

class Rand: public Device {
  public:
    Rand():Device(0,1){} // out
    void cycle() {setOutput(0,((sample)rand()/RAND_MAX));}
};
