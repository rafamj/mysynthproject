//constant.h
#include "device.h"

class Constant: public Device {
  public:
      Constant(sample v):Device(0,1){setOutput(0,v);} //value
  };
