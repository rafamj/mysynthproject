//delay2.h
#include "device.h"

class Delay2: public Device {
  sample *si[2];
  sample so[1];
  const int MAX=3;
  int i;
  sample *alm;
  public:
    Delay2():Device(si,2,so,1,0,0,0,0){i=0;alm=new sample[rate*5];} //in del, out
    void cycle();
    ~Delay2(){delete [] alm;}
};
