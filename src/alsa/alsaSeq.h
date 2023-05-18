//alsaSeq.h
//#include <alsa/asoundlib.h>
#include "../device.h"

class AlsaSeq: public Device {
  const char *deviceName;
  int channel;
  int portIn,portOut;
  MidiInput mi[1];
  MidiOutput mo[1];
  bool channelOK(int ch) { return channel==-1 || channel==ch;}
  void connect(int client, int port);
  void close();
  public:
  snd_seq_t *handle;
  AlsaSeq();
  AlsaSeq(int channel,const char *devName);
  void open();
  void getMessage(int n, midi_message_t *m);
  void callback();
  ~AlsaSeq();
};
