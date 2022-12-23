//filter4LP.h
#include "device.h"

//from https://www.musicdsp.org
//
class Filter4LP: public Device {
  sample coef[9];
  sample d[4];
  sample omega; //peak freq
  sample g;     //peak mag

  sample k,p,q,a;
  sample a0,a1,a2,a3,a4;

  int ticks;
  void calculate(sample omega, sample g);
  public:
    Filter4LP();
    void cycle();
};
