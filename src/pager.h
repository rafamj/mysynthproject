//pager.h
#include "device.h"
class Pager: public Device{
  sample **si;
  sample *so;
  MidiInput mi[1];
  int page;
  int maxPages;
  unsigned int *in;
  int nInputs;
  unsigned int up;
  unsigned int down;
  public:
  Pager(sample **si1, sample *soa, int maxPages, unsigned int *inp,  int n, unsigned int down, unsigned int up);
  void getMessage(int n, midi_message_t *m);
  ~Pager();
};
