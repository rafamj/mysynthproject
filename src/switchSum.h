//switch.h
#include "device.h"

class SwitchSum: public Device{
  sample *si[128];
  sample so[1];
  MidiInput mi[1];
  int note[128];
  public:
  SwitchSum();
  void getMessage(int n, midi_message_t *m);
  void cycle();
};
