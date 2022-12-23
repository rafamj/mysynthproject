//poly.h
#include "device.h"

class Poly: public Device {
  int max_notes;
  bool *note_active;
  unsigned int *notes;
  public:
    Poly(int n);
    void getMessage(int n, snd_seq_event_t *m);
    ~Poly();
};
