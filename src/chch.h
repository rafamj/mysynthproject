//chch.h
#include "device.h"
//change channel
class Chch: public Device{
  int channel;
  public:
  Chch(int ch);
  void getMessage(int n, snd_seq_event_t *m);
};
