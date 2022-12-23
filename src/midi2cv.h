//midi2cv.h
#include "device.h"

class Midi2cv: public Device{
  int notes;
  public:
  Midi2cv();
  void getMessage(int n, snd_seq_event_t *m);
};
