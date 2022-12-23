//delay2.h
#include "device.h"

class Delay2: public Device {
  const int MAX=3;
  int i;
  sample *alm;
  public:
    Delay2():Device(2,1){i=0;alm=new sample[rate*5];} //in del, out
    void cycle();
    ~Delay2(){delete [] alm;}
};
