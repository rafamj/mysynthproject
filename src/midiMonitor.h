//midiMonitor.h
#include "device.h"

class MidiMonitor: public Device {
  public:
    MidiMonitor():Device(0,0,1,0){} //in
    void getMessage(int n, snd_seq_event_t *m);
};
