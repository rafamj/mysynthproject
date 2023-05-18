//MidiSelect.h
#include "device.h"


class MidiSelect: public Device{
  MidiInput mi[1];
  MidiOutput mo[1];
  int page;
  unsigned int *in;
  int nInputs;
  unsigned int *select;
  int nSelect;
  unsigned int *translate;
  public:
  MidiSelect(unsigned int *inp, int n, unsigned int *select, int ns, unsigned int *trans, unsigned int ntrans);
  void getMessage(int n, midi_message_t *m);
  ~MidiSelect();
};
