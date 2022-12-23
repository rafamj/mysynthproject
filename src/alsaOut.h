//alsaOut.h
#include <alsa/asoundlib.h>
#include "device.h"

class AlsaOut: public Device {
#define BUFSIZE 16384

  const char *deviceName;
  short buffer[BUFSIZE];
  //void write_pcm(snd_pcm_t *handle, short *buffer, int size);
  void open();
  void close();
  public:
  snd_pcm_t *handle;
  AlsaOut(const char *devName);
  void callback();
  ~AlsaOut();
};
