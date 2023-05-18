//SAMD21Midi.h
//#include <alsa/asoundlib.h>
#include <I2S.h>
#include "wiring_private.h"

#include "device.h"

class SAMD21Midi: public Device {
  MidiInput mi[1];
  MidiOutput mo[1];
  int channel;
  bool channelOK(int ch) { return channel==-1 || channel==ch;}
  void close();
  public:
  void open();
  static Uart Serial2;
  SAMD21Midi(int channel=-1);
  void getMessage(int n, midi_message_t *m);
  void callback();
  ~SAMD21Midi();
};
