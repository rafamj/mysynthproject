//chch.h
#include "device.h"
//change channel
class Chch: public Device{
  int channel;
  public:
  Chch(int ch);
  void getMessage(int n, midi_message_t *m);
};
