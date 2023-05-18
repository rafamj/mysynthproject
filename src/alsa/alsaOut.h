//alsaOut.h
#include <alsa/asoundlib.h>
#include "../device.h"

class AlsaOut: public Device {
#define BUFSIZE 16384

  const char *deviceName;
  short buffer[BUFSIZE];
  sample *si[2];
  //void write_pcm(snd_pcm_t *handle, short *buffer, int size);
  void close();
  public:
  snd_pcm_t *handle;
  AlsaOut();
  AlsaOut(const char *devName);
  void open();
  void callback(void (*cycle)());
  ~AlsaOut();
};
