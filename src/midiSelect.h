//MidiSelect.h
#include "device.h"


class MidiSelect: public Device{
  int page;
  unsigned int *in;
  int nInputs;
  unsigned int *select;
  int nSelect;
  unsigned int *translate;
  public:
  MidiSelect(unsigned int *inp, int n, unsigned int *select, int ns, unsigned int *trans);
  void getMessage(int n, snd_seq_event_t *m);
  ~MidiSelect();
};
