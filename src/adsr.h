//adsr.h
#include "device.h"

  enum State {CLOSED, A, D, S, R};
class Adsr: public Device {
  sample *si[5];
  sample so[1];
  State state;
  sample out;
  public:
  Adsr();
  void cycle();
};
