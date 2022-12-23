//pager.h
#include "device.h"

class Pager: public Device{
  int page;
  int maxPages;
  unsigned int *in;
  int nInputs;
  unsigned int up;
  unsigned int down;
  public:
  Pager(int maxPages, unsigned int *inp, int n, unsigned int down, unsigned int up);
  void getMessage(int n, snd_seq_event_t *m);
  ~Pager();
};
