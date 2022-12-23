//alsaSeqMidi.h
#include <alsa/asoundlib.h>
#include "device.h"

class AlsaSeqMidi: public Device {
  const char *deviceName;
  int channel;
  //int seq_nfds;
  //struct pollfd *pfds;
  bool channelOK(int ch) { return channel==-1 || channel==ch;}
  void open();
  void close();
  public:
  snd_seq_t *seq_handle;
  AlsaSeqMidi(int channel);
  void callback();
  ~AlsaSeqMidi();
};
