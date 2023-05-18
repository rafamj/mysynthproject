//poly.h
#include "device.h"
class Poly: public Device {
   sample so[129];
   MidiInput mi[1];
   MidiOutput mo[129];
  int max_notes;
  bool *note_active;
  unsigned int *notes;
  public:
    Poly(int n,bool *na, unsigned int *no);
    void getMessage(int n, midi_message_t *m);
    //~Poly();
};
