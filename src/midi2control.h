//midi2control.h
#include "device.h"

class Midi2Control: public Device {
  public:
    Midi2Control():Device(0,128,1,0){}
    void getMessage(int n, snd_seq_event_t *m);
};
