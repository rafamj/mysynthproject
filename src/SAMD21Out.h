//SAMD21Out.h
#include "device.h"

class SAMD21Out: public Device {
//#define BUFSIZE 16384

  //const char *deviceName;
  //short buffer[BUFSIZE];
  sample *si[2];
  void close();
  public:
  SAMD21Out();
  void open();
  void callback(void(*cycl)());
  ~SAMD21Out();
};
