//midi2control.h
#include "device.h"

class Midi2Control: public Device {
  sample so[128];
  MidiInput mi[1];
  public:
    Midi2Control():Device(0,0,so,128,mi,1,0,0){}
    void getMessage(int n, midi_message_t *m);
};
