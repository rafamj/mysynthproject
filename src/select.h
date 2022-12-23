//select.h
#include "device.h"


class Select: public Device{
  int page;
  unsigned int *in;
  int nInputs;
  unsigned int *select;
  int nSelect;
  public:
  Select(unsigned int *inp, int n, unsigned int *select, int ns);
  void getMessage(int n, snd_seq_event_t *m);
  ~Select();
};
