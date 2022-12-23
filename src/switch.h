//switch.h
#include "device.h"

class Switch: public Device{
  int note[128];
  public:
  Switch();
  void getMessage(int n, snd_seq_event_t *m);
  void cycle();
};
