//atan.h
#include "device.h"

//return (MAX_VALUE)/(MAX_VALUE/x+28125 *x/MAX_VALUE/100000);

class Atan: public Device {
  sample *si[1];
  sample so[1];
  public:
    Atan():Device(si,1,so,1,0,0,0,0){} //in, out
    void cycle() {setOutput(0,atan(getInput(0)));}
};
