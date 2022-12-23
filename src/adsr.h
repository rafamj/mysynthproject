//adsr.h
#include "device.h"

  enum State {CLOSED, A, D, S, R};
class Adsr: public Device {
  State state;
  sample out;
  public:
  Adsr();
  void cycle();
};
