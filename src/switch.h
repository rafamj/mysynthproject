//switch.h
#include "device.h"

class Switch: public Device{
  sample *si[128];
  sample so[128];
  MidiInput mi[1];
  int note[128];
  public:
  Switch();
  void getMessage(int n, midi_message_t *m);
  void cycle();
};
