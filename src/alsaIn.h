//alsaIn.h
#include <alsa/asoundlib.h>
#include "device.h"

class AlsaIn: public Device {
#define BUFSIZE 16384

  const char *deviceName;
  short buffer[BUFSIZE];
  int len;
  int index;
  //static void write_pcm(snd_pcm_t *handle, short *buffer, int size);
  void open();
  public:
  snd_pcm_t *handle;
  AlsaIn(const char *devName);
  void cycle();
  //void callback();
  ~AlsaIn();
};
