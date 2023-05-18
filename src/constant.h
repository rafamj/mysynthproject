//constant.h
#include "device.h"

class Constant: public Device {
  sample so[1];
  public:
      Constant(sample v):Device(0,0,so,1,0,0,0,0){setOutput(0,v);} //value
  };
