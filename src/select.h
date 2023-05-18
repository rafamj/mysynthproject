//select.h
#include "device.h"


class Select: public Device{
  sample *so;
  MidiInput mi[1];
  int page;
  unsigned int *in;
  int nInputs;
  unsigned int *select;
  int nSelect;
  public:
  Select(sample *s,unsigned int *inp, int n, unsigned int *select, int ns);
  void getMessage(int n, midi_message_t *m);
  ~Select();
};
