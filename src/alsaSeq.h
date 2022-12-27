//alsaSeq.h
#include <alsa/asoundlib.h>
#include "device.h"

class AlsaSeq: public Device {
  const char *deviceName;
  int channel;
  int portIn,portOut;
  bool channelOK(int ch) { return channel==-1 || channel==ch;}
  void connect(int client, int port);
  void open();
  void close();
  public:
  snd_seq_t *handle;
  AlsaSeq(int channel,const char *devName);
  void getMessage(int n, snd_seq_event_t *m);
  void callback();
  ~AlsaSeq();
};
