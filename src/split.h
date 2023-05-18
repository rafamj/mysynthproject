//split.h
#include "device.h"

class Split: public Device{
  int note;
  public:
  Split(int n);
  void getMessage(int n, midi_message_t *m);
};
