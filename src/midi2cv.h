//midi2cv.h
#include "device.h"

class Midi2cv: public Device{
  int notes;
  sample so[3];
  MidiInput mi[1];
  sample frequencies[128];
  public:
  Midi2cv();
  void getMessage(int n, midi_message_t *m);
};
